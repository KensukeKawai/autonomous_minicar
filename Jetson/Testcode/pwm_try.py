import RPi.GPIO as GPIO
import time

output_pin1 = 32
output_pin2 = 33

pwm_carrier_p1 = 300
pwm_carrier_p2 = 300


GPIO.setmode(GPIO.BOARD)
GPIO.setup(output_pin1, GPIO.OUT, initial=GPIO.HIGH)
p1 = GPIO.PWM(output_pin1, pwm_carrier_p1)
GPIO.setup(output_pin2, GPIO.OUT, initial=GPIO.HIGH)
p2 = GPIO.PWM(output_pin2, pwm_carrier_p2)

p1.start(25)
p2.start(25)

try:
    while True:
        time.sleep(1)

except KeyboardInterrupt:
    p1.stop()
    p2.stop()
    GPIO.cleanup()