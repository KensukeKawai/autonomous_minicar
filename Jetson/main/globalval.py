
# ID設定
ID_MODE_STOP = 0
ID_MODE_NORMAL = 1
ID_MODE_MTORIGIN = 2

T_MAIN_TGT = 0.1                    # 狙いの定周期時間[s]
T_OFFSET = T_MAIN_TGT * 6           # 実周期を狙いの周期になるように調整する時間
T_MAIN = T_MAIN_TGT - T_OFFSET/1000 # 調整した定周期時間
