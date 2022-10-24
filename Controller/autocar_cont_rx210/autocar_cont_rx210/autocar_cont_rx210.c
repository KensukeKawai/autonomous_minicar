
#include <iodefine.h>
#include <vect.h>
#include <mathf.h>
#include <machine.h>
#include "global_data.h"


//�g�p�n�[�h�E�F�A�ݒ�֐�
void vdg_HW_init()
{
	/*****************************************/
	// ICLK = PCLKD = 50MHs,		 //
	// PCLKB = FCLK = BCLK = 25MHz		 //
	/*****************************************/
	
	unsigned int i;
   
   	SYSTEM.PRCR.WORD = 0xA507;			//�N���b�N������H�֘A���W�X�^�A���샂�[�h�֘A���W�X�^�AVRCR���W�X�^�ւ̏����݋���
    
//  	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;			//MTU2a���W���[���X�g�b�v��ԉ���
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;			//SCI1���W���[���X�g�b�v��ԉ���
	SYSTEM.MSTPCRA.BIT.MSTPA5 = 0;			//TMR0���W���[���X�g�b�v��ԉ���
    
 	SYSTEM.VRCR = 0x00;				//VRCR���W�X�^�i�d�����M�����[�^���䃌�W�X�^�j��0x00
  	SYSTEM.HOCOCR2.BYTE = 0x03;			//�����I���`�b�v�I�V���[�^HOCR�̓�����g��=50MHz
    	SYSTEM.HOCOWTCR2.BYTE = 0x03;			//HOCO�N���b�N�����܂ł̑ҋ@���Ԑݒ�
    	SYSTEM.HOCOCR.BYTE = 0x00;			//HOCO����B��������HOCOWTCR2�Őݒ肵�����ԑ҂��Ȃ���΂����Ȃ�
    	for(i=0;i<100;i++){ nop();}			//9216�T�C�N�����҂�
	SYSTEM.SCKCR.LONG = 0x10811110;   		//ICLK,PCLKD: no division PCLKB,BCLK,FCLK: divide-by-2 */
	while (0x10811110 != SYSTEM.SCKCR.LONG){}	//SCKCR�̏������݊������Ă邩�m�F�҂�
	SYSTEM.BCKCR.BYTE = 0x01;
	while (0x01 != SYSTEM.BCKCR.BYTE){}
	SYSTEM.SCKCR3.WORD = 0x0100;			//HOCO�I��
	while (0x0100 != SYSTEM.SCKCR3.WORD){}		//�������܂�Ă邩�m�F�҂�
	SYSTEM.PRCR.WORD = 0xA500;			//PRCR���W�X�^�ɁA�e���W�X�^�ւ̏����݋֎~�ݒ�
    
	while(0 != SYSTEM.OPCCR.BIT.OPCMTSF){}		//�������샂�[�h�ڍs�̑J�ڏ�ԃt���O�Ď�
	SYSTEM.OPCCR.BIT.OPCM = 0;			//�������샂�[�h
}

//���o�͐ݒ�֐�
void vdg_IO_init()
{	
	//PORT0.PDR.BYTE = 0xFF;
	PORT1.PDR.BYTE = 0xFF;
	PORT2.PDR.BYTE = 0xFD;		//P21(RXD0)����́BRS232C���x���ϊ����g�p���Ȃ�����P21�͓��͐ݒ�
	PORT3.PDR.BYTE = 0xDE;		//P30(RXD1),P35(���͐ݒ�̂�)�̂ݓ���
	//PORT4.PDR.BYTE = 0xFF;
	PORT5.PDR.BYTE = 0xFF;
	PORTA.PDR.BYTE = 0xFF;
	PORTB.PDR.BYTE = 0xFF;
	PORTC.PDR.BYTE = 0xFF;
	//PORTD.PDR.BYTE = 0xFF;
	PORTE.PDR.BYTE = 0xFF;
	//PORTH.PDR.BYTE = 0xFF;
	//PORTJ.PDR.BYTE = 0xFF;
	
	//�C���^���v�^���̓|�[�g����͐ݒ�
	MOTOR1_IRQ = 0;
	
	//MPC�̏������݃v���e�N�g���W�X�^PWPR�ݒ�
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	
	MPC.PH1PFS.BIT.ISEL = 1;	//IRQ0�Ƃ��Ďg�p CN1-25
	
	MPC.P26PFS.BIT.PSEL = 10;	//TXD1�Ƃ��Ďg�p
	MPC.P30PFS.BIT.PSEL = 10;	//RXD1�Ƃ��Ďg�p
	
	MPC.PWPR.BIT.PFSWE = 0;		//P**PFS���W�X�^�ւ̏������݋֎~

	//�|�[�g���[�h���W�X�^�ݒ�B�ėp���o��or���Ӌ@�\
	/*** SCI1 ***/
	PORT2.PMR.BIT.B6 = 1;
	PORT3.PMR.BIT.B0 = 1;
}

void vdg_TMR_init()
{
//	TMR0.TCORA
	TMR0.TCR.BIT.CCLR = 1;	//�R���y�A�}�b�`A�ɂ��N���A
	TMR0.TCCR.BIT.CKS = 0;	//�����N���b�N��PCLK�ŃJ�E���g
	TMR0.TCCR.BIT.CSS = 2;	//�����N���b�N��PCLK�ŃJ�E���g
	
	//TMR0.TCR.BIT.OVIE = 1;	//�I�[�o�[�t���[�ɂ�銄���݂�����
	TMR0.TCR.BIT.CMIEA = 1;	//�R���y�A�}�b�`A�ɂ�銄���ݗv��������
	ICU.IER[0x15].BIT.IEN6 = 1;	//TMR0�R���y�A�}�b�`A�����ݗv������
	TMR0.TCORA = 49;	//1MHz�Ŋ����ݔ����Bf = PCLK/(N+1)�̂��߁A24�B
}

void IRQ_init()
{
	ICU.IER[0x08].BYTE = 0x00;
	ICU.IRQFLTE0.BYTE = 0x00;
	ICU.IRQFLTC0.WORD = 0x0000;

	ICU.IRQCR[0].BIT.IRQMD = 2;	//IRQ0�����オ��G�b�W���o
	ICU.IR[64].BIT.IR = 0;		//IRQ0�����ݗv���t���O"0"
	ICU.IRQFLTE0.BIT.FLTEN0 = 1;	//�f�B�W�^���t�B���^�L��
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 1;	//�T���v�����O�N���b�NPCLK/8
	ICU.IER[0x08].BIT.IEN0 = 1;	//IRQ0�����ݗv������
}

void IPR_init()
{
	ICU.IPR[64].BIT.IPR = 6;	//IRQ0�O�������ݗD��x
	ICU.IPR[218].BIT.IPR = 12;	//SCI1�����ݗD��x�i����M���ʁj
	ICU.IPR[174].BIT.IPR = 1;	//TMR0�R���y�A�}�b�`A�����ݗD��x
}

void wait_nop()
{
	unsigned long i;
	for(i=0;i<20000000;i++){ nop();}
}

void SCI1_init()
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

void Excep_TMR0_CMIA0()
{
	u1g_tmr_trg = 1;
	
	if(u1g_tricntr_dir == CNTUP)
	{
		s2g_tricntr++;
	}
	else
	{
		s2g_tricntr--;
	}
	
	if(s2g_tricntr >= CAREER_CNT)
	{
		u1g_tricntr_dir = CNTDOWN;		//�O�p�g�g�b�v���B������J�E���g�_�E��
		TRICNT_TRG_UP = !TRICNT_TRG_UP;
	}
	else if(s2g_tricntr <= 0)
	{
		u1g_tricntr_dir = CNTUP;
		//MOTOR1_VL_OUT = !MOTOR1_VL_OUT;
	}
	
}

void Excep_SCI1_RXI1()
{
}

void Excep_SCI1_TXI1()
{
}

void Excep_ICU_IRQ0()
{
	
}


void Variable_init()
{
	u1g_tricntr_dir = CNTUP;
	s2g_motor1_UH_cnt = 25;
	s2g_motor1_UL_cnt = 30;
	s2g_motor1_VH_cnt = 25;
	s2g_motor1_VL_cnt = 30;
	s2g_motor1_WH_cnt = 25;
	s2g_motor1_WL_cnt = 30;
}

void vdg_motor_set()
{
	if(s2g_tricntr <= s2g_motor1_UH_cnt)
	{
		MOTOR1_UH_OUT = 1;
	}
	else
	{
		MOTOR1_UH_OUT = 0;
	}
	if(s2g_tricntr >= s2g_motor1_UL_cnt)
	{
		MOTOR1_UL_OUT = 1;
	}
	else
	{
		MOTOR1_UL_OUT = 0;
	}
	if(s2g_tricntr <= s2g_motor1_VH_cnt)
	{
		MOTOR1_VH_OUT = 1;
	}
	else
	{
		MOTOR1_VH_OUT = 0;
	}
	if(s2g_tricntr >= s2g_motor1_VL_cnt)
	{
		MOTOR1_VL_OUT = 1;
	}
	else
	{
		MOTOR1_VL_OUT = 0;
	}
	if(s2g_tricntr <= s2g_motor1_WH_cnt)
	{
		MOTOR1_WH_OUT = 1;
	}
	else
	{
		MOTOR1_WH_OUT = 0;
	}
	if(s2g_tricntr >= s2g_motor1_WL_cnt)
	{
		MOTOR1_WL_OUT = 1;
	}
	else
	{
		MOTOR1_WL_OUT = 0;
	}
}

void main()
{
	vdg_HW_init();
	vdg_IO_init();
	Variable_init();
	vdg_TMR_init();
	//IRQ_init();
	IPR_init();
	//SCI1_init();


	while(1)
	{
		if(u1g_tmr_trg == 1)
		{
			u1g_tmr_trg = 0;
			SET_TRG = 1;
			vdg_motor_set();
			SET_TRG = 0;
		}
	}//while
	
}//main