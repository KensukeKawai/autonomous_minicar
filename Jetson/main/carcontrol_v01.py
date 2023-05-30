# General Import
import time
import signal
import threading
import RPi.GPIO as GPIO

# My Module import
import strcontrol as str
import communication as cm

# Variable
global nmtgtm1, nmtgtm2, tt
global now_time
now_time = 0

# Const（ピン番号はPIN、IDはID）
ID_MODE_STOP = 0
ID_MODE_NORMAL = 1
ID_MODE_MTORIGIN = 2
PIN_SERVO_SUPPLY = 13       # サーボ電源のON/OFF用GPIO
PIN_PWMOUT_FRONT = 32
PIN_PWMOUT_REAR = 33
SPI_CL_FRONT = 0           # SPIインスタンス生成時のチップセレクト。フロント側。
SPI_CL_REAR = 1
T_MAIN_TGT = 0.1
T_OFFSET = T_MAIN_TGT * 6
T_MAIN = T_MAIN_TGT - T_OFFSET/1000

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

def strstop():
    str_front.FrontOut(127)
    str_rear.RearOut(127)
    GPIO.output(PIN_SERVO_SUPPLY, GPIO.LOW)

def main_func():
    global now_time
    old_time = now_time
    now_time = time.time()
    # esp32rec_ly,esp32rec_rx,esp32rec_id = uart.read()   # Get Controller Value

    # if esp32rec_id == ID_MODE_NORMAL:
    #     id_send_cntrlr = ID_MODE_NORMAL
    #     # Steering Control
    #     # str_front.FrontOut(esp32rec_rx)
    #     # str_rear.RearOut(esp32rec_rx)
    #     GPIO.output(PIN_SERVO_SUPPLY, GPIO.HIGH)

    # elif esp32rec_id == ID_MODE_MTORIGIN:
    #     id_send_cntrlr = ID_MODE_MTORIGIN
    #     strstop()

    # else:   # STOP
    #     id_send_cntrlr = ID_MODE_STOP
    #     strstop()

    # # # SPI Send
    # rec_spi1 = spi_front.xfer2([esp32rec_ly,esp32rec_ly,esp32rec_rx,esp32rec_rx])    # MSB First
    # print(rec_spi1)
    print("実周期時間：{:.5f}[ms], バラツキ：{:.5f}[ms]".format((now_time-old_time)*1000,((now_time-old_time)-T_MAIN_TGT)*1000))

    tt = threading.Timer(T_MAIN, main_func)
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

        tt = threading.Timer(T_MAIN, main_func)
        tt.start()

        # Control Start
        while True:
            # pass
            time.sleep(2)

            # except KeyboardInterrupt:
            # #     strstop()
            # #     uart.close()
            # #     spi_front.close()
            # #     spi_rear.close()
                # break