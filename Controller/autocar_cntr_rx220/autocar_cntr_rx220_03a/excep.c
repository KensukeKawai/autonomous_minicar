
#include "common.h"

/***************�O���[�o���ϐ���`***************/
//excep���͐擪��"ex"��t�^����

/***MTU0.TGIA0�����ݕϐ���`***/
volatile unsigned char u1g_extgia0_interrupt;

/***�d���Z���TADC�֌W�ϐ���`***/
volatile unsigned short u2g_exs12adi0_adccntm1ad;
volatile unsigned short u2g_exs12adi0_adccntm2ad;
volatile unsigned char u1g_exs12adi0_xadcex;

/***SPI�֌W�ϐ���`***/
volatile unsigned char u1g_exspri0_xrspirec;


/***************�O���[�o���ϐ���`***************/



/**********TGRA�̃R���y�A�}�b�`�����ݏ���**********/
//���ꂪ�J�E���^�̎R�g���K�B�������珈���J�n���Ă����B
void Excep_MTU0_TGIA0()
{
	u1g_extgia0_interrupt = 1;
}

/**********AD�ϊ��̊����ݏ���**********/
//����񐔕���AD�ϊ����{�����Z
//����񐔎��{�����犮���t���O�𗧂Ă�
//ADDR0�FM1�AADDR1�FM2
void Excep_S12AD_S12ADI0()	//����
{
	/***************static�ϐ���`***************/
	volatile static unsigned short u2s_s12adi0_addrm1add = 0;
	volatile static unsigned short u2s_s12adi0_addrm2add = 0;
	volatile static unsigned char u1s_s12adi0_adccnt = 0;
	
	u1s_s12adi0_adccnt++;
	
	//ADC���ω񐔖����̏ꍇ
	if(u1s_s12adi0_adccnt < ADC_CNV_COUNT)
	{
		u2s_s12adi0_addrm1add += S12AD.ADDR0;
		u2s_s12adi0_addrm2add += S12AD.ADDR1;
		S12AD.ADCSR.BIT.ADST = 0;
		S12AD.ADCSR.BIT.ADST = 1;
	}
	
	//ADC���ω񐔂ɓ��B�����ꍇ
	else
	{
		u2s_s12adi0_addrm1add += S12AD.ADDR0;
		u2s_s12adi0_addrm2add += S12AD.ADDR1;
		//result�ɕ��ω񐔕����Z����ADC�J�E���g���i�[�BADC_CNT2V�ŕ��ω񐔕����܂߂Čv�Z���邽�߉��Z�l�ŗǂ��B
		u2g_exs12adi0_adccntm1ad = u2s_s12adi0_addrm1add;
		u2g_exs12adi0_adccntm2ad = u2s_s12adi0_addrm2add;
		//static�ϐ������Z�b�g
		u1s_s12adi0_adccnt = 0;
		u2s_s12adi0_addrm1add = 0;
		u2s_s12adi0_addrm2add = 0;

		//ADC_CNV_COUNT��ADC���{������A�t���O�𗧂Ă�
		u1g_exs12adi0_xadcex = 1;
	}
}

/**********SPI��M�o�b�t�@�t���̊����ݏ���**********/
//Jetson����̃`�b�v�Z���N�g�A�N�e�B�uLow����4Bite����M���������炱���ɗ���
//Jetson�w�ߎ�M�㎟�^�X�N��Jetson�ɑ��M�������f�[�^��Long Word�œ���Ă���
void Excep_RSPI0_SPRI0()
{
	// RSPI0.SPCR.BIT.SPRIE = 0;		//��M�����ݗv���̔���������
	// PORT_GENERAL_P1 != PORT_GENERAL_P1;
	u1g_exspri0_xrspirec = 1;		//Jetson�����SPI��M�����t���OON
	// RSPI0.SPDR.LONG = 0;
}


void Excep_SCI1_TXI1()
{
	
}

void Excep_SCI1_RXI1()
{
	
}