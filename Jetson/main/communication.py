
import serial
import spidev

# Parameter Set
BAUDRATE = 115200
SPI_SPEED = 1000000

def BytesToHex(Bytes):
    return ''.join(["0x%02X " % x for x in Bytes]).strip()

# シリアル通信処理のclass
class UART():
    # コンストラクタでSerialのインスタンス生成
    def __init__(self):
        self.uart = serial.Serial('/dev/ttyUSB0', BAUDRATE)
    
    def read(self):
        self.esp32rec_ly = int(BytesToHex(self.uart.read()),16)
        self.esp32rec_rx = int(BytesToHex(self.uart.read()),16)
        self.esp32rec_id = int(BytesToHex(self.uart.read()),16)
        return self.esp32rec_ly,self.esp32rec_rx,self.esp32rec_id

    def single_read(self):
        self.esp32rec = int(BytesToHex(self.uart.read()),16)
        return self.esp32rec

    def write(self, write_data):
        self.uart.write(write_data)

# SPI通信処理のclass
class SPI():
    # コンストラクタでspidevのインスタンス生成
    def __init__(self):
        self.spi = spidev.SpiDev()            # インスタンス生成
        self.spi.open(0, 0)                   # (基本0、チップセレクト0 or 1）
        self.spi.max_speed_hz = SPI_SPEED     # 通信周波数設定
        # SPIモードは1が一番ノイズ耐性高い
        # self.spi.mode = 1                     # デフォルト0のため書き込み不要。MOSIのシフト、SCLKのサンプリング立ち上がり or 立ち下がり 
    
    # def send_rec(self, nmtgtm1, nmtgtm2, id):
    #     # ここにbit処理した上でSPI送受信処理


def SPI_set(speed):
    # チップセレクトごとにspiインスタンス生成
    spi0 = spidev.SpiDev()
    spi1 = spidev.SpiDev()
    spi0.open(0, 0)                # (基本0、チップセレクト0 or 1）
    spi0.max_speed_hz = speed      #通信周波数設定。最低100kHz。
    spi1.open(0, 1)                # (基本0、チップセレクト0 or 1）
    spi1.max_speed_hz = speed      #通信周波数設定。最低100kHz。
    spi0.mode = 0               #デフォルト0のため書き込み不要。MOSIのシフト、SCLKのサンプリング立ち上がり or 立ち下がり
    spi1.mode = 0               #デフォルト0のため書き込み不要。MOSIのシフト、SCLKのサンプリング立ち上がり or 立ち下がり
    return spi0, spi1
