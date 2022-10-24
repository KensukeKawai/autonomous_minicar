
#include "iodefine.h"
#include "vect.h"
#include <machine.h>
#include "global_data.h"
#include "api_init.h"

//�g�p�n�[�h�E�F�A�ݒ�֐�
void vdg_HW_init()
{
	/*****************************************/
	// ICLK = PCLKD = 32MHs,		 //
	// PCLKB = FCLK = BCLK = 32MHz		 //
	/*****************************************/
	
	unsigned int i;
	
	/***�v���e�N�g����***/
   	SYSTEM.PRCR.WORD = 0xA503;			//�N���b�N������H�A����d�͒ጸ�@�\�֘A���W�X�^�ւ̏����݋���
	/***�v���e�N�g����***/

	/***�T�u�N���b�N��~�ݒ�***/
	SYSTEM.SOSCCR.BYTE = 0x01;			//�T�u�N���b�N���U���~
	RTC.RCR3.BIT.RTCEN = 0;				//�T�u�N���b�N���U���~
	/***�T�u�N���b�N��~�ݒ�***/
	
	/***HOCO�N���b�N�̔��U�ݒ�***/
  	SYSTEM.HOCOCR2.BYTE = 0x00;			//�����I���`�b�v�I�V���[�^HOCR�̓�����g��=32MHz
    	SYSTEM.HOCOWTCR2.BYTE = 0x14;			//HOCO�N���b�N�����܂ł̑ҋ@���Ԑݒ�B180�T�C�N��(��5.625us)�҂��B
    	SYSTEM.HOCOCR.BYTE = 0x00;			//HOCO����B��������HOCOWTCR2�Őݒ肵�����ԑ҂��Ȃ���΂����Ȃ�
    	for(i=0;i<100;i++){ nop();}			//9216�T�C�N�����҂�
	/***HOCO�N���b�N�̔��U�ݒ�***/
	
	/***�����N���b�N������ݒ�***/
	SYSTEM.SCKCR.LONG = 0x0A;			//���Ѹۯ�(ICLK),����Ӽޭ�ٸۯ�B/D(PCLKB/D),�O���޽�ۯ�(BCLK),FlashIF�ۯ�)FCLK)��1����
	//while (0x0A != SYSTEM.SCKCR.LONG){}		//SCKCR�̏������݊������Ă邩�m�F�҂�
	/***�����N���b�N������ݒ�***/
	
	/***�V�X�e���N���b�N�؂�ւ�***/
	SYSTEM.SCKCR3.BIT.CKSEL = 1;			//HOCO�I��
	//while (1 != SYSTEM.SCKCR3.BIT.CKSEL){}	//�������܂�Ă邩�m�F�҂�
	/***�V�X�e���N���b�N�؂�ւ�***/
	
	/***����d�͐��䃂�[�h�ݒ�***/
	SYSTEM.OPCCR.BIT.OPCM = 2;			//�������샂�[�h1A
	while (SYSTEM.OPCCR.BIT.OPCM != 2){}		//�J�ڊ����҂�
	/***����d�͐��䃂�[�h�ݒ�***/
	
	/***���W���[���X�g�b�v�����ݒ�***/
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;			//MTU2a���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;			//S12AD���W���[���X�g�b�v��ԉ���
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;			//SCI1���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRB.BIT.MSTPB4 = 0;			//SCI12���W���[���X�g�b�v��ԉ���
	//CMPA�̓A�i���O���W���[���̂��ߐݒ�s�v
	/***���W���[���X�g�b�v�����ݒ�***/
	
	/***�v���e�N�g�ݒ�***/
	SYSTEM.PRCR.WORD = 0xA500;			//PRCR���W�X�^�ɁA�e���W�X�^�ւ̏����݋֎~�ݒ�
	/***�v���e�N�g�ݒ�***/
}

//���o�͐ݒ�֐�
void vdg_IO_init()
{	
	PORT0.PDR.BYTE = 0xFF;
	PORT1.PDR.BYTE = 0xFF;		//P14(MTIOC3A)�o��
	PORT2.PDR.BYTE = 0x7F;		//P26(TXD1)�o�́AP27(MTIOC2B)����
	PORT3.PDR.BYTE = 0xFE;		//P30(RXD1)����
	PORT4.PDR.BYTE = 0xFE;		//P40(AN000)����
	PORT5.PDR.BYTE = 0xFF;
	//PORTA.PDR.BYTE = 0xEF;		//PA4(IRQ5)�����
	PORTA.PDR.BYTE = 0xFF;
	PORTB.PDR.BYTE = 0xFF;		//PB1(MTIOC4C)�o�́APB3(MTIOC4A)�o��
	PORTC.PDR.BYTE = 0x7F;		//PC7�̓{�[�h����͐ݒ�ɂ��Ă���
	PORTE.PDR.BYTE = 0xFB;		//PE1(TXD12)�o�́APE2(EXD12)����
	PORTH.PDR.BYTE = 0xFF;
	
	PORT2.PODR.BIT.B6 = 1;		//TXD�[�q��HIGH
	PORTE.PODR.BIT.B1 = 1;		//TXD�[�q��HIGH
	
	PORT3.PCR.BIT.B0 = 1;		//RXD1�̓v���A�b�v
	PORTE.PCR.BIT.B2 = 1;		//RXD12�̓v���A�b�v
}

void vdg_MPC_init()
{
	/***MPC�ݒ�̂��߂ɏ������݃v���e�N�g���W�X�^PWPR�ݒ�***/
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	/***MPC�ݒ�̂��߂ɏ������݃v���e�N�g���W�X�^PWPR�ݒ�***/
	
	/***API�ɍ��킹��MPC�Ńs������t��***/
	MPC.P26PFS.BIT.PSEL = 10;	//P26(CN2-23)��TXD1
	MPC.P30PFS.BIT.PSEL = 10;	//P30(CN2-25)��RXD1
	MPC.P27PFS.BIT.PSEL = 1;	//P27(CN2-24)��MTIOC2B
	MPC.P40PFS.BIT.ASEL = 1;	//P40(CN1-1)��AN000
	MPC.PB1PFS.BIT.PSEL = 2;	//PB1(CN2-2)��MTIOC4C
	MPC.PB3PFS.BIT.PSEL = 2;	//PB3(CN2-3)��MTIOC4A
	MPC.PC2PFS.BIT.PSEL = 1;	//PC2(CN2-7)��MTIOC4B
	MPC.PC3PFS.BIT.PSEL = 1;	//PC3(CN2-8)��MTIOC4D
	MPC.PC4PFS.BIT.PSEL = 1;	//PC4(CN2-9)��MTIOC3D
	MPC.PC5PFS.BIT.PSEL = 1;	//PC5(CN2-10)��MTIOC3B
	MPC.PE1PFS.BIT.PSEL = 12;	//PE1(CN1-8)��TXD12
	MPC.PE2PFS.BIT.PSEL = 12;	//PE2(CN1-9)��RXD12
	
	MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)��MTIOC3A�g�O���o��
	//MPC.PA4PFS.BIT.ISEL = 1;	//PA4(CN1-16)��IRQ5
	/***API�ɍ��킹��MPC�Ńs������t��***/
	
	//P**PFS���W�X�^�ւ̏������݋֎~
	MPC.PWPR.BIT.PFSWE = 0;		

	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)�BP**PFS�̂����hPSEL�h�ɑ΂��Ă̂�PMR���`***/
	PORT2.PMR.BIT.B6 = 1;		//P26(CN2-23)��TXD1
	PORT3.PMR.BIT.B0 = 1;		//P30(CN2-25)��RXD1
	PORT2.PMR.BIT.B7 = 1;		//P27(CN2-24)��MTIOC2B
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
	PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
	PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
	PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
	PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
	PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
	PORT1.PMR.BIT.B4 = 1;		//P14(CN2-19)��MTIOC3A
	PORTE.PMR.BIT.B1 = 1;		//PE1(CN1-8)��TXD12
	PORTE.PMR.BIT.B2 = 1;		//PE2(CN1-9)��RXD12
	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)***/
}

void vdg_MTU2a_init()
{
	MTU.TSTR.BYTE = 0x00;		//TCNT�J�E���g���~
	MTU3.TCR.BYTE = 0x00;		//PCLK/1�A�����オ��G�b�W�ŃJ�E���g�ATCNT�N���A�֎~
	MTU4.TCR.BYTE = 0x00;		//PCLK/1�A�����オ��G�b�W�ŃJ�E���g�ATCNT�N���A�֎~
	
	/***TCNT�̏����ݒ�***/
	MTU3.TCNT = CNT_DT;		//�f�b�h�^�C���ݒ�
	MTU4.TCNT = 0;
	/***TCNT�̏����ݒ�***/
	
	/***�f���[�e�B��̏����ݒ�***/
	/*�R���y�A���W�X�^*/
	MTU3.TGRB = CNT_DT;
	MTU4.TGRA = CNT_DT;
	MTU4.TGRB = CNT_DT;
	/*�R���y�A���W�X�^*/
	/*�o�b�t�@���W�X�^*/
	MTU3.TGRD = CNT_DT;
	MTU4.TGRC = CNT_DT;
	MTU4.TGRD = CNT_DT;
	/*�o�b�t�@���W�X�^*/
	/***�f���[�e�B��̏����ݒ�***/
	
	/***�f�b�h�^�C���A�L�����A�����̐ݒ�***/
	MTU.TDDR = CNT_DT;
	MTU.TCDR = CNT_HALF_CARR;
	MTU.TCBR = CNT_HALF_CARR;
	MTU3.TGRA = CNT_HALF_CARR + CNT_DT;
	MTU3.TGRC =  CNT_HALF_CARR + CNT_DT;
	/***�f�b�h�^�C���A�L�����A�����̐ݒ�***/
	
	/***PWM�o�̓��x���ݒ�F���������[�T�C�h�A�t�����n�C�T�C�h***/
	MTU.TOCR1.BIT.OLSP = 1;		//�����o�͂�����LOW�A�N�e�B�uHIGH�AUP(HIGH)DOWN(LOW)
	MTU.TOCR1.BIT.OLSN = 1;		//�t���o�͂�����LOW�A�N�e�B�uHIGH�AUP(LOW)DOWN(HIGH)
	/***PWM�o�̓��x���ݒ�F���������[�T�C�h�A�t�����n�C�T�C�h***/
	
	MTU.TOCR1.BIT.PSYE =1;		//MTIOC3A�g�O���o��
	
	/***����PWM���[�h�ݒ�(MTU3�݂̂ɍs��)***/
	MTU3.TMDR.BIT.MD = 13;		//����PWM���[�h1�i�R�œ]���j
	MTU3.TMDR.BIT.BFA = 1;		//TGRA��TGRC���o�b�t�@����
	MTU3.TMDR.BIT.BFB = 1;		//TGRB��TGRD���o�b�t�@����
	/***����PWM���[�h�ݒ�***/
	
	//MTU4.TIER.BIT.TTGE2 = 1;	//MTU4.TCNT�A���_�[�t���[��AD�ϊ��v���o��
	//MTU3.TIER.BIT.TGIEA = 1;	//MTU3.TGRA�̊����ݗv��(TGIA)������
	MTU3.TIER.BIT.TTGE = 1;		//�R��AD�ϊ��J�n�v������������
	
	/***�o�͋��֎~�ݒ�***/
	MTU.TOER.BYTE = 0xC0;		//�����ݒ�ł͏o�͋֎~�ɐݒ�
//	MTU.TOER.BYTE = 0xFF;		//�S���o�͋���
	/***�o�͋��֎~�ݒ�***/
	
//	MTU.TSTR.BYTE = 0xC0;		//MTU3.TCNT��MTU4.TCNT�̃J�E���g������J�n
	

	/***MTU2�C���v�b�g�L���v�`���̐ݒ�***/
	MTU2.TCR.BIT.TPSC = 7;		//PCLK/1024�ŃJ�E���g
	MTU2.TCR.BIT.CKEG = 0;		//�����オ��G�b�W�ŃJ�E���g
	MTU2.TCR.BIT.CCLR = 2;		//TGRB�C���v�b�g�L���v�`����TCNT�N���A
	
	MTU2.TIOR.BIT.IOB = 0;		//�A�E�g�v�b�g�R���y�A�A�o�͋֎~(�t�B���^�L���������G�b�W�l��)
	MTU2.NFCR.BIT.NFBEN = 1;	//MTIOCnB�[�q�̃m�C�Y�t�B���^������
	MTU2.NFCR.BIT.NFCS = 2;		//�m�C�Y�t�B���^�N���b�N��PCLK/32
	
	vdg_wait_nop(100);		//�N���b�N�ݒ��A�ݒ肵���N���b�N2���������x�҂��Ă���C���v�b�g�L���v�`���ɐݒ�
	MTU2.TIOR.BIT.IOB = 10;		//MTIOC2B�𗼃G�b�W�ŃC���v�b�g�L���v�`��
	
	MTU2.TIER.BIT.TGIEB = 1;	//�����ݗv��TGIB������
	MTU2.TIER.BIT.TCIEV = 1;	//�I�[�o�[�t���[�����ݗv��TCIV������
	ICU.IER[0X0F].BIT.IEN6 = 1;	//TGIB2�̊����ݗv��������
	ICU.IER[0x0F].BIT.IEN7 = 1;	//TCIV2�̊����ݗv��������
	
	MTU.TSTR.BIT.CST2 = 0;		//�����1�ɂ���ƃJ�E���g�J�n
	/***MTU2�C���v�b�g�L���v�`���̐ݒ�***/
}

void vdg_S12AD_init()
{
	S12AD.ADCSR.BIT.ADCS = 0;		//�V���O���X�L�������[�h
	S12AD.ADCSR.BIT.ADIE = 1;		//�X�L�����I�������݋���
	S12AD.ADANSA.WORD = 0x01;		//AN000��ϊ��Ώۂɂ���
	S12AD.ADCER.BIT.ACE = 1;		//�f�[�^�ǂݏo���ɂ�鎩���N���A������
	S12AD.ADSTRGR.BIT.TRSA = 3;		//MTU3.TGRA�R���y�A�}�b�`��ADC�J�n�g���K�ݒ�
	S12AD.ADCSR.BIT.TRGE = 1;		//�g���K�J�n����

	ICU.IER[0x0C].BIT.IEN6 = 1;		//S12AD��S12ADI0�����ݗv��������
}

void vdg_SCI_init()
{
	/***PC�Ƃ̒ʐM�p***/
	SCI1.SCR.BYTE = 0x00;		//������
	SCI1.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	//SCI1.SIMR1.BIT.IICM = 0;	//�V���A���C���^�[�t�F�[�X���[�h
	SCI1.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI1.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI1.SNFR.BIT.NFCS = 4;		//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI1.BRR = 3;			//3:500000bps,9:200000bps
	SCI1.SCR.BIT.RE = 1;		//��M���������
	SCI1.SCR.BIT.TE = 1;		//���M���������
	SCI1.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI1.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	
	ICU.IER[0x1B].BIT.IEN4 = 1;	//���M�����݋���
	ICU.IER[0x1B].BIT.IEN3 = 1;	//��M�����݋���
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//���M���������݋���
	
	/***�}�X�^�[�}�C�R���Ƃ̒ʐM�p***/
	SCI12.SCR.BYTE = 0x00;		//������
	SCI12.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	//SCI1.SIMR1.BIT.IICM = 0;	//�V���A���C���^�[�t�F�[�X���[�h
	SCI12.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI12.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI12.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI12.SNFR.BIT.NFCS = 4;		//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI12.BRR = 7;			//3:500000bps,9:200000bps,7:250000bps
	SCI12.SCR.BIT.RE = 1;		//��M���������
	SCI12.SCR.BIT.TE = 1;		//���M���������
	SCI12.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI12.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	
	ICU.IER[0x1E].BIT.IEN0 = 1;	//���M�����݋���
	ICU.IER[0x1D].BIT.IEN7 = 1;	//��M�����݋���
}

//void vdg_IRQ_init()
//{
//	ICU.IER[0x08].BYTE = 0x00;		//�SIRQ�������ݗv���֎~
//	ICU.IRQFLTE0.BYTE = 0x00;		//�SIRQ���f�B�W�^���t�B���^����
//	ICU.IRQFLTC0.BIT.FCLKSEL5 = IRQ_FLT_8;	//�T���v�����O�N���b�NPCLK/8
//	ICU.IRQCR[5].BIT.IRQMD = 3;		//IRQ5���G�b�W���o
//	ICU.IR[69].BIT.IR = 0;			//IRQ5�����ݗv���t���O"0"�i[]���̓x�N�^�ԍ��j
//	ICU.IRQFLTE0.BIT.FLTEN5 = 1;		//�f�B�W�^���t�B���^�L��
//	//ICU.FIR.BIT.FVCT = 69;			//IRQ5�����������݂ɐݒ�B�x�N�^�ԍ�
//	//ICU.FIR.BIT.FIEN = 1;			//���������݂�����
//	ICU.IER[0x08].BIT.IEN5 = 1;		//IRQ5�����ݗv������
//}

void vdg_IPR_init()
{
	ICU.IPR[102].BIT.IPR = 13;	//S12ADI0�O�������ݗD��x
	ICU.IPR[218].BIT.IPR = 12;	//SCI1�����ݗD��x�i����M���ʁj
	ICU.IPR[238].BIT.IPR = 11;	//SCI12�����ݗD��x�i����M���ʁj
	ICU.IPR[125].BIT.IPR = 14;	//MTIOC2B�C���v�b�g�L���v�`�������ݗD��x
	ICU.IPR[127].BIT.IPR = 15;	//MTU2.TCNT�I�[�o�[�t���[�����ݗD��x
	
	//ICU.IPR[69].BIT.IPR = 15;	//IRQ5�����ݗD��x
}