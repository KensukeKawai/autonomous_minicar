
import globalval as g

# Const
STICK_CENTER = 127                          # スティックの受信中心値
STICK_HYS = 20                               # スティックの中心ヒス
STICK_FWD = 128 - STICK_HYS                 # ヒス処理した正方向のスティック値
STICK_REV = 127 - STICK_HYS                 # ヒス処理した負方向のスティック値

# スティックデータをヒス処理して正規化
def sticknorm(ly, rx):
    stick_ly = ly - STICK_CENTER
    stick_rx = rx - STICK_CENTER

    # 左スティック
    # スティックが前進方向
    if stick_ly > STICK_HYS:      # ヒス分より大きい場合は正回転
        sticknorm_ly = (stick_ly - STICK_HYS) / STICK_FWD
    # スティックが後進方向
    elif stick_ly < -STICK_HYS:
        sticknorm_ly = (stick_ly + STICK_HYS) / STICK_REV
    # スティックが中心値付近
    else:
        sticknorm_ly = 0
        
    # 右スティック
    # スティックが前進方向
    if stick_rx > STICK_HYS:      # ヒス分より大きい場合は正回転
        sticknorm_rx = (stick_rx - STICK_HYS) / STICK_FWD
    # スティックが後進方向
    elif stick_rx < -STICK_HYS:
        sticknorm_rx = (stick_rx + STICK_HYS) / STICK_REV
    # スティックが中心値付近
    else:
        sticknorm_rx = 0
        
    return -sticknorm_ly, sticknorm_rx

def idcal(id_handcon):
    if g.id_handcon_z == g.ID_MODE_ORG and id_handcon == g.ID_MODE_ORG:
        id_handconmed = g.ID_MODE_STP
    else:
        id_handconmed = id_handcon
    # 前回値ラッチ
    g.id_handcon_z = id_handcon

    return id_handconmed