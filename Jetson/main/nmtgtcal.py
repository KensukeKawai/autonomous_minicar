
import globalval as g

# *****Constant *****
NMMAX = 2000                                 # 最大モータ目標回転数
KMAXNMDIFF = 0.3                            # 操舵時の左右モータ差動率最大値
RT_NMTGT = NMMAX * g.T_MAIN_TGT             # 1秒でNMMAXに達するレートにしておく

class NMTGT():
    # def __init__():

    def nmtgtcal(self, nml_ly, nml_rx):
        # 左右の差動は前後で共通で良いため共通で計算
        self.nmtgtnf_l = int(nml_ly * NMMAX * ( 1 + nml_rx * KMAXNMDIFF))     # 左スティック分×(1+右スティック×差動率最大値)
        self.nmtgtnf_r = int(nml_ly * NMMAX * ( 1 - nml_rx * KMAXNMDIFF))    # 左スティック分×(1-右スティック×差動率最大値)

        self.nmtgt_l = self.nmtgtnf_l
        self.nmtgt_r = self.nmtgtnf_r
        
        return self.nmtgt_l, self.nmtgt_r