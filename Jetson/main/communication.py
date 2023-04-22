
import serial
import spidev

# Parameter Set
BAUDRATE = 115200
SPI_SPEED = 100000  # 基本、設定できる最低の100kHzにする。最大200kHzくらい

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
    def __init__(self,chipselect):
        self.spi = spidev.SpiDev()            # インスタンス生成
        self.spi.open(0, chipselect)          # (基本0、チップセレクト0 or 1）
        self.spi.max_speed_hz = SPI_SPEED     # 通信周波数設定
        # SPIモードは1が一番ノイズ耐性高い
        # self.spi.mode = 1                     # デフォルト0のため書き込み不要。MOSIのシフト、SCLKのサンプリング立ち上がり or 立ち下がり 

    # bit処理してSPI送受信処理
    def send(self, nmtgtm1, nmtgtm2, id):
        #Const
        BITMASK_MODE_NORMAL = 1<<28
        BITMASK_MODE_MTORIGIN = 1<<29
        BITMASK_MODE_STOP = 1<<30
        BITMASK_ROTDIR = 1<<13
        BITMASK_NMTGT = 0x1FFF
        BITSHIFT_NM2 = 14
        # モータ1処理
        self.nmtgtm1 = nmtgtm1 | BITMASK_ROTDIR

        # モータ2処理
        self.nmtgtm2 = nmtgtm2 << BITSHIFT_NM2

        # ID処理
        self.id