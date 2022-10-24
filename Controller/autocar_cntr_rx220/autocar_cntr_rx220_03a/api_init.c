
#include "common.h"

//�g�p�n�[�h�E�F�A�ݒ�֐�
void vdg_HW_init()
{
	/*****************************************/
	// ICLK = PCLKD = 32MHs,		 //
	// PCLKB = FCLK = BCLK = 32MHz		 //
	/*****************************************/
	
	/***�v���e�N�g����***/
   	SYSTEM.PRCR.WORD = 0xA503;					//�N���b�N������H�A����d�͒ጸ�@�\�֘A���W�X�^�ւ̏����݋���
	/***�v���e�N�g����***/

	/***�T�u�N���b�N��~�ݒ�***/
	SYSTEM.SOSCCR.BYTE = 0x01;					//�T�u�N���b�N���U���~
	RTC.RCR3.BIT.RTCEN = 0;						//�T�u�N���b�N���U���~
	/***�T�u�N���b�N��~�ݒ�***/
	
	/***HOCO�N���b�N�̔��U�ݒ�***/
  	SYSTEM.HOCOCR2.BYTE = 0x00;					//�����I���`�b�v�I�V���[�^HOCR�̓�����g��=32MHz
	SYSTEM.HOCOWTCR2.BYTE = 0x14;				//HOCO�N���b�N�����܂ł̑ҋ@���Ԑݒ�B180�T�C�N��(��5.625us)�҂��B
	SYSTEM.HOCOCR.BYTE = 0x00;					//HOCO����B��������HOCOWTCR2�Őݒ肵�����ԑ҂��Ȃ���΂����Ȃ�
	vdg_wait_nop(100);							//9216�T�C�N�����҂�
	/***HOCO�N���b�N�̔��U�ݒ�***/
	
	/***�����N���b�N������ݒ�***/
	SYSTEM.SCKCR.LONG = 0x0A;					//���Ѹۯ�(ICLK),����Ӽޭ�ٸۯ�B/D(PCLKB/D),�O���޽�ۯ�(BCLK),FlashIF�ۯ�)FCLK)��1����
	//while (0x0A != SYSTEM.SCKCR.LONG){}		//SCKCR�̏������݊������Ă邩�m�F�҂�
	/***�����N���b�N������ݒ�***/
	
	/***�V�X�e���N���b�N�؂�ւ�***/
	SYSTEM.SCKCR3.BIT.CKSEL = 1;				//HOCO�I��
	//while (1 != SYSTEM.SCKCR3.BIT.CKSEL){}	//�������܂�Ă邩�m�F�҂�
	/***�V�X�e���N���b�N�؂�ւ�***/
	
	/***����d�͐��䃂�[�h�ݒ�***/
	SYSTEM.OPCCR.BIT.OPCM = 2;					//�������샂�[�h1A
	while (SYSTEM.OPCCR.BIT.OPCM != 2){}		//�J�ڊ����҂�
	/***����d�͐��䃂�[�h�ݒ�***/
	
	/***���W���[���X�g�b�v�����ݒ�***/
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;				//MTU���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;				//S12AD���W���[���X�g�b�v��ԉ���
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;				//SCI1���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRB.BIT.MSTPB17 = 0;				//RSPI0���W���[���X�g�b�v��ԉ���
	//CMPA�̓A�i���O���W���[���̂��ߐݒ�s�v
	/***���W���[���X�g�b�v�����ݒ�***/
	
	/***�v���e�N�g�ݒ�***/
	SYSTEM.PRCR.WORD = 0xA500;					//PRCR���W�X�^�ɁA�e���W�X�^�ւ̏����݋֎~�ݒ�
	/***�v���e�N�g�ݒ�***/
}

//���o�͐ݒ�֐�
void vdg_IO_init()
{	
	PORT0.PDR.BYTE = 0xFF;
	PORT1.PDR.BYTE = 0x9F;		//P14(MTIOC3A)�o�́AP15(IRQ5)���́AP16(MOSIA)���́AP17(MISOA)�o��
	PORT2.PDR.BYTE = 0xFF;		//P26(TXD1)�o��
	PORT3.PDR.BYTE = 0xFA;		//P30(RXD1)���́AP32(IRQ2)����
	PORT4.PDR.BYTE = 0xFC;		//P40(AN000)���́AP41(AN001)����
	PORT5.PDR.BYTE = 0xFF;
	PORTA.PDR.BYTE = 0xA5;		//PA0(MTIOC4A)�o�́APA1(MTCLKC)���́APA3(IRQ6)���́APA4(MTCLKA)���́APA6(MTCLKB)����
	PORTB.PDR.BYTE = 0xFE;		//PB0(RSPCKA)���́APB1(MTIOC0C)�o�́APB3(MTIOC0A)�o��
	PORTC.PDR.BYTE = 0x4F;		//PC4(SSLA0)���́APC5(MTCLKD)���́APC6(MTIOC3C)�o�́APC7(BOOT�p)����
	PORTE.PDR.BYTE = 0xFB;		//PE1(MTIOC4C)�o�́APE2(IRQ7)����
	PORTH.PDR.BYTE = 0xF9;		//PH1(IRQ0)���́APH2(IRQ1)����
	
	PORT2.PODR.BIT.B6 = 1;		//TXD1��HIGH
	PORT3.PCR.BIT.B0 = 1;		//RXD1�̓v���A�b�v
}

void vdg_MPC_init()
{
	/***MPC�ݒ�̂��߂ɏ������݃v���e�N�g���W�X�^PWPR�ݒ�***/
	MPC.PWPR.BIT.B0WI = 0;		//PFSWE�r�b�g��1���������ނ��߂�NOWI��0��������
	MPC.PWPR.BIT.PFSWE = 1;		//P**PFS�ɏ������ނ��߂�PFSWE��1����������
	/***MPC�ݒ�̂��߂ɏ������݃v���e�N�g���W�X�^PWPR�ݒ�***/
	
	/***API�ɍ��킹��MPC�Ńs������t��***/
	MPC.PH1PFS.BIT.ISEL = 1;	//PH1(CN2-16)��IRQ0
	MPC.PH2PFS.BIT.ISEL = 1;	//PH2(CN2-17)��IRQ1
	MPC.P32PFS.BIT.ISEL = 1;	//P32(CN2-27)��IRQ2
	MPC.P15PFS.BIT.ISEL = 1;	//P15(CN2-20)��IRQ5
	MPC.PA3PFS.BIT.ISEL = 1;	//PA3(CN1-15)��IRQ6
	MPC.PE2PFS.BIT.ISEL = 1;	//PE2(CN1-09)��IRQ7
	
	MPC.PB3PFS.BIT.PSEL = 1;	//PB3(CN2-03)��MTIOC0A
	MPC.PB1PFS.BIT.PSEL = 1;	//PB1(CN2-02)��MTIOC0C
	MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)��MTIOC3A
	MPC.PC6PFS.BIT.PSEL = 1;	//PC6(CN2-11)��MTIOC3C
	MPC.PA0PFS.BIT.PSEL = 1;	//PA0(CN1-13)��MTIOC4A
	MPC.PE1PFS.BIT.PSEL = 1;	//PE1(CN1-08)��MTIOC4C

	MPC.PA4PFS.BIT.PSEL = 2;	//PA4(CN1-16)��MTCLKA
	MPC.PA6PFS.BIT.PSEL = 2;	//PA6(CN1-17)��MTCLKB
	MPC.PA1PFS.BIT.PSEL = 2;	//PA1(CN1-14)��MTCLKC
	MPC.PC5PFS.BIT.PSEL = 2;	//PC5(CN2-10)��MTCLKD

	MPC.P30PFS.BIT.PSEL = 10;	//P30(CN2-25)��RXD1
	MPC.P26PFS.BIT.PSEL = 10;	//P26(CN2-23)��TXD1

	MPC.PB0PFS.BIT.PSEL = 13;	//PB0(CN2-01)��RSPCKA
	MPC.P16PFS.BIT.PSEL = 13;	//P16(CN2-21)��MOSIA
	MPC.P17PFS.BIT.PSEL = 13;	//P17(CN2-22)��MISOA
	MPC.PC4PFS.BIT.PSEL = 13;	//PC4(CN2-09)��SSLA0

	MPC.P40PFS.BIT.ASEL = 1;	//P40(CN1-01)��AN000
	MPC.P41PFS.BIT.ASEL = 1;	//P41(CN1-00)��AN001

	//MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)��MTIOC3A�g�O���o��
	/***API�ɍ��킹��MPC�Ńs������t��***/
	
	//P**PFS���W�X�^�ւ̏������݋֎~
	MPC.PWPR.BIT.PFSWE = 0;		

	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)�BP**PFS�̂����hPSEL�h�ɑ΂��Ă̂�PMR���`***/
	PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-03)��MTIOC0A
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-02)��MTIOC0C
	PORT1.PMR.BIT.B4 = 1;		//P14(CN2-19)��MTIOC3A
	PORTC.PMR.BIT.B6 = 1;		//PC6(CN2-11)��MTIOC3C
	PORTA.PMR.BIT.B0 = 1;		//PA0(CN1-13)��MTIOC4A
	PORTE.PMR.BIT.B1 = 1;		//PE1(CN1-08)��MTIOC4C

	PORTA.PMR.BIT.B4 = 1;		//PA4(CN1-16)��MTCLKA
	PORTA.PMR.BIT.B6 = 1;		//PA6(CN1-17)��MTCLKB
	PORTA.PMR.BIT.B1 = 1;		//PA1(CN1-14)��MTCLKC
	PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTCLKD

	PORT3.PMR.BIT.B0 = 1;		//P30(CN2-25)��RXD1
	PORT2.PMR.BIT.B6 = 1;		//P26(CN2-23)��TXD1

	PORTB.PMR.BIT.B0 = 1;		//PB0(CN2-01)��RSPCKA
	PORT1.PMR.BIT.B6 = 1;		//P16(CN2-21)��MOSIA
	PORT1.PMR.BIT.B7 = 1;		//P17(CN2-22)��MISOA
	PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-09)��SSLA0
	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)***/
}

void vdg_MTU2a_init()
{
	/**************PWM1���[�h�ݒ�*****************/
	MTU.TSYR.BIT.SYNC0 = 1;		//MTU0.TCNT�͓�������
	MTU.TSYR.BIT.SYNC3 = 1;		//MTU3.TCNT�͓�������
	MTU.TSYR.BIT.SYNC4 = 1;		//MTU4.TCNT�͓�������

	MTU0.TCR.BYTE = 0x20;		//00100000�FPCLK/1,1����,TGRA�R���y�A�}�b�`��TCNT�N���A
	MTU3.TCR.BYTE = 0x60;		//01100000�FPCLK/1,1����,MTU0�̃J�E���^�N���A��TCNT�N���A
	MTU4.TCR.BYTE = 0x60;		//01100000�FPCLK/1,1����,MTU0�̃J�E���^�N���A��TCNT�N���A

	// MTU0.TCR.BYTE = 0x20;		//00100000�FPCLK/1,1����,TGRA�R���y�A�}�b�`��TCNT�N���A
	// MTU3.TCR.BYTE = 0x20;		//01100000�FPCLK/1,1����,MTU0�̃J�E���^�N���A��TCNT�N���A
	// MTU4.TCR.BYTE = 0x20;		//01100000�FPCLK/1,1����,MTU0�̃J�E���^�N���A��TCNT�N���A
	
	MTU0.TMDR.BIT.MD = 2;		//0010�FPWM1���[�h
	MTU3.TMDR.BIT.MD = 2;		//0010�FPWM1���[�h
	MTU4.TMDR.BIT.MD = 2;		//0010�FPWM1���[�h

	MTU0.TIORH.BYTE = 0x21;		//MTIOC0A����Lo�}�b�`Hi,MTIOC0B�o�͋֎~
	MTU0.TIORL.BYTE = 0x21;		//MTIOC0C����Lo�}�b�`Hi,MTIOC0D�o�͋֎~
	MTU3.TIORH.BYTE = 0x21;		//MTIOC3A����Lo�}�b�`Hi,MTIOC3B�o�͋֎~
	MTU3.TIORL.BYTE = 0x21;		//MTIOC3C����Lo�}�b�`Hi,MTIOC3D�o�͋֎~
	MTU4.TIORH.BYTE = 0x21;		//MTIOC4A����Lo�}�b�`Hi,MTIOC4B�o�͋֎~
	MTU4.TIORL.BYTE = 0x21;		//MTIOC4C����Lo�}�b�`Hi,MTIOC4D�o�͋֎~

	MTU0.TIER.BIT.TGIEA = 1;	//TGIA0(MTU0.TGRA)�̃R���y�A�}�b�`�����ݗv������
	MTU0.TIER.BIT.TTGE = 1;		//MTU0.TGRA�R���y�A�}�b�`��ADC�J�n�v������

	MTU0.TGRA = CNT_CARRIER;	//�L�����A�J�E���g�ݒ�
	MTU3.TGRA = CNT_CARRIER;	//�L�����A�J�E���g�ݒ�
	MTU4.TGRA = CNT_CARRIER;	//�L�����A�J�E���g�ݒ�
	MTU0.TGRC = CNT_CARRIER;	//�L�����A�J�E���g�ݒ�
	MTU3.TGRC = CNT_CARRIER;	//�L�����A�J�E���g�ݒ�
	MTU4.TGRC = CNT_CARRIER;	//�L�����A�J�E���g�ݒ�

	MTU0.TGRB = CNT_CARRIER + 10;	//�����̓L�����A�J�E���g�����傫����Duty0%
	MTU3.TGRB = CNT_CARRIER + 10;	//�����̓L�����A�J�E���g�����傫����Duty0%
	MTU4.TGRB = CNT_CARRIER + 10;	//�����̓L�����A�J�E���g�����傫����Duty0%
	MTU0.TGRD = CNT_CARRIER + 10;	//�����̓L�����A�J�E���g�����傫����Duty0%
	MTU3.TGRD = CNT_CARRIER + 10;	//�����̓L�����A�J�E���g�����傫����Duty0%
	MTU4.TGRD = CNT_CARRIER + 10;	//�����̓L�����A�J�E���g�����傫����Duty0%

	//�����݋��ݒ�
	ICU.IER[0x0E].BIT.IEN2 = 1;		//TGIA0�����݋���

//	MTU.TSTR.BYTE = 0xC1;		//MTU0.TCNT,MTU3.TCNT,MTU4.TCNT�̃J�E���g����J�n
	/**************PWM1���[�h�ݒ�*****************/

	/**************�ʑ��v�����[�h�ݒ�*****************/
	MTU1.TMDR.BIT.MD = 4;		//�ʑ��v�����[�h1
	MTU2.TMDR.BIT.MD = 4;		//�ʑ��v�����[�h1
	MTU1.TIER.BIT.TCIEV = 1;	//MTU1�ʑ��v���I�[�o�[�t���[�����݋���
	MTU1.TIER.BIT.TCIEU = 1;	//MTU1�ʑ��v���A���_�[�t���[�����݋���
	MTU2.TIER.BIT.TCIEV = 1;	//MTU2�ʑ��v���I�[�o�[�t���[�����݋���
	MTU2.TIER.BIT.TCIEU = 1;	//MTU2�ʑ��v���A���_�[�t���[�����݋���
	//MTU.TSTR.BYTE = 0X06;		//MTU1,2�̈ʑ��v������J�n
	/**************�ʑ��v�����[�h�ݒ�*****************/

	//MTU.TSTR.BYTE = 0xC7;		//MTU0,1,2,3,4��TCNT�J�E���g�J�n
}

void vdg_S12AD_init()
{
	S12AD.ADCSR.BIT.ADCS = 1;		//�O���[�v�X�L�������[�h
	S12AD.ADCSR.BIT.ADIE = 1;		//�X�L�����I�������݋��������݂Ŋ����t���OON�ɂ��邾��
	S12AD.ADANSA.WORD = 0x03;		//AN000,AN001��ϊ��Ώۂɂ���(�O���[�vA)
	S12AD.ADCER.BIT.ACE = 1;		//�f�[�^�ǂݏo���ɂ�鎩���N���A������
	S12AD.ADSTRGR.BIT.TRSA = 1;		//�v���FTRG0AN�BMTU0.TGRA�R���y�A�}�b�`��ADC�J�n�g���K�ݒ�
	S12AD.ADCSR.BIT.TRGE = 1;		//�g���K(MTU0.TCNT�R���y�A�}�b�`)�ɂ��ADC�J�n����

	ICU.IER[0x0C].BIT.IEN6 = 1;		//S12AD��S12ADI0�����ݗv��������
}

void vdg_SCI_init()
{
	/***PC�Ƃ̒ʐM�p***/
	SCI1.SCR.BYTE = 0x00;			//������
	SCI1.SCR.BIT.CKE = 0;			//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	//SCI1.SIMR1.BIT.IICM = 0;		//�V���A���C���^�[�t�F�[�X���[�h
	SCI1.SMR.BYTE = 0x00;			//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = 1;			//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI1.SEMR.BIT.NFEN = 1;			//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI1.SNFR.BIT.NFCS = 4;			//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI1.BRR = 9;					//3:500000bps,9:200000bps
	SCI1.SCR.BIT.RE = 1;			//��M���������
	SCI1.SCR.BIT.TE = 1;			//���M���������
	SCI1.SCR.BIT.RIE = 1;			//��M�y�уG���[�����ݗv��������
	SCI1.SCR.BIT.TIE = 1;			//���M�����ݗv��������
	
	ICU.IER[0x1B].BIT.IEN4 = 1;		//���M�����݋���
	ICU.IER[0x1B].BIT.IEN3 = 1;		//��M�����݋���
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//���M���������݋���
}

void vdg_SPI_init()
{
	//SSLP���W�X�^�ݒ�
	RSPI0.SSLP.BYTE = 0x00;			//SSL0~3�̃`�b�v�Z���N�g���ɐ����u�A�N�e�B�uLow�v
	//SPDCR���W�X�^�ݒ�
	RSPI0.SPDCR.BIT.SPFC = 1;		//�t���[����1�i�g�p����SPCMD*�̐����w�肷��j
	RSPI0.SPDCR.BIT.SPRDTD = 0;		//SPDR�̓ǂݏo����o�b�t�@����M�o�b�t�@�ɐݒ�
	RSPI0.SPDCR.BIT.SPLW = 1;		//SPDR���W�X�^�փ����O���[�h�A�N�Z�X
	//SPCR2���W�X�^�ݒ�
	RSPI0.SPCR2.BIT.SPPE = 0;		//���M�f�[�^�Ƀp���e�B�r�b�g�t�^���Ȃ��A��M�f�[�^�Ƀp���e�B�`�F�b�N���s��Ȃ�
	RSPI0.SPCR2.BIT.SPIIE =  0;		//�A�C�h�������ݗv���̔������֎~
	//SPCMD0���W�X�^�ݒ�
	RSPI0.SPCMD0.BIT.CPHA = 0;		//SPI���[�h0
	RSPI0.SPCMD0.BIT.CPOL = 0;		//�A�C�h������RSPCK��Low�ݒ�
	RSPI0.SPCMD0.BIT.SSLA = 0;		//�X���[�u���[�h�̂���0�ݒ�
	RSPI0.SPCMD0.BIT.SSLKP = 0;		//�X���[�u���[�h�̂���0�ݒ�
	RSPI0.SPCMD0.BIT.SPB = 2;		//�f�[�^����32bit
	RSPI0.SPCMD0.BIT.LSBF = 0;		//MSB�t�@�[�X�g
	
	//�����݃R���g���[���̐ݒ�
	ICU.IER[0x05].BIT.IEN5 = 1;		//��M�o�b�t�@�t�������݋���

	//SPCR���W�X�^�ݒ�
	RSPI0.SPCR.BYTE = 0xc0;
	//��L0xC0�̓���͈ȉ�
	// RSPI0.SPCR.BIT.SPMS = 0;		//SPI����
	// RSPI0.SPCR.BIT.TXMD = 0;		//�S��d�������V���A���ʐM
	// RSPI0.SPCR.BIT.MODFEN = 0;		//���[�h�t�H���g�G���[���o���֎~
	// RSPI0.SPCR.BIT.MSTR = 0;		//�X���[�u���[�h
	// RSPI0.SPCR.BIT.SPEIE = 0;		//RSPI�G���[�����ݗv���̔������֎~
	// RSPI0.SPCR.BIT.SPTIE = 0;		//���M�����ݗv���̔������֎~
	// RSPI0.SPCR.BIT.SPRIE = 1;		//��M�����ݗv���̔���������
	// RSPI0.SPCR.BIT.SPE = 1;			//RSPI�@�\��L��

	//�����O���[�h�A�N�Z�X����ꍇ�͈ȉ��ōs��
	//RSPI0.SPDR.LONG
}

void vdg_IRQ_init()
{
	//�g�p����ԍ���0,1,2,5,6,7
	//�O�������݂̓C���^���v�^�������̓z�[���Z���T�̂ǂ��炩
	//��������A���G�b�W�g���K���o
	ICU.IER[0x08].BYTE = 0x00;		//�SIRQ�������ݗv���֎~
	ICU.IRQFLTE0.BYTE = 0x00;		//�SIRQ���f�B�W�^���t�B���^����
	//�f�B�W�^���t�B���^�͈ȉ��ݒ�N���b�N3�񕪘A����v�Ō��o
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 1;	//�T���v�����O�N���b�NPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL1 = 1;	//�T���v�����O�N���b�NPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL2 = 1;	//�T���v�����O�N���b�NPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL5 = 1;	//�T���v�����O�N���b�NPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL6 = 1;	//�T���v�����O�N���b�NPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL7 = 1;	//�T���v�����O�N���b�NPCLK/8
	//�S�ė��G�b�W���o
	ICU.IRQCR[0].BIT.IRQMD = 3;		//���G�b�W���o
	ICU.IRQCR[1].BIT.IRQMD = 3;		//���G�b�W���o
	ICU.IRQCR[2].BIT.IRQMD = 3;		//���G�b�W���o
	ICU.IRQCR[5].BIT.IRQMD = 3;		//���G�b�W���o
	ICU.IRQCR[6].BIT.IRQMD = 3;		//���G�b�W���o
	ICU.IRQCR[7].BIT.IRQMD = 3;		//���G�b�W���o
	//�x�N�^�ԍ����ƂɊ����ݗv���t���O0��������
	ICU.IR[64].BIT.IR = 0;			//IRQ0�����ݗv���t���O"0"�i[]���̓x�N�^�ԍ��j
	ICU.IR[65].BIT.IR = 0;			//IRQ1�����ݗv���t���O"0"�i[]���̓x�N�^�ԍ��j
	ICU.IR[66].BIT.IR = 0;			//IRQ2�����ݗv���t���O"0"�i[]���̓x�N�^�ԍ��j
	ICU.IR[69].BIT.IR = 0;			//IRQ5�����ݗv���t���O"0"�i[]���̓x�N�^�ԍ��j
	ICU.IR[70].BIT.IR = 0;			//IRQ6�����ݗv���t���O"0"�i[]���̓x�N�^�ԍ��j
	ICU.IR[71].BIT.IR = 0;			//IRQ7�����ݗv���t���O"0"�i[]���̓x�N�^�ԍ��j
	//�f�B�W�^���t�B���^�L��
	ICU.IRQFLTE0.BYTE = 0xE7;		//�f�B�W�^���t�B���^�L��

	//IRQ�����ݗv�����B�O�������ݎg�p����ꍇ�͈ȉ��s��L����
	// ICU.IER[0x08].BYTE = 0xE7;		//IRQ�����ݗv������
}

void vdg_IPR_init()
{
	ICU.IPR[114].BIT.IPR = 15;		//TGIA0(MTU0.TGRA)�̃R���y�A�}�b�`�����ݗD��x
	ICU.IPR[102].BIT.IPR = 13;		//S12ADI0�O�������ݗD��x
	// ICU.IPR[218].BIT.IPR = 12;		//SCI1�����ݗD��x�i����M���ʁj
	ICU.IPR[44].BIT.IPR = 0;		//SPI��M�����ݗD��x
	// ICU.IPR[64].BIT.IPR = 1;		//IRQ0�����ݗD��x
	// ICU.IPR[65].BIT.IPR = 1;		//IRQ1�����ݗD��x
	// ICU.IPR[66].BIT.IPR = 1;		//IRQ2�����ݗD��x
	// ICU.IPR[69].BIT.IPR = 1;		//IRQ5�����ݗD��x
	// ICU.IPR[70].BIT.IPR = 1;		//IRQ6�����ݗD��x
	// ICU.IPR[71].BIT.IPR = 1;		//IRQ7�����ݗD��x
}