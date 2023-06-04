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

# Variable
global tt, now_time
now_time = 0
g.rec_f_id = g.ID_MODE_STOP
g.nmtgtl = 0
g.nmtgtr = 0

# Const
# PIN設定
PIN_SERVO_SUPPLY = 13               # サーボ電源のON/OFF用GPIO
PIN_PWMOUT_FRONT = 32
PIN_PWMOUT_REAR = 33
# 内部API設定
SPI_CL_FRONT = 0                    # SPIインスタンス生成時のチップセレクト。フロント側。
SPI_CL_REAR = 1

STICK_CENTER = 127                          # スティックの受信中心値
STICK_HYS = 3                               # スティックの中心ヒス
STICK_FWD = 128 - STICK_HYS                 # ヒス処理した正方向のスティック値
STICK_REV = 127 - STICK_HYS                 # ヒス処理した負方向のスティック値

# 汎用初期化処理
GPIO.setwarnings(False)             # PWMのインスタンス生成時にワーニング出るため消し去る
GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN_SERVO_SUPPLY, GPIO.OUT, initial=GPIO.LOW)

# Instance
uart = cm.UART()
spi_front = cm.SPI(SPI_CL_FRONT)
spi_rear = cm.SPI(SPI_CL_REAR)
str_front = str.STRCNT(PIN_PWMOUT_FRONT)
str_rear = str.STRCNT(PIN_PWMOUT_REAR)
nmtgt = nmt.NMTGT()

# スティックデータをヒス処理して正規化
def sticknml(ly, rx):
    stick_ly = ly - STICK_CENTER
    stick_rx = rx - STICK_CENTER

    # 左スティック
    # スティックが前進方向
    if stick_ly > STICK_HYS:      # ヒス分より大きい場合は正回転
        sticknml_ly = (stick_ly - STICK_HYS) / STICK_FWD
    # スティックが後進方向
    elif stick_ly < -STICK_HYS:
        sticknml_ly = (stick_ly + STICK_HYS) / STICK_REV
    # スティックが中心値付近
    else:
        sticknml_ly = 0
        
    # 右スティック
    # スティックが前進方向
    if stick_rx > STICK_HYS:      # ヒス分より大きい場合は正回転
        sticknml_rx = (stick_rx - STICK_HYS) / STICK_FWD
    # スティックが後進方向
    elif stick_rx < -STICK_HYS:
        sticknml_rx = (stick_rx + STICK_HYS) / STICK_REV
    # スティックが中心値付近
    else:
        sticknml_rx = 0
    return sticknml_ly, sticknml_rx

def strstop():
    str_front.FrontOut(127)
    str_rear.RearOut(127)
    GPIO.output(PIN_SERVO_SUPPLY, GPIO.LOW)

def strset(rx):
        str_front.FrontOut(rx)
        str_rear.RearOut(rx)
        GPIO.output(PIN_SERVO_SUPPLY, GPIO.HIGH)

def esp32rec():
    esp32rec_ly,esp32rec_rx,esp32rec_id = uart.read()   # Get Controller Value

    # 操作コントローラ側がNORMAL（○ボタン)
    if esp32rec_id == g.ID_MODE_NORMAL:
        # モータコントローラ側がノーマル状態
        if g.rec_f_id == g.ID_MODE_NORMAL:
            id_set = g.ID_MODE_NORMAL
            # スティックデータの正規化
            sticknml_ly, sticknml_rx = sticknml(esp32rec_ly, esp32rec_rx)
            # モータ目標回転数算出
            g.nmtgtl, g.nmtgtr = nmtgt.nmtgtcal(sticknml_ly, sticknml_rx)
            # 前後ステアの操舵指示
            strset(sticknml_rx)
        # モータコントローラ側がノーマル状態以外の場合は全て停止させておく
        else:
            strstop()
            g.nmtgtl = 0
            g.nmtgtr = 0

    # 操作コントローラ側が原点学習要求（△ボタン)
    elif esp32rec_id == g.ID_MODE_MTORIGIN:
        strstop()
        # モータコントローラ側がまだ原点学習処理中だったらそのまま原点学習要求を立てておく
        if g.rec_f_id == g.ID_MODE_MTORIGIN:
            id_set = g.ID_MODE_MTORIGIN
        # モータコントローラ側が原点学習完了しSTOP状態だったらNORMAL要求を立ててモードを切り替える
        elif g.rec_f_id == g.ID_MODE_STOP:
            id_set = g.ID_MODE_NORMAL
        g.nmtgtl = 0
        g.nmtgtr = 0

    #  操作コントローラ側が停止要求（×ボタン)
    else:   # STOP
        strstop()
        id_set = g.ID_MODE_STOP
        g.nmtgtl = 0
        g.nmtgtr = 0
    
    g.rec_f_id, g.rec_f_nml, g.rec_f_nmr = spi_front.sendrec(g.nmtgtl,g.nmtgtr,id_set)

def main_func():
    global now_time
    old_time = now_time
    now_time = time.time()



    print("実周期時間：{:.5f}[ms], バラツキ：{:.5f}[ms]".format((now_time-old_time)*1000,((now_time-old_time)-g.T_MAIN_TGT)*1000))
    tt = threading.Timer(g.T_MAIN, main_func)
    tt.start()

# Loop
while True:
    # rec_first = uart.single_read()
    rec_first = 255

    # Wait Controller
    while rec_first == 255:
        print("Pushed 'Start Button' of PS3 Controller")
        time.sleep(1)
        # uart.write(255)

        # 初回でtt定義とstartメソッド呼び出し
        tt = threading.Timer(g.T_MAIN, main_func)
        tt.start()

        # Control Start
        while True:
            time.sleep(2)

            # except KeyboardInterrupt:
            # #     strstop()
            # #     uart.close()
            # #     spi_front.close()
            # #     spi_rear.close()
                # break