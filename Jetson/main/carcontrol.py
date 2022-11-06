# General Import
import time

# My Module import
import servopwm as pwm
import communication as cm


# Variable
global nmtgtm1, nmtgtm2
cnt_pwm_off = 0
flag_pwm_off = 1

# Const
ID_MODE_STOP = 0
ID_MODE_NORMAL = 1
ID_MODE_MTORIGIN = 2
CNT_PWM_OFF = 2

# Instance
uart = cm.UART()
# spi1 = cm.SPI()
# spi2 = cm.SPI()
spi0,spi1 = cm.SPI_set(100000)              # 基本、設定できる最低の100kHzにする。最大250kHzくらい。
# spi1 = cm.SPI_set(1000000)
# pwm1, pwm2 = pf.PWM_set(pf.F_CARRIER, pf.F_CARRIER)
# pwm1.start(pf.DUTY_MID_FR)
# pwm2.start(pf.DUTY_MID_RR)

while True:
    # rec_first = uart.single_read()
    rec_first = 255

    while rec_first == 255:
        print("Pushed 'Start Button' of PS3 Controller")
        time.sleep(1)
        uart.write(255)

        while True:

            # Get Controller Value
            esp32rec_ly,esp32rec_rx,esp32rec_id = uart.read()


            # SPI PWM set
            # NORMAL
            if esp32rec_id == ID_MODE_NORMAL:
            #     nmtgtm1 = 0
            # #     nmtgtm2 = 0
            #     duty_fr_str = esp32rec_rx*pwm.KSLOPE_DUTY_FR+pwm.DUTY_MIN_FR
            #     duty_rr_str = esp32rec_rx*pwm.KSLOPE_DUTY_RR+pwm.DUTY_MIN_RR
                id_send_cntrlr = ID_MODE_NORMAL
                send_data = 255
            #     if flag_pwm_off == 0:
            #         pwm1.ChangeDutyCycle(duty_fr_str)
            #         pwm2.ChangeDutyCycle(duty_rr_str)
            #     else:
            #         pwm1.start(duty_fr_str)
            #         pwm2.start(duty_rr_str)
            #         flag_pwm_off = 0
            #         cnt_pwm_off = 0

            # MTORIGIN
            elif esp32rec_id == ID_MODE_MTORIGIN:
            #     nmtgtm1 = 0
            # #     nmtgtm2 = 0
            #     duty_fr_str = pwm.DUTY_MAX
            #     duty_rr_str = pwm.DUTY_MAX
                id_send_cntrlr = ID_MODE_MTORIGIN
                send_data = 0x20
            #     pwm1.ChangeDutyCycle(duty_fr_str)
            #     pwm2.ChangeDutyCycle(duty_rr_str)

            # STOP
            else:
            #     nmtgtm1 = 0
            #     nmtgtm2 = 0
                # duty_fr_str = pwm.DUTY_MAX
                # duty_rr_str = pwm.DUTY_MAX
                id_send_cntrlr = ID_MODE_STOP
                send_data = 0
            #     pwm1.ChangeDutyCycle(duty_fr_str)
            #     pwm2.ChangeDutyCycle(duty_rr_str)
            #     if cnt_pwm_off >= CNT_PWM_OFF:
            #         pwm1.stop()
            #         pwm2.stop()
            #         flag_pwm_off = 1
            #     else:
            #         cnt_pwm_off += 1


            # # SPI Send
            # send_data = 0x20
            rec_spi0 = spi0.xfer2([esp32rec_ly,esp32rec_ly,esp32rec_rx,esp32rec_rx])           # MSB First
            # rec_spi1 = spi1.xfer2([esp32rec_ly,esp32rec_ly,esp32rec_rx,esp32rec_rx])
            # print(rec_spi0,rec_spi1)
            print(rec_spi0)
            # print("nmtgtm1:{:03},nmtgtm2:{:03},DUTY_Fr:{:f},DUTY_Rr{:f},ID:{}".format(nmtgtm1,nmtgtm2,duty_fr_str,duty_rr_str,id_send_cntrlr))
            # print(flag_pwm_off)

# except KeyboardInterrupt:
    # pwm1.stop()
    # pwm2.stop()
    # uart.close()
    # spi0.close()