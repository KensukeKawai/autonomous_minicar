# General Import
import time
from typing import Set

# My Function import
import peripheralfunc as pf

# Variable
global nmtgtm1, nmtgtm2
cnt_pwm_off = 0
flag_pwm_off = 1

# Variable init

# *****Parameter Set ***** #
# PWM Parameter
F_CARRIER = 300
DUTY_MID_FR = 45
DUTY_MID_RR = 45
DDUTY_FR = 3
DDUTY_RR = 3
DUTY_MAX = 100

CNT_PWM_OFF = 2

ID_MODE_STOP = 0
ID_MODE_NORMAL = 1
ID_MODE_MTORIGIN = 2

DUTY_MAX_FR = DUTY_MID_FR + DDUTY_FR
DUTY_MIN_FR = DUTY_MID_FR - DDUTY_FR
KSLOPE_DUTY_FR = (DUTY_MAX_FR-DUTY_MIN_FR)/255
DUTY_MAX_RR = DUTY_MID_RR + DDUTY_RR
DUTY_MIN_RR = DUTY_MID_RR - DDUTY_RR
KSLOPE_DUTY_RR = (DUTY_MAX_RR-DUTY_MIN_RR)/255

uart = pf.UART_set(115200)
spi0, spi1 = pf.SPI_set(1000000)
pwm1, pwm2 = pf.PWM_set(F_CARRIER, F_CARRIER)

try:
    rec_ly = rec_rx = 0
    pwm1.start(DUTY_MID_FR)
    pwm2.start(DUTY_MID_RR)

    while True:
        rec_first = int(pf.BytesToHex(uart.read()),16)

        while rec_first == 255:
            print("Pushed 'Start Button' of PS3 Controller")
            time.sleep(1)
            uart.write(255)

            while True:
                # Get Controller Value
                esp32rec_ly = int(pf.BytesToHex(uart.read()),16)
                esp32rec_rx = int(pf.BytesToHex(uart.read()),16)
                esp32rec_id = int(pf.BytesToHex(uart.read()),16)

                # SPI PWM set
                # NORMAL
                if esp32rec_id == ID_MODE_NORMAL:
                    nmtgtm1 = 0
                    nmtgtm2 = 0
                    duty_fr_str = esp32rec_rx*KSLOPE_DUTY_FR+DUTY_MIN_FR
                    duty_rr_str = esp32rec_rx*KSLOPE_DUTY_RR+DUTY_MIN_RR
                    id_send_cntrlr = ID_MODE_NORMAL
                    if flag_pwm_off == 0:
                        pwm1.ChangeDutyCycle(duty_fr_str)
                        pwm2.ChangeDutyCycle(duty_rr_str)
                    else:
                        pwm1.start(duty_fr_str)
                        pwm2.start(duty_rr_str)
                        flag_pwm_off = 0
                        cnt_pwm_off = 0

                # MTORIGIN
                elif esp32rec_id == ID_MODE_MTORIGIN:
                    nmtgtm1 = 0
                    nmtgtm2 = 0
                    duty_fr_str = DUTY_MAX
                    duty_rr_str = DUTY_MAX
                    id_send_cntrlr = ID_MODE_MTORIGIN
                    pwm1.ChangeDutyCycle(duty_fr_str)
                    pwm2.ChangeDutyCycle(duty_rr_str)

                # STOP
                else:
                    nmtgtm1 = 0
                    nmtgtm2 = 0
                    duty_fr_str = DUTY_MAX
                    duty_rr_str = DUTY_MAX
                    id_send_cntrlr = ID_MODE_STOP
                    pwm1.ChangeDutyCycle(duty_fr_str)
                    pwm2.ChangeDutyCycle(duty_rr_str)
                    if cnt_pwm_off >= CNT_PWM_OFF:
                        pwm1.stop()
                        pwm2.stop()
                        flag_pwm_off = 1
                    else:
                        cnt_pwm_off += 1


                # SPI Send
                rec_spi0 = spi0.xfer2([rec_ly,rec_ly,rec_ly,rec_ly])
                rec_spi1 = spi1.xfer2([rec_rx,rec_rx,rec_rx,rec_rx])

                
                # print("Ly:{:03} , Rx:{:03}, ID:{}, CS0: {}, CS1:{}".format(esp32rec_ly,esp32rec_rx,esp32rec_id,rec_spi0,rec_spi1))
                # print("nmtgtm1:{:03},nmtgtm2:{:03},DUTY_Fr:{:f},DUTY_Rr{:f},ID:{}".format(nmtgtm1,nmtgtm2,duty_fr_str,duty_rr_str,id_send_cntrlr))
                print(flag_pwm_off)

except KeyboardInterrupt:
    pwm1.stop()
    pwm2.stop()
    uart.close()
    spi0.close()
    spi1.close()