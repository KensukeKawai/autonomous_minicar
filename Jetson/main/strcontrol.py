
# from inspect import Parameter
import RPi.GPIO as GPIO

# *****Parameter Set ***** #
# Stick Value HYS


# PWM Parameter
F_CARRIER = 300
DUTY_MID_FR = 45
DUTY_MID_RR = 45
DDUTY_FR = 3
DDUTY_RR = 3

# DUTY_MAX = 100
# DUTY_MAX_FR = DUTY_MID_FR + DDUTY_FR
# DUTY_MIN_FR = DUTY_MID_FR - DDUTY_FR
# KSLOPE_DUTY_FR = DDUTY_FR * 2
# DUTY_MAX_RR = DUTY_MID_RR + DDUTY_RR
# DUTY_MIN_RR = DUTY_MID_RR - DDUTY_RR
# KSLOPE_DUTY_RR = DDUTY_RR * 2


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
    def FrontOut(self,rx_nml):
        self.duty= rx_nml * DDUTY_FR + DUTY_MID_FR
        if self.status_front == 0:
            self.str.start(self.duty)
        else:
            self.str.ChangeDutyCycle(self.duty)
            self.status_front = 1

    def RearOut(self,rx_nml):
        self.duty = -1 * rx_nml * DDUTY_RR + DUTY_MID_FR
        if self.status_rear == 0:
            self.str.start(self.duty)
        else:
            self.str.ChangeDutyCycle(self.duty)
            self.status_rear = 1