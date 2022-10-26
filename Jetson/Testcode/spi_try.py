#!/usr/bin/env python
 
# loopback test script
# connect MOSI and MISO
 
import spidev
import time

def SPI_set(speed):
  # spidev-setting
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
 
try:
  spi0, spi1 = SPI_set(1000000)
  while True:
    rec_spi0 = spi0.xfer2([1,2,3,4])
    rec_spi1 = spi1.xfer2([5,6,7,8])
    print("CS0: {}, CS1:{}".format(rec_spi0,rec_spi1))
    time.sleep(1)
 
except KeyboardInterrupt:
  spi0.close()
  spi1.close()