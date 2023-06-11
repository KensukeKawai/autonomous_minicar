
# from inspect import Parameter
import RPi.GPIO as GPIO

import globalval as g

# *****Parameter Set ***** #
# PWM Parameter
F_CARRIER = 300
DUTY_MID_FR = 45
DUTY_MID_RR = 45
DDUTY_FR = 3
DDUTY_RR = 3


class STRCNT():
    # コンストラクタでインスタンス生成
    def __init__(self,output):
        # PWM GPIO Initialize
        # GPIO.setmode(GPIO.BOARD)
        GPIO.setup(output, GPIO.OUT, initial=GPIO.HIGH)
        self.str = GPIO.PWM(output, F_CARRIER)
        self.status_front = 0
        self.status_rear = 0

    # メソッド
    def FrontOut(self,rx_norm):
        self.duty= rx_norm * DDUTY_FR + DUTY_MID_FR
        if self.status_front == 0:
            self.str.start(self.duty)
        else:
            self.str.ChangeDutyCycle(self.duty)
            self.status_front = 1

    def RearOut(self,rx_norm):
        self.duty = -1 * rx_norm * DDUTY_RR + DUTY_MID_FR
        if self.status_rear == 0:
            self.str.start(self.duty)
        else:
            self.str.ChangeDutyCycle(self.duty)
            self.status_rear = 1

def strstop(self):
    self.str_front.FrontOut(127)
    self.str_rear.RearOut(127)
    GPIO.output(g.PIN_SERVO_SUPPLY, GPIO.LOW)

def strset(self, rx_norm):
    self.str_front.FrontOut(rx_norm)
    self.str_rear.RearOut(rx_norm)
    GPIO.output(g.PIN_SERVO_SUPPLY, GPIO.HIGH)
