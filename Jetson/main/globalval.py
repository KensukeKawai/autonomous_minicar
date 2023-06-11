
# 制御周期設定
T_MAIN_TGT = 0.1                    # 狙いの定周期時間[s]
T_OFFSET = T_MAIN_TGT * 6           # 実周期を狙いの周期になるように調整する時間
T_MAIN = T_MAIN_TGT - T_OFFSET/1000 # 調整した定周期時間

# ID設定
ID_MODE_STP = 1
ID_MODE_RUN = 2
ID_MODE_ORG = 3
ID_MODE_RUNTOSTP = 4
ID_MODE_MAX = 4

# PIN設定
PIN_SERVO_SUPPLY = 13               # サーボ電源のON/OFF用GPIO
PIN_PWMOUT_FRONT = 32
PIN_PWMOUT_REAR = 33