
#include "iodefine.h"
#include "vect.h"
#include <mathf.h>
#include <machine.h>
#include "global_data.h"


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
	PORT1.PDR.BYTE = 0xDF;		//P15(RXD1)����
	PORT2.PDR.BYTE = 0xFF;
	PORT3.PDR.BYTE = 0xFE;		//P30(RXD1)����
	PORT4.PDR.BYTE = 0xFE;		//P40(AN000)����
	PORT5.PDR.BYTE = 0xFF;
	//PORTA.PDR.BYTE = 0xFD;		//PA1(CVREFA)����
	//PORTA.PDR.BYTE = 0xEF;		//PA4(IRQ5)�����
	PORTA.PDR.BYTE = 0xFF;
	PORTB.PDR.BYTE = 0xFF;
	PORTC.PDR.BYTE = 0x7F;		//PC7�̓{�[�h����͐ݒ�ɂ��Ă���
	//PORTE.PDR.BYTE = 0xF7;		//PE3(CMPA1)����
	PORTE.PDR.BYTE = 0xDF;		//PE5(MTIOC2B)�����
	PORTH.PDR.BYTE = 0xFF;
	
	
	//PORTA.PDR.BIT.B1 = 0;		//PA1(CVREFA)����
	//PORTE.PDR.BIT.B3 = 0;		//PE3(CMPA1)����
}

void vdg_MPC_init()
{
	/***MPC�ݒ�̂��߂ɏ������݃v���e�N�g���W�X�^PWPR�ݒ�***/
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	/***MPC�ݒ�̂��߂ɏ������݃v���e�N�g���W�X�^PWPR�ݒ�***/
	
	//PORTA.PMR.BIT.B1 = 0;
	//PORTE.PMR.BIT.B3 = 0;
	
	/***API�ɍ��킹��MPC�Ńs������t��***/
	MPC.P15PFS.BIT.PSEL = 10;	//P15(CN2-20)��RXD1
	MPC.P16PFS.BIT.PSEL = 10;	//P16(CN2-21)��TXD1
	MPC.P40PFS.BIT.ASEL = 1;	//P40(CN1-1)��AN000
	//MPC.PA1PFS.BIT.ASEL = 1;	//PA1(CN1-14)��CVREFA
	MPC.PB1PFS.BIT.PSEL = 2;	//PB1(CN2-2)��MTIOC4C
	MPC.PB3PFS.BIT.PSEL = 2;	//PB3(CN2-3)��MTIOC4A
	MPC.PC2PFS.BIT.PSEL = 1;	//PC2(CN2-7)��MTIOC4B
	MPC.PC3PFS.BIT.PSEL = 1;	//PC3(CN2-8)��MTIOC4D
	MPC.PC4PFS.BIT.PSEL = 1;	//PC4(CN2-9)��MTIOC3D
	MPC.PC5PFS.BIT.PSEL = 1;	//PC5(CN2-10)��MTIOC3B
	MPC.PE5PFS.BIT.PSEL = 2;	//PE5(CN1-12)��MTIOC2B
	//MPC.PE3PFS.BIT.ASEL = 1;	//PE3(CN1-10)��CMPA1
	
	MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)��MTIOC3A�g�O���o��
	//MPC.PA4PFS.BIT.ISEL = 1;	//PA4(CN1-16)��IRQ5
	/***API�ɍ��킹��MPC�Ńs������t��***/
	
	//P**PFS���W�X�^�ւ̏������݋֎~
	MPC.PWPR.BIT.PFSWE = 0;		

	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)�BP**PFS�̂����hPSEL�h�ɑ΂��Ă̂�PMR���`***/
	PORT1.PMR.BIT.B5 = 1;		//P15(CN2-20)��RXD1
	PORT1.PMR.BIT.B6 = 1;		//P16(CN2-21)��TXD1
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
	PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
	PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
	PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
	PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
	PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
	PORTE.PMR.BIT.B5 = 1;		//PE5(CN1-12)��MTIOC2B
	
	PORT1.PMR.BIT.B4 = 1;		//P14(CN2-19)��MTIOC3A
	/***�|�[�g���[�h���W�X�^�ݒ�(�ėp���o��or���Ӌ@�\)***/
}

void vdg_wait_nop(unsigned long N)
{
	unsigned long i;
	for(i=0;i<N;i++){ nop();}
}

void vdg_MTU2a_init()
{
	MTU.TSTR.BYTE = 0x00;		//TCNT�J�E���g���~
	MTU3.TCR.BYTE = 0x00;		//PCLK/1�A�����オ��G�b�W�ŃJ�E���g�ATCNT�N���A�֎~
	MTU4.TCR.BYTE = 0x00;		//PCLK/1�A�����オ��G�b�W�ŃJ�E���g�ATCNT�N���A�֎~
	
	/***TCNT�̏����ݒ�***/
	MTU3.TCNT = DEADTIME;		//�f�b�h�^�C���ݒ�
	MTU4.TCNT = 0;
	/***TCNT�̏����ݒ�***/
	
	/***�f���[�e�B��̏����ݒ�***/
	/*�R���y�A���W�X�^*/
	MTU3.TGRB = DEFAULT_DUTY;
	MTU4.TGRA = DEFAULT_DUTY;
	MTU4.TGRB = DEFAULT_DUTY;
	/*�R���y�A���W�X�^*/
	/*�o�b�t�@���W�X�^*/
	MTU3.TGRD = DEFAULT_DUTY;
	MTU4.TGRC = DEFAULT_DUTY;
	MTU4.TGRD = DEFAULT_DUTY;
	/*�o�b�t�@���W�X�^*/
	/***�f���[�e�B��̏����ݒ�***/
	
	/***�f�b�h�^�C���A�L�����A�����̐ݒ�***/
	MTU.TDDR = DEADTIME;
	MTU.TCDR = CARRIER_HALF_COUNT;
	MTU.TCBR = CARRIER_HALF_COUNT;
	MTU3.TGRA = CARRIER_HALF_COUNT + DEADTIME;
	MTU3.TGRC =  CARRIER_HALF_COUNT + DEADTIME;
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

void vdg_SCI1_init()
{
	SCI1.SCR.BYTE = 0x00;		//������
	SCI1.SCR.BIT.CKE = 0;		//SCK�[�q��I/O�|�[�g�Ƃ��Ďg�p
	//SCI1.SIMR1.BIT.IICM = 0;	//�V���A���C���^�[�t�F�[�X���[�h
	SCI1.SMR.BYTE = 0x00;		//PCLK�A1�X�g�b�v�r�b�g�A�p���e�B���A�f�[�^��8bit�A����������
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = 1;		//��{�N���b�N8�T�C�N����1�r�b�g��
	SCI1.SEMR.BIT.NFEN = 1;		//�f�B�W�^���m�C�Y�t�B���^�L��
	SCI1.SNFR.BIT.NFCS = 0;		//1�������̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI1.BRR = 4;			//4:312500bps, 9:156250bps
	SCI1.SCR.BIT.RE = 1;		//��M���������
	SCI1.SCR.BIT.TE = 1;		//���M���������
	SCI1.SCR.BIT.RIE = 1;		//��M�y�уG���[�����ݗv��������
	SCI1.SCR.BIT.TIE = 1;		//���M�����ݗv��������
	
	ICU.IER[0x1B].BIT.IEN4 = 1;	//���M�����݋���
	ICU.IER[0x1B].BIT.IEN3 = 1;	//��M�����݋���
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//���M���������݋���
}


void vdg_CMPA_init()
{
	SYSTEM.PRCR.BIT.PRC3 = 1;		//LVD�֘A���W�X�^�ւ̏������݋���
	//SYSTEM.LVCMPCR.BIT.EXVREFINP1 = 1;	//�R���p���[�^A1�O��ref�d�����͑I���FCVREFA�[�q���͓d��
	//SYSTEM.LVCMPCR.BIT.EXVCCINP1 = 1;	//�R���p���[�^A1��r�d�����͑I���FCMPA1�[�q���͓d��
	SYSTEM.LVCMPCR.BYTE = 0x03;		//CVREFA,CMPA1�[�q���͓d��
	//SYSTEM.LVD1CR0.BIT.LVD1FSAMP = 
	SYSTEM.LVD1CR0.BIT.LVD1DFDIS = 0;	//�f�B�W�^���t�B���^�h�����h
	SYSTEM.LVD1CR0.BIT.LVD1RI = 0;		//�R���p���[�^A1������
	SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 2;	//������ELC���������FCMPA1��CVREFA���㏸���~���g���K
	SYSTEM.LVD1CR1.BIT.LVD1IRQSEL = 1;	//�����ݎ�ނ��u�}�X�J�u�������݁v�ɐݒ�
	SYSTEM.LVCMPCR.BIT.LVD1E = 1;		//�R���p���[�^A1��H�L��
	vdg_wait_nop(50000);
	
	SYSTEM.LVD1CR0.BIT.LVD1CMPE = 1;	//�R���p���[�^A1��H��r���ʏo�͋���
	SYSTEM.LVD1SR.BIT.LVD1DET = 0;		//�R���p���[�^A1�d���ω����o�t���O�B�����ݒ��0�B
	SYSTEM.LVD1CR0.BIT.LVD1RIE = 1;		//�R���p���[�^A1�����ݗv������
	
	ICU.IER[0x0B].BIT.IEN0 = 1;		//�R���p���[�^A1�����݋���
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
	ICU.IPR[125].BIT.IPR = 14;	//MTIOC2B�C���v�b�g�L���v�`�������ݗD��x
	ICU.IPR[127].BIT.IPR = 15;	//MTU2.TCNT�I�[�o�[�t���[�����ݗD��x
	
	//ICU.IPR[69].BIT.IPR = 15;	//IRQ5�����ݗD��x
}



void Excep_S12AD_S12ADI0()
{
	u2g_adc_result = S12AD.ADDR0;
	u1g_adc_xadcex = 1;
}

void Excep_SCI1_RXI1()
{
}

void Excep_SCI1_TXI1()
{
}

void Excep_MTU2_TGIB2()
{

	volatile unsigned char i;
	volatile unsigned short u2t_tgib2_count;
	volatile float f4t_tgib2_time;
	volatile float f4t_tgib2_nm;
	volatile float f4t_tgib2_nmadd = 0;
	volatile float f4t_tgib2_elanglefull;
	volatile unsigned short u2t_tgib2_countel360;
	
	volatile static float f4s_tgib2_nmary[NM_AVE_NUM] = {0};
	volatile static float f4s_tgib2_nmsmz = 0;
	volatile static float f4s_tgib2_cel360 = 1;

	
	u2t_tgib2_count = MTU2.TGRB;			//TGRB�J�E���g�����擾
	
	f4t_tgib2_time = (float)u2t_tgib2_count / PCLK1024;	//�J�E���g���~�N���b�N����
	f4t_tgib2_nm = K1PLST2RPM / f4t_tgib2_time;	//1�p���X���Ԃ����]���Z�o
	
	if(u1g_tciv2_xovflw == 1)
	{
		for( i = 0; i < NM_AVE_NUM; i++ )
		{
			f4s_tgib2_nmary[i] = 0;
		}
		u1g_tciv2_xovflw = 0;
	}

	for( i = 1; i < NM_AVE_NUM; i++ )
	{
		f4s_tgib2_nmary[NM_AVE_NUM-i] = f4s_tgib2_nmary[NM_AVE_NUM-i-1];
	}
	
	f4s_tgib2_nmary[0] = f4t_tgib2_nm;
	
	for( i = 0; i < NM_AVE_NUM; i++ )
	{
		f4t_tgib2_nmadd += f4s_tgib2_nmary[i];
	}
	f4g_tgib2_nmsm = f4t_tgib2_nmadd / (float)NM_AVE_NUM;

	
	u2g_tgib2_citrpt++;
	if(u2g_tgib2_citrpt >= ENC_NTEETH*2)
	{
		u2g_tgib2_citrpt -= ENC_NTEETH*2;
		u1g_tgib2_xzero = 1;
	}
	
	f4g_tgib2_mcangle = (float)u2g_tgib2_citrpt * (float)KPLSN2MANGLE;
	f4t_tgib2_elanglefull = f4g_tgib2_mcangle * POLE_PAIRS;
	
	u2t_tgib2_countel360 = f4t_tgib2_elanglefull / 360;
	f4g_tgib2_elangle = f4t_tgib2_elanglefull - (float)u2t_tgib2_countel360 * 360;
}

void Excep_MTU2_TCIV2()
{
	/***�I�[�o�[�t���[�͖�2.09s�Ŕ���***/
	u1g_tciv2_xovflw = 1;			//�z��ߋ��v�f�̏������̂��߂Ƀt���O���J
	f4g_tgib2_nmsm = 0;			//�I�[�o�[�t���[�������]��0
	f4g_tgib2_dnm = 0;			//�I�[�o�[�t���[�������]���ω���0
}


//void Excep_CMPA_CMPA1()
//{
//	SYSTEM.LVD1SR.BIT.LVD1DET = 0;		//�R���p���[�^A1�d���ω����o�t���O���蓮���Z�b�g
//	CMPA_TRG = !CMPA_TRG;
//	//���_�w�K��Ɋp�x���o�J�n
//	//�����Ń^�C�}�ƘA�g���Ċ����ݎ��Ԃ���p���x���Z�o
//}

void vdg_variable_init()
{

}

void vdg_current_cal()
{
	volatile unsigned short i;
	volatile float f4t_current_vout;
	volatile float f4t_current_imadd = 0;
	volatile static float f4s_current_imary[ADC_AVE_NUM] = {0};	//�S�ď�����
	
	f4t_current_vout = u2g_adc_result * ADC_CNT2V;
	
	for( i = 1; i < ADC_AVE_NUM; i++ )
	{
		f4s_current_imary[ADC_AVE_NUM-i] = f4s_current_imary[ADC_AVE_NUM-i-1];
	}
	
	f4s_current_imary[0] = (f4t_current_vout - ACS724_VMED) * ACS724_IRATE;	//(ADC�d��-2.5V)*5
	
	for( i = 0; i < ADC_AVE_NUM; i++ )
	{
		f4t_current_imadd += f4s_current_imary[i];
	}
	f4g_current_imsm = f4t_current_imadd / (float)ADC_AVE_NUM;
	
	f4g_current_i = f4t_current_vout;
}

void vdg_motor_cntr()
{
		
	//f4g_tgib2_dnm = f4g_tgib2_nmsm - f4s_tgib2_nmsmz;	//1�L�����A���̉�]���ω���(����l-�O��l)
	
	//f4s_tgib2_nmsmz = f4g_tgib2_nmsm;		//�O��l���b�`
}

void main()
{
	vdg_HW_init();
	vdg_IO_init();
	vdg_MPC_init();
	vdg_variable_init();
	vdg_MTU2a_init();
	vdg_S12AD_init();
	//vdg_IRQ_init();
	//vdg_SCI1_init();
	//vdg_CMPA_init();
	vdg_IPR_init();
	
	MTU.TOER.BYTE = 0xFF;		//�S���o�͋���
	MTU.TSTR.BYTE = 0xC4;		//MTU2.TCNT,MTU3.TCNT��MTU4.TCNT�̃J�E���g������J�n

	while(1)
	{

		if(u1g_adc_xadcex == 1)
		{
			vdg_current_cal();		//�d���Z���TADC���ʂ���d���l�Z�o
			
			MTU3.TGRD = 2000;
			MTU4.TGRC = 2000;
			MTU4.TGRD = 2000;
			
			if(u2g_adc_result > 2200){TRG1 = 1;}
			else{TRG1 = 0;}
			
			u1g_adc_xadcex = 0;
		}
		
	}//while
	
}//main