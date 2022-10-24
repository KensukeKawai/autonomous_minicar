
#ifndef _GLOBAL_DATA_H_
#define _GLOBAL_DATA_H_

/***�n�[�h����***/
#define ENC_NTEETH 54		    	//���[�^���G���R�[�_����
#define POLE_PAIRS 7			    //�ɑΐ�
#define V_PW 18		    		    //�d���d��
#define NM_MAX 6000                 //���[�^�ő��]��

/***�ėp***/
#define PI 3.14159265

/***CLK***/
#define PCLK1  32000000
#define PCLK1024 31250	            //PCLK1/1024

/***MTU2a***/
#define HZ_CARRIER 5000            //�L�����A���g��
#define T_CARRIER (1/(float)(HZ_CARRIER))
#define CNT_CARRIER ((unsigned short)(6400))           //�L�����A�J�E���g���B32MHz/5kHz

/***�|�[�g��`***/
#define PORT_M1_UH PORTC.PMR.BIT.B6
#define PORT_M1_VH PORTB.PMR.BIT.B3
#define PORT_M1_WH PORTB.PMR.BIT.B1
#define PORT_M1_UL PORTH.PODR.BIT.B0
#define PORT_M1_VL PORT5.PODR.BIT.B5
#define PORT_M1_WL PORT5.PODR.BIT.B4

#define PORT_M2_UH PORTE.PMR.BIT.B1
#define PORT_M2_VH PORTA.PMR.BIT.B0
#define PORT_M2_WH PORT1.PMR.BIT.B4
#define PORT_M2_UL PORTE.PODR.BIT.B3
#define PORT_M2_VL PORTE.PODR.BIT.B4
#define PORT_M2_WL PORTE.PODR.BIT.B5

#define PORT_GENERAL_P1 PORT4.PODR.BIT.B3
#define PORT_GENERAL_P2 PORT4.PODR.BIT.B2
#define PORT_GENERAL_P3 PORT4.PODR.BIT.B4

/***���W�X�^��`***/
#define MTU_M1_UH MTU3.TGRD
#define MTU_M1_VH MTU0.TGRB
#define MTU_M1_WH MTU0.TGRD

#define MTU_M2_UH MTU4.TGRD
#define MTU_M2_VH MTU4.TGRB
#define MTU_M2_WH MTU3.TGRB

#define MTU_M1_ENCTCNT MTU2.TCNT
#define MTU_M2_ENCTCNT MTU1.TCNT


#endif
