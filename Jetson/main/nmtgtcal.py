
# *****Constant *****
# nmtgtnfcal
STICK_CENTER = 127                          # 左スティックの受信中心値
STICK_HYS = 3                               # 左スティックの中心ヒス
NMMAX = 500                                 # 最大モータ目標回転数
KSTICKTONM_FWD = NMMAX / (STICK_CENTER+STICK_HYS)   # 左スティック⇛目標回転数変換係数
KSTICKTONM_REV = NMMAX / (STICK_CENTER-STICK_HYS)   # 左スティック⇛目標回転数変換係数
NMROT_FWD = 1
NMROT_REV = 0

class NMTGT():
    # def __init__():

    def nmtgtnfcal(self, ly):
        self.stick = ly - STICK_CENTER

        # スティックが前進方向
        if self.stick > STICK_HYS:   # ヒス分より大きい場合は正回転
            self.nmtgtnf = int(self.stick * KSTICKTONM_FWD)      # 中心値処理したスティック値⇛目標モータ回転数
            self.nmrotdir = NMROT_FWD

        # スティックが中心値付近
        elif self.stick >= -1*STICK_HYS and self.stick <= STICK_HYS:
            self.nmtgtnf = 0        # ヒス内の場合は0処理
            self.nmrotdir = NMROT_FWD

        # スティックが後進方向
        else:
            self.nmtgtnf = int(-1 * self.stick * KSTICKTONM_REV)
            self.nmrotdir = NMROT_REV

    # def nmtgtbscal(self):