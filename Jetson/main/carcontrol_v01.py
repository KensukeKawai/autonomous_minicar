# General Import
import time
import RPi.GPIO as GPIO

# My Module import
import strcontrol as str
import communication as cm

# Variable
global nmtgtm1, nmtgtm2
# cnt_pwm_off = 0
# flag_pwm_off = 1

# Const（ピン番号はPIN、IDはID）
ID_MODE_STOP = 0
ID_MODE_NORMAL = 1
ID_MODE_MTORIGIN = 2
# CNT_PWM_OFF = 2
PIN_SERVO_SUPPLY = 13       # サーボ電源のON/OFF用GPIO
PIN_PWMOUT_FRONT = 32
PIN_PWMOUT_REAR = 33
SPI_CL_FRONT = 0           # SPIインスタンス生成時のチップセレクト。フロント側。
SPI_CL_REAR = 1

# Instance
uart = cm.UART()
spi_front = cm.SPI(SPI_CL_FRONT)
spi_rear = cm.SPI(SPI_CL_REAR)
str_front = str.PWM(PIN_PWMOUT_FRONT)
str_rear = str.PWM(PIN_PWMOUT_REAR)

# 汎用初期化処理
GPIO.setmode(GPIO.BOARD)

# Loop
while True:
    rec_first = uart.single_read()

    # Wait Controller
    while rec_first == 255:
        print("Pushed 'Start Button' of PS3 Controller")
        time.sleep(1)
        uart.write(255)
        GPIO.setup(PIN_SERVO_SUPPLY, GPIO.OUT, initial=GPIO.LOW)

        # Control Start
        while True:
            esp32rec_ly,esp32rec_rx,esp32rec_id = uart.read()   # Get Controller Value

            if esp32rec_id == ID_MODE_NORMAL:
                id_send_cntrlr = ID_MODE_NORMAL
                str_front.FrontOut(esp32rec_rx)
                str_rear.RearOut(esp32rec_rx)
                GPIO.output(PIN_SERVO_SUPPLY, GPIO.HIGH)

            elif esp32rec_id == ID_MODE_MTORIGIN:
                id_send_cntrlr = ID_MODE_MTORIGIN
                str_front.FrontOut(127)
                str_rear.RearOut(127)
                GPIO.output(PIN_SERVO_SUPPLY, GPIO.LOW)

            else:   # STOP
                id_send_cntrlr = ID_MODE_STOP
                str_front.FrontOut(127)
                str_rear.RearOut(127)
                GPIO.output(PIN_SERVO_SUPPLY, GPIO.LOW)

            # # SPI Send
            rec_spi1 = spi_front.xfer2([esp32rec_ly,esp32rec_ly,esp32rec_rx,esp32rec_rx])    # MSB First
            print(rec_spi1)

# except KeyboardInterrupt:
    # pwm1.stop()
    # pwm2.stop()
    # uart.close()
    # spi0.close()