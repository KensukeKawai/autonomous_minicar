
import RPi.GPIO as GPIO
import serial
import spidev
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

def SPI_set(speed):
  # チップセレクトごとにspiインスタンス生成
  spi0 = spidev.SpiDev()
  spi1 = spidev.SpiDev()
  spi0.open(0, 0)              #（基本0、チップセレクト「0 or 1」）
  spi0.max_speed_hz = speed  #通信周波数指定
  spi1.open(0, 1)              #（基本0、チップセレクト「0 or 1」）
  spi1.max_speed_hz = speed  #通信周波数指定
  # spi.mode = 0               #デフォルト0のため書き込み不要。MOSIのシフト、SCLKのサンプリング（立ち上がり or 立ち下がり）
  return spi0, spi1

def BytesToHex(Bytes):
    return ''.join(["0x%02X " % x for x in Bytes]).strip()

def main():
    try:
        rec_ly = rec_rx = 0
        # button = "OFF"
        uart = UART_set(115200)
        spi0, spi1 = SPI_set(1000000)
        pwm1, pwm2 = PWM_set(300, 300)

        while True:
            rec_first = int(BytesToHex(uart.read()),16)
            while rec_first == 255:
                print("Pushed 'Start Button' of PS3 Controller")
                time.sleep(1)
                uart.write(255)

                while True:
                    esp32rec_ly = int(BytesToHex(uart.read()),16)
                    esp32rec_rx = int(BytesToHex(uart.read()),16)
                    esp32rec_id = int(BytesToHex(uart.read()),16)
                    rec_spi0 = spi0.xfer2([rec_ly,rec_ly,rec_ly,rec_ly])
                    rec_spi1 = spi1.xfer2([rec_rx,rec_rx,rec_rx,rec_rx])
                    pwm1.start(rec_ly/255*100)
                    pwm2.start(rec_rx/255*100)
                    # rec_button = int(BytesToHex(uart.read()),16)
                    # if rec_button != 0: button = "ON"
                    # print("Ly : {} , Rx : {} , Button : {}".format(rec_ly,rec_rx,button))
                    print("Ly:{:03} , Rx:{:03}, ID:{}, CS0: {}, CS1:{}".format(esp32rec_ly,esp32rec_rx,esp32rec_id,rec_spi0,rec_spi1))

    except KeyboardInterrupt:
        pwm1.stop()
        pwm2.stop()
        uart.close()
        spi0.close()
        spi1.close()

if __name__ == "__main__":
    main()