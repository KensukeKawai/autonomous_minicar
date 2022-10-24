
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
	//SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;			//MTU2a���W���[���X�g�b�v��ԉ���
	//SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;			//S12AD���W���[���X�g�b�v��ԉ���
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;			//SCI1���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRB.BIT.MSTPB26 = 0;			//SCI5���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRB.BIT.MSTPB25 = 0;			//SCI6���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRC.BIT.MSTPC26 = 0;			//SCI9���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRB.BIT.MSTPB4 = 0;			//SCI12���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;			//CMT0,CMT1���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRA.BIT.MSTPA5 = 0;			//TMR0,TMR1���W���[���X�g�b�v��ԉ���
	
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
	PORT2.PDR.BYTE = 0xFF;		//P26(TXD1)�o��
	PORT3.PDR.BYTE = 0xFE;		//P30(RXD1)����
	PORT4.PDR.BYTE = 0xFF;		//
	PORT5.PDR.BYTE = 0xFF;
	PORTA.PDR.BYTE = 0xF7;		//PA3(RXD5)����
	PORTB.PDR.BYTE = 0xBE;		//PB0(RXD6)���́APB6(RXD9)����
	PORTC.PDR.BYTE = 0x7F;		//PC7�̓{�[�h����͐ݒ�ɂ��Ă���
	PORTE.PDR.BYTE = 0xFB;		//PE1(TXD12)�o�́APE2(RXD12)����
	PORTH.PDR.BYTE = 0xFF;
	
	PORT2.PODR.BIT.B6 = 1;		//TXD1��HIGH
	PORTA.PODR.BIT.B4 = 1;		//TXD5��HIGH
	PORTB.PODR.BIT.B1 = 1;		//TXD6��HIGH
	PORTB.PODR.BIT.B7 = 1;		//TXD9��HIGH
	PORTE.PODR.BIT.B1 = 1;		//TXD12��HIGH
	
	PORT3.PCR.BIT.B0 = 1;		//RXD1�̓v���A�b�v
	PORTA.PCR.BIT.B3 = 1;		//RXD5�̓v���A�b�v
	PORTB.PCR.BIT.B0 = 1;		//RXD6�̓v���A�b�v
	PORTB.PCR.BIT.B6 = 1;		//RXD9�̓v���A�b�v
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
	
	MPC.PA4PFS.BIT.PSEL = 10;	//PA4(CN1-16)��TXD5
	MPC.PA3PFS.BIT.PSEL = 10;	//PA3(CN1-15)��RXD5
	MPC.PB1PFS.BIT.PSEL = 11;	//PB1(CN2-2)��TXD6
	MPC.PB0PFS.BIT.PSEL = 11;	//PB0(CN2-1)��RXD6
	MPC.PB7PFS.BIT.PSEL = 10;	//PB7(CN2-6)��TXD9
	MPC.PB6PFS.BIT.PSEL = 10;	//PB6(CN2-5)��RXD9
	
	MPC.PE1PFS.BIT.PSEL = 12;	//PE1(CN1-8)��TXD12
	MPC.PE2PFS.BIT.PSEL = 12;	//PE2(CN1-9)��RXD12
	
	/***API�ɍ��킹��MPC�Ńs������t��***/
	
	//P**PFS���W�X�^�ւ̏������݋֎~
	MPC.PWPR.BIT.PFSWE = 0;		

	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)�BP**PFS�̂����hPSEL�h�ɑ΂��Ă̂�PMR���`***/
	PORT2.PMR.BIT.B6 = 1;		//P26(CN2-23)��TXD1
	PORT3.PMR.BIT.B0 = 1;		//P30(CN2-25)��RXD1
	
	PORTA.PMR.BIT.B4 = 1;		//PA4(CN1-16)��TXD5
	PORTA.PMR.BIT.B3 = 1;		//PA3(CN1-15)��RXD5
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��TXD6
	PORTB.PMR.BIT.B0 = 1;		//PB0(CN2-1)��RXD6
	PORTB.PMR.BIT.B7 = 1;		//PB7(CN2-6)��TXD9
	PORTB.PMR.BIT.B6 = 1;		//PB6(CN2-5)��RXD9
	
	PORTE.PMR.BIT.B1 = 1;		//PE1(CN1-8)��TXD12
	PORTE.PMR.BIT.B2 = 1;		//PE2(CN1-9)��RXD12
	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)***/
}


void vdg_SCI_init()
{
	/***PC or Jetson �Ƃ̒ʐM�p***/
	SCI1.SCR.BYTE = 0x00;		//������
	SCI1.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	SCI1.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	SCI1.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI1.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI1.SNFR.BIT.NFCS = 4;		//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI1.BRR = 3;			//3:500000bps,9:200000bps
	SCI1.SCR.BIT.RE = 1;		//��M���������
	SCI1.SCR.BIT.TE = 1;		//���M���������
	SCI1.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI1.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	//SCI1.SCR.BIT.TEIE = 1;		//���M�I�������ݗv��������
	
	ICU.IER[0x1B].BIT.IEN4 = 1;	//���M�����݋���
	ICU.IER[0x1B].BIT.IEN3 = 1;	//��M�����݋���
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//���M���������݋���
	
	/***�X���[�u�}�C�R�� FL �Ƃ̒ʐM�p***/
	SCI5.SCR.BYTE = 0x00;		//������
	SCI5.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	SCI5.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	SCI5.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI5.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI5.SNFR.BIT.NFCS = 4;		//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI5.BRR = 3;			//3:500000bps,9:200000bps
	SCI5.SCR.BIT.RE = 1;		//��M���������
	SCI5.SCR.BIT.TE = 1;		//���M���������
	SCI5.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI5.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	//SCI5.SCR.BIT.TEIE = 1;		//���M�I�������ݗv��������
	
	ICU.IER[0x1C].BIT.IEN0 = 1;	//���M�����݋���
	//ICU.IER[0x1C].BIT.IEN1 = 1;	//���M�I�������݋���
	ICU.IER[0x1B].BIT.IEN7 = 1;	//��M�����݋���
	
	/***�X���[�u�}�C�R�� FR �Ƃ̒ʐM�p***/
	SCI6.SCR.BYTE = 0x00;		//������
	SCI6.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	SCI6.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	SCI6.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI6.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI6.SNFR.BIT.NFCS = 4;		//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI6.BRR = 3;			//3:500000bps,9:200000bps
	SCI6.SCR.BIT.RE = 1;		//��M���������
	SCI6.SCR.BIT.TE = 1;		//���M���������
	SCI6.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI6.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	//SCI6.SCR.BIT.TEIE = 1;		//���M�I�������ݗv��������
	
	ICU.IER[0x1C].BIT.IEN4 = 1;	//���M�����݋���
	//ICU.IER[0x1C].BIT.IEN5 = 1;	//���M�I�������݋���
	ICU.IER[0x1C].BIT.IEN3 = 1;	//��M�����݋���
	
	/***�X���[�u�}�C�R�� RL �Ƃ̒ʐM�p***/
	SCI9.SCR.BYTE = 0x00;		//������
	SCI9.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	SCI9.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	SCI9.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI9.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI9.SNFR.BIT.NFCS = 4;		//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI9.BRR = 3;			//3:500000bps,9:200000bps
	SCI9.SCR.BIT.RE = 1;		//��M���������
	SCI9.SCR.BIT.TE = 1;		//���M���������
	SCI9.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI9.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	//SCI9.SCR.BIT.TEIE = 1;		//���M�I�������ݗv��������
	
	ICU.IER[0x1D].BIT.IEN4 = 1;	//���M�����݋���
	//ICU.IER[0x1D].BIT.IEN5 = 1;	//���M�I�������݋���
	ICU.IER[0x1D].BIT.IEN3 = 1;	//��M�����݋���
	
	/***�X���[�u�}�C�R�� RR �Ƃ̒ʐM�p***/
	SCI12.SCR.BYTE = 0x00;		//������
	SCI12.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	SCI12.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	SCI12.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI12.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI12.SNFR.BIT.NFCS = 4;		//8�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI12.BRR = 7;			//3:500000bps,9:200000bps,7:250000bps
	SCI12.SCR.BIT.RE = 1;		//��M���������
	SCI12.SCR.BIT.TE = 1;		//���M���������
	SCI12.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI12.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	//SCI12.SCR.BIT.TEIE = 1;		//���M�I�������ݗv��������
	
	ICU.IER[0x1E].BIT.IEN0 = 1;	//���M�����݋���
	//ICU.IER[0x1E].BIT.IEN1 = 1;	//���M�I�������݋���
	ICU.IER[0x1D].BIT.IEN7 = 1;	//��M�����݋���
}

void vdg_CMT_init()
{
	CMT0.CMCR.BIT.CKS = 0;			//PCLK/8����
	CMT0.CMCR.BIT.CMIE = 1;			//�����݋���
	CMT0.CMCOR = CNT_SLAVE_SET;		//1kHz
	//CMT.CMSTR0.BIT.STR0 = 1;		//�J�E���g�J�n
	
	ICU.IER[0x03].BIT.IEN4 = 1;		//�����݋���
}

void vdg_TMR_init()
{
	
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
	//ICU.IPR[102].BIT.IPR = 13;	//S12ADI0�O�������ݗD��x
	ICU.IPR[218].BIT.IPR = 11;	//SCI1�����ݗD��x�i����M���ʁj
	ICU.IPR[222].BIT.IPR = 12;	//SCI5�����ݗD��x�i����M���ʁj
	ICU.IPR[226].BIT.IPR = 12;	//SCI6�����ݗD��x�i����M���ʁj
	ICU.IPR[234].BIT.IPR = 12;	//SCI9�����ݗD��x�i����M���ʁj
	ICU.IPR[238].BIT.IPR = 12;	//SCI12�����ݗD��x�i����M���ʁj
	
	ICU.IPR[004].BIT.IPR = 10;	//5kHz�����J�E���^�����ݗD��x
	
	//ICU.IPR[69].BIT.IPR = 15;	//IRQ5�����ݗD��x
}