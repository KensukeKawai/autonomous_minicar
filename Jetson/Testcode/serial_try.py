import serial
import time

uart = serial.Serial('/dev/ttyUSB0', 115200)  # デバイス名とボーレートを設定しポートをオープン 
uart.write(str.encode('a'))      # 出力

def BytesToHex(Bytes):
  return ''.join(["0x%02X " % x for x in Bytes]).strip()

try:
  while True:
    rec_ly = uart.read()
    rec_rx = uart.read()
    print("Ly : {} , Rx : {}".format(int(BytesToHex(rec_ly),16),int(BytesToHex(rec_rx),16)))
 
except KeyboardInterrupt:
    uart.close()             # ポートのクローズ