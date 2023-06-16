# General Import
import time
import signal
import threading
import RPi.GPIO as GPIO

# My Module import
import globalval as g
import strcontrol as str
import communication as cm
import nmtgtcal as nmt
import handcontroller as hc

# Variable
# initialize gloval value
global tt, now_time
now_time = 0
g.rec_f_id = g.ID_MODE_STP
g.id_handcon_z = g.ID_MODE_STP
g.nmtgt_l = 0
g.nmtgt_r = 0

# Const

# General Initialize
GPIO.setwarnings(False)             # PWMのインスタンス生成時にワーニング出るため消し去る
GPIO.setmode(GPIO.BOARD)
GPIO.setup(g.PIN_SERVO_SUPPLY, GPIO.OUT, initial=GPIO.LOW)

# Make Instance of Class
uart = cm.UART()
spi_front = cm.SPI(cm.SPI_CL_FRONT)
spi_rear = cm.SPI(cm.SPI_CL_REAR)
str_front = str.STRCNT(g.PIN_PWMOUT_FRONT)
str_rear = str.STRCNT(g.PIN_PWMOUT_REAR)
nmtgt = nmt.NMTGT()


def strstop():
    str_front.FrontOut(0)
    str_rear.RearOut(0)
    GPIO.output(g.PIN_SERVO_SUPPLY, GPIO.LOW)

def strset(rx_norm):
    str_front.FrontOut(rx_norm)
    str_rear.RearOut(rx_norm)
    GPIO.output(g.PIN_SERVO_SUPPLY, GPIO.HIGH)

def cont_actuator():
    stick_ly,stick_rx,id_handcon = uart.read()   # Get Controller Value
    id_handconmed = hc.idcal(id_handcon)

    # ID受信
    g.id_fmot = spi_front.rec_only_id()
    g.id_rmot = spi_rear.rec_only_id()

    # コントローラとID状態から指定ID判断
    if g.id_fmot == g.ID_MODE_STP:
        g.id_fmotreq = id_handconmed
    elif g.id_fmot == g.ID_MODE_RUN:
        if id_handconmed == g.ID_MODE_ORG:
            g.id_fmotreq = g.ID_MODE_RUN
        else:
            g.id_fmotreq = id_handconmed
    elif g.id_fmot == g.ID_MODE_ORG:
        g.id_fmotreq = g.ID_MODE_ORG
    elif g.id_fmot == g.ID_MODE_RUNTOSTP:
        g.id_fmotreq = g.ID_MODE_STP
    else:
        g.id_fmotreq = g.ID_MODE_STP

    if g.id_rmot == g.ID_MODE_STP:
        g.id_rmotreq = id_handconmed
    elif g.id_rmot == g.ID_MODE_RUN:
        if id_handconmed == g.ID_MODE_ORG:
            g.id_rmotreq = g.ID_MODE_RUN
        else:
            g.id_rmotreq = id_handconmed
    elif g.id_rmot == g.ID_MODE_ORG:
        g.id_rmotreq = g.ID_MODE_ORG
    elif g.id_rmot == g.ID_MODE_RUNTOSTP:
        g.id_rmotreq = g.ID_MODE_STP
    else:
        g.id_rmotreq = g.ID_MODE_STP


    g.id_rmotreq = g.ID_MODE_RUN        # デバッグ用

    # 指定IDに応じてnmtgt決定（値自体は前回状態から100msかけて算出した結果→位置推定とかの処理考慮）
    if (g.id_fmotreq == g.ID_MODE_RUN) & (g.id_rmotreq == g.ID_MODE_RUN):       # 前後モータにRUN指令できる場合
        sticknorm_ly, sticknorm_rx = hc.sticknorm(stick_ly, stick_rx)       # スティックデータの正規化
        g.nmtgt_l, g.nmtgt_r = nmtgt.nmtgtcal(sticknorm_ly, sticknorm_rx)   # モータ目標回転数算出
        strset(sticknorm_rx)                                            # 前後ステアの操舵指示
    else:
        g.nmtgt_l = 0
        g.nmtgt_r = 0
        strstop()
        # sticknorm_ly, sticknorm_rx = hc.sticknorm(stick_ly, stick_rx)       # スティックデータの正規化
        # strset(sticknorm_rx)

    # 指定ID、nmtgtを送信
    g.nmact_fl, g.nmact_fr = spi_front.sendrec(g.nmtgt_l,g.nmtgt_r,g.id_fmotreq)
    print(id_handcon,id_handconmed,g.id_fmotreq,g.id_fmot)

    # 次周期に向けたnmtgt算出処理


# def main_func():
#     global now_time
#     old_time = now_time
#     now_time = time.time()

#     cont_actuator()

#     # print("実周期時間：{:.5f}[ms], バラツキ：{:.5f}[ms]".format((now_time-old_time)*1000,((now_time-old_time)-g.T_MAIN_TGT)*1000))
#     tt = threading.Timer(g.T_MAIN, main_func)
#     tt.start()

# Loop
while True:
    rec_first = uart.single_read()

    # Wait Controller
    while rec_first == 255:
        print("Pushed 'Start Button' of PS3 Controller")
        time.sleep(1)
        uart.write(255)

        # 初回でtt定義とstartメソッド呼び出し
        # tt = threading.Timer(g.T_MAIN, main_func)
        # tt.start()

        # Control Start
        while True:
            cont_actuator()
            # time.sleep(0.2)