
import serial
import spidev
import globalval as g

# Parameter Set
BAUDRATE = 115200
SPI_SPEED = 100000  # 基本、設定できる最低の100kHzにする。最大200kHzくらい

BITMASK_MODE_NORMAL = 1<<28
BITMASK_MODE_MTORIGIN = 1<<29
BITMASK_MODE_STOP = 1<<30
BITMASK_ROTDIR = 1<<13
BITMASK_NMTGT = 0x1FFF
BITSHIFT_NM2 = 14

# xfer2でSPI送信する際のバイト分割
# BITMASK_B1 = 0x000000FF
# BITMASK_B2 = 0x0000FF00
# BITMASK_B3 = 0x00FF0000
# BITMASK_B4 = 0xFF000000
BITMASK_BYTE = 0xFF


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
    def sendrec(self, nmtgtm1, nmtgtm2, id):
        # 初期化
        self.nmtgtm1 = 0
        self.nmtgtm2 = 0

        # モータ目標回転数
        if nmtgtm1 < 0:
            self.nmtgtm1 = (abs(nmtgtm1) & BITMASK_NMTGT) | BITMASK_ROTDIR
        else:
            self.nmtgtm1 = nmtgtm1 & BITMASK_NMTGT
        if nmtgtm2 < 0:
            self.nmtgtm2 = (abs(nmtgtm2) & BITMASK_NMTGT) | BITMASK_ROTDIR
        else:
            self.nmtgtm2 = nmtgtm2 & BITMASK_NMTGT

        self.nmtgt = (self.nmtgtm2<<BITSHIFT_NM2) | self.nmtgtm1

        # ID設定
        if id == g.ID_MODE_NORMAL:
            self.senddata = self.nmtgtm1 | BITMASK_MODE_NORMAL
        elif id == g.ID_MODE_MTORIGIN:
            self.senddata = self.nmtgtm1 | BITMASK_MODE_MTORIGIN
        elif id == g.ID_MODE_STOP:
            self.senddata = self.nmtgtm1 | BITMASK_MODE_STOP
        
        # 送信データビット処理して送信
        self.send_b1 = self.senddata & BITMASK_BYTE
        self.send_b2 = (self.senddata>>8) & BITMASK_BYTE
        self.send_b3 = (self.senddata>>16) & BITMASK_BYTE
        self.send_b4 = (self.senddata>>24) & BITMASK_BYTE
        self.rec_data = self.spi.xfer2([self.b4,self.b3,self.b2,self.b1])    # MSB First

        # 受信データ処理
        if (self.rec_data & BITMASK_MODE_NORMAL) == 1:
            self.rec_id = g.ID_MODE_NORMAL
        elif (self.rec_data & BITMASK_MODE_MTORIGIN) == 1:
            self.rec_id = g.ID_MODE_MTORIGIN
        elif (self.rec_data & BITMASK_MODE_STOP) == 1:
            self.rec_id = g.ID_MODE_STOP

        if (self.rec_data & BITMASK_ROTDIR) == 0:
            self.rec_nm1 = self.rec_data & BITMASK_NMTGT
        else:
            self.rec_nm1 = -1 * (self.rec_data & BITMASK_NMTGT)

        self.nm2pre = self.rec_data>>BITSHIFT_NM2
        if (self.nm2pre & BITMASK_ROTDIR) == 0:
            self.rec_nm2 = self.nm2pre & BITMASK_NMTGT
        else:
            self.rec_nm2 = -1 * (self.nm2pre & BITMASK_NMTGT)

        return self.rec_id, self.rec_nm1, self.rec_nm2