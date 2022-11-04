
# from inspect import Parameter
# from typing import Set
import RPi.GPIO as GPIO
import serial
import spidev


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
  spi0.open(0, 0)                # (基本0、チップセレクト0 or 1）
  spi0.max_speed_hz = speed      #通信周波数設定
  spi1.open(0, 1)                # (基本0、チップセレクト0 or 1）
  spi1.max_speed_hz = speed      #通信周波数設定
    # spi.mode = 0               #デフォルト0のため書き込み不要。MOSIのシフト、SCLKのサンプリング立ち上がり or 立ち下がり
  return spi0, spi1

def BytesToHex(Bytes):
    return ''.join(["0x%02X " % x for x in Bytes]).strip()