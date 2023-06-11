
import serial
import spidev
import globalval as g

# Parameter Set
BAUDRATE = 115200
SPI_SPEED = 100000                  # 基本、設定できる最低の100kHzにする。最大200kHzくらい
SPI_CL_FRONT = 0                    # SPIインスタンス生成時のチップセレクト。フロント側。
SPI_CL_REAR = 1

BITMASK_MODE_RUN = 1<<28
BITMASK_MODE_ORG = 2<<28
BITMASK_MODE_RUNTOSTP = 3<<28
BITMASK_ROTDIR = 1<<13
BITMASK_NMTGT = 0x1FFF
BITMASK_MODE = 0xF0000000
BITMASK_BYTE = 0xFF                 # xfer2でSPI送信する際のバイト分割
BITSHIFT_NM2 = 14
BITSHIFT_MODE = 28

def BytesToHex(Bytes):
    return ''.join(["0x%02X " % x for x in Bytes]).strip()

# シリアル通信処理のclass
class UART():
    # コンストラクタでSerialのインスタンス生成
    def __init__(self):
        self.uart = serial.Serial('/dev/ttyUSB0', BAUDRATE)
    
    def read(self):
        global esp32rec_id_z
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
        if id == g.ID_MODE_RUN:
            self.senddata = self.nmtgt | BITMASK_MODE_RUN
        elif id == g.ID_MODE_ORG:
            self.senddata = self.nmtgt | BITMASK_MODE_ORG
        elif id == g.ID_MODE_STP:
            # self.senddata = self.nmtgt | BITMASK_MODE_STP
            self.senddata = self.nmtgt
        
        # 送信データビット処理して送信
        self.send_B1 = self.senddata & BITMASK_BYTE
        self.send_B2 = (self.senddata>>8) & BITMASK_BYTE
        self.send_B3 = (self.senddata>>16) & BITMASK_BYTE
        self.send_B4 = (self.senddata>>24) & BITMASK_BYTE
        self.rec_data = self.spi.xfer2([self.B4,self.B3,self.B2,self.B1])    # MSB First

        # 受信データ処理
        self.rec_id = self.rec_data >> BITSHIFT_MODE
        if self.rec_mode > g.ID_MODE_MAX:
            self.rec_id = g.ID_MODE_STP

        if (self.rec_data & BITMASK_ROTDIR) == 0:
            self.rec_nm1 = self.rec_data & BITMASK_NMTGT
        else:
            self.rec_nm1 = -1 * (self.rec_data & BITMASK_NMTGT)

        self.nm2pre = self.rec_data>>BITSHIFT_NM2
        if (self.nm2pre & BITMASK_ROTDIR) == 0:
            self.rec_nm2 = self.nm2pre & BITMASK_NMTGT
        else:
            self.rec_nm2 = -1 * (self.nm2pre & BITMASK_NMTGT)

        return self.rec_nm1, self.rec_nm2
    
    def rec_only_id(self):
        # Jetson受信のみの場合はIDをFFにしてマイコン側に通達
        self.rec_data = self.spi.xfer2([0xF0,0x00,0x00,0x00])    # MSB First
        self.rec_id = self.rec_data >> BITSHIFT_MODE

        if self.rec_mode > g.ID_MODE_MAX:
            self.rec_id = g.ID_MODE_STP
        
        return self.rec_id