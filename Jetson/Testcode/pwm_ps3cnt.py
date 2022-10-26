
from multiprocessing.connection import wait
import RPi.GPIO as GPIO
import serial
import time

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

def UART_set(baudrate):
    # デバイス名とボーレートを設定しポートをオープン 
    uart = serial.Serial('/dev/ttyUSB0', baudrate)
    return uart

def BytesToHex(Bytes):
    return ''.join(["0x%02X " % x for x in Bytes]).strip()

def main():
    try:
        rec_ly = 0
        rec_rx = 0
        button = "OFF"
        pwm1, pwm2 = PWM_set(300, 300)
        uart = UART_set(115200)

        while True:
            rec_first = int(BytesToHex(uart.read()),16)
            while rec_first == 255:
                print(rec_first)
                time.sleep(1)
                # uart.write(str.encode('a'))      # 出力
                uart.write(255)

                while True:
                    rec_ly = int(BytesToHex(uart.read()),16)
                    rec_rx = int(BytesToHex(uart.read()),16)
                    # rec_button = int(BytesToHex(uart.read()),16)
                    # if rec_button != 0: button = "ON"
                    # print("Ly : {} , Rx : {} , Button : {}".format(rec_ly,rec_rx,button))
                    print("Ly : {} , Rx : {}".format(rec_ly,rec_rx))

                    pwm1.start(rec_ly/255*100)
                    pwm2.start(rec_rx/255*100)

    except KeyboardInterrupt:
        pwm1.stop()
        pwm2.stop()
        uart.close()

if __name__ == "__main__":
    main()