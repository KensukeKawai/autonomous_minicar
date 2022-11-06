
# from inspect import Parameter
import RPi.GPIO as GPIO

# *****Parameter Set ***** #
# PWM Parameter
F_CARRIER = 300
DUTY_MID_FR = 45
DUTY_MID_RR = 45
DDUTY_FR = 3
DDUTY_RR = 3
DUTY_MAX = 100

DUTY_MAX_FR = DUTY_MID_FR + DDUTY_FR
DUTY_MIN_FR = DUTY_MID_FR - DDUTY_FR
KSLOPE_DUTY_FR = (DUTY_MAX_FR-DUTY_MIN_FR)/255
DUTY_MAX_RR = DUTY_MID_RR + DDUTY_RR
DUTY_MIN_RR = DUTY_MID_RR - DDUTY_RR
KSLOPE_DUTY_RR = (DUTY_MAX_RR-DUTY_MIN_RR)/255

def PWM_set(p1_carrier,p2_carrier):
    # PWM Parameter Set
    output_pin1 = 32
    output_pin2 = 33
    # PWM GPIO Initialize
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(output_pin1, GPIO.OUT, initial=GPIO.HIGH)
    p1 = GPIO.PWM(output_pin1, p1_carrier)
    GPIO.setup(output_pin2, GPIO.OUT, initial=GPIO.HIGH)
    p2 = GPIO.PWM(output_pin2, p2_carrier)
    return p1, p2