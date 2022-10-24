
#include "common.h"

/***************�O���[�o���ϐ���`***************/
volatile signed long s4g_mtcnt_nmsm1;
volatile signed long s4g_mtcnt_nmsm2;
volatile unsigned char u1g_mtcnt_idstagem1;
volatile unsigned char u1g_mtcnt_idstagem2;

volatile unsigned char u1g_mtcnt_idorthantm1;
volatile unsigned char u1g_mtcnt_idorthantm2;

volatile unsigned short u2g_mtcnt_cntm1 = CNT_MAX;
volatile unsigned short u2g_mtcnt_cntm2 = CNT_MAX;

volatile unsigned char u1g_mtcnt_xnormal;
volatile unsigned char u1g_mtcnt_xmtorigin;
volatile unsigned char u1g_mtcnt_xstop;

//���[�^��]����LPF����
void vdg_mtcnt_nmsmcal()
{
	/**********static�ϐ���`**********/
	volatile static float f4s_mtcnt_nm1z = 0;
	volatile static float f4s_mtcnt_nmsm1z = 0;
	volatile static float f4s_mtcnt_nm2z = 0;
	volatile static float f4s_mtcnt_nmsm2z = 0;

	/**********�e���|�����ϐ���`**********/
	volatile float f4t_mtcnt_nmsm1;
	volatile float f4t_mtcnt_nmsm2;

	//1��LPF����
	f4t_mtcnt_nmsm1 = LPF_K1*((float)(s4g_mtsensor_nm1)+f4s_mtcnt_nm1z)-LPF_K2*f4s_mtcnt_nmsm1z;
	f4t_mtcnt_nmsm2 = LPF_K1*((float)(s4g_mtsensor_nm2)+f4s_mtcnt_nm2z)-LPF_K2*f4s_mtcnt_nmsm2z;

	//�O��l���b�`����
	f4s_mtcnt_nm1z = (float)(s4g_mtsensor_nm1);
	f4s_mtcnt_nm2z = (float)(s4g_mtsensor_nm2);
	f4s_mtcnt_nmsm1z = f4t_mtcnt_nmsm1;
	f4s_mtcnt_nmsm2z = f4t_mtcnt_nmsm2;
	s4g_mtcnt_nmsm1 = (signed long)(f4t_mtcnt_nmsm1);
	s4g_mtcnt_nmsm2 = (signed long)(f4t_mtcnt_nmsm2);
}

//���݂̃��[�^�d�C�p����w�胂�[�^�Ɏw������֐i�p�X�e�[�WID����
void vdg_mtcnt_stagejdg(unsigned char motor, unsigned char id_direction)
{
	switch(motor)
	{
		//Motor1
		case ID_MOTOR1:
		switch(id_direction)
		{
			//�O�i�����ɐi�p
			case ID_MT_ADV:
			if(f4g_mtsensor_eanglem1>=0 && f4g_mtsensor_eanglem1<60)
			{ u1g_mtcnt_idstagem1 = ID_STAGE1; }
			else if(f4g_mtsensor_eanglem1>=60 && f4g_mtsensor_eanglem1<120)
			{ u1g_mtcnt_idstagem1 = ID_STAGE2; }
			else if(f4g_mtsensor_eanglem1>=120 && f4g_mtsensor_eanglem1<180)
			{ u1g_mtcnt_idstagem1 = ID_STAGE3; }
			else if(f4g_mtsensor_eanglem1>=180 && f4g_mtsensor_eanglem1<240)
			{ u1g_mtcnt_idstagem1 = ID_STAGE4; }
			else if(f4g_mtsensor_eanglem1>=240 && f4g_mtsensor_eanglem1<300)
			{ u1g_mtcnt_idstagem1 = ID_STAGE5; }
			else if(f4g_mtsensor_eanglem1>=300 && f4g_mtsensor_eanglem1<360)
			{ u1g_mtcnt_idstagem1 = ID_STAGE6; }
			break;
			//��i�����ɐi�p
			case ID_MT_BACK:
			if(f4g_mtsensor_eanglem1>=0 && f4g_mtsensor_eanglem1<60)
			{ u1g_mtcnt_idstagem1 = ID_STAGE4; }
			else if(f4g_mtsensor_eanglem1>=60 && f4g_mtsensor_eanglem1<120)
			{ u1g_mtcnt_idstagem1 = ID_STAGE5; }
			else if(f4g_mtsensor_eanglem1>=120 && f4g_mtsensor_eanglem1<180)
			{ u1g_mtcnt_idstagem1 = ID_STAGE6; }
			else if(f4g_mtsensor_eanglem1>=180 && f4g_mtsensor_eanglem1<240)
			{ u1g_mtcnt_idstagem1 = ID_STAGE1; }
			else if(f4g_mtsensor_eanglem1>=240 && f4g_mtsensor_eanglem1<300)
			{ u1g_mtcnt_idstagem1 = ID_STAGE2; }
			else if(f4g_mtsensor_eanglem1>=300 && f4g_mtsensor_eanglem1<360)
			{ u1g_mtcnt_idstagem1 = ID_STAGE3; }
			break;
		}
		break;
		//Motor2
		case ID_MOTOR2:
		switch(id_direction)
		{
			//�O�i�����ɐi�p
			case ID_MT_ADV:
			if(f4g_mtsensor_eanglem2>=0 && f4g_mtsensor_eanglem2<60)
			{ u1g_mtcnt_idstagem2 = ID_STAGE1; }
			else if(f4g_mtsensor_eanglem2>=60 && f4g_mtsensor_eanglem2<120)
			{ u1g_mtcnt_idstagem2 = ID_STAGE2; }
			else if(f4g_mtsensor_eanglem2>=120 && f4g_mtsensor_eanglem2<180)
			{ u1g_mtcnt_idstagem2 = ID_STAGE3; }
			else if(f4g_mtsensor_eanglem2>=180 && f4g_mtsensor_eanglem2<240)
			{ u1g_mtcnt_idstagem2 = ID_STAGE4; }
			else if(f4g_mtsensor_eanglem2>=240 && f4g_mtsensor_eanglem2<300)
			{ u1g_mtcnt_idstagem2 = ID_STAGE5; }
			else if(f4g_mtsensor_eanglem2>=300 && f4g_mtsensor_eanglem2<360)
			{ u1g_mtcnt_idstagem2 = ID_STAGE6; }
			break;
			//��i�����ɐi�p
			case ID_MT_BACK:
			if(f4g_mtsensor_eanglem2>=0 && f4g_mtsensor_eanglem2<60)
			{ u1g_mtcnt_idstagem2 = ID_STAGE4; }
			else if(f4g_mtsensor_eanglem2>=60 && f4g_mtsensor_eanglem2<120)
			{ u1g_mtcnt_idstagem2 = ID_STAGE5; }
			else if(f4g_mtsensor_eanglem2>=120 && f4g_mtsensor_eanglem2<180)
			{ u1g_mtcnt_idstagem2 = ID_STAGE6; }
			else if(f4g_mtsensor_eanglem2>=180 && f4g_mtsensor_eanglem2<240)
			{ u1g_mtcnt_idstagem2 = ID_STAGE1; }
			else if(f4g_mtsensor_eanglem2>=240 && f4g_mtsensor_eanglem2<300)
			{ u1g_mtcnt_idstagem2 = ID_STAGE2; }
			else if(f4g_mtsensor_eanglem2>=300 && f4g_mtsensor_eanglem2<360)
			{ u1g_mtcnt_idstagem2 = ID_STAGE3; }
			break;
		}
		break;
	}
}

//API�ł�OFF�o�͂�GND�ɂȂ炸�����Ă��邽�߁APMR��ėp�o��LOW�ɂ���
void vdg_mtcnt_outset(unsigned char motor, unsigned char mode, unsigned short cntduty)
{

//�O�����Őݒ肳�ꂽduty�J�E���g�ƒ��O�Ŕ��肵���X�e�[�W���[�h����o��

	switch(motor)
	{
		case ID_MOTOR1:
		switch(mode)
		{
			case ID_STAGE1:		//UH �� VL
			PORT_M1_UH = 1;
			PORT_M1_VH = 0;
			PORT_M1_WH = 0;
			PORT_M1_UL = 0;
			PORT_M1_VL = 1;
			PORT_M1_WL = 0;
			MTU_M1_UH = cntduty;
			MTU_M1_VH = CNT_OUTOFF;
			MTU_M1_WH = CNT_OUTOFF;
			break;

			case ID_STAGE2:		//UH �� WL
			PORT_M1_UH = 1;
			PORT_M1_VH = 0;
			PORT_M1_WH = 0;
			PORT_M1_UL = 0;
			PORT_M1_VL = 0;
			PORT_M1_WL = 1;
			MTU_M1_UH = cntduty;
			MTU_M1_VH = CNT_OUTOFF;
			MTU_M1_WH = CNT_OUTOFF;
			break;
			
			case ID_STAGE3:		//VH �� WL
			PORT_M1_UH = 0;
			PORT_M1_VH = 1;
			PORT_M1_WH = 0;
			PORT_M1_UL = 0;
			PORT_M1_VL = 0;
			PORT_M1_WL = 1;
			MTU_M1_UH = CNT_OUTOFF;
			MTU_M1_VH = cntduty;
			MTU_M1_WH = CNT_OUTOFF;
			break;
			
			case ID_STAGE4:		//VH �� UL
			PORT_M1_UH = 0;
			PORT_M1_VH = 1;
			PORT_M1_WH = 0;
			PORT_M1_UL = 1;
			PORT_M1_VL = 0;
			PORT_M1_WL = 0;
			MTU_M1_UH = CNT_OUTOFF;
			MTU_M1_VH = cntduty;
			MTU_M1_WH = CNT_OUTOFF;
			break;
			
			case ID_STAGE5:		//WH �� UL
			PORT_M1_UH = 0;
			PORT_M1_VH = 0;
			PORT_M1_WH = 1;
			PORT_M1_UL = 1;
			PORT_M1_VL = 0;
			PORT_M1_WL = 0;
			MTU_M1_UH = CNT_OUTOFF;
			MTU_M1_VH = CNT_OUTOFF;
			MTU_M1_WH = cntduty;
			break;

			case ID_STAGE6:		//WH �� VL
			PORT_M1_UH = 0;
			PORT_M1_VH = 0;
			PORT_M1_WH = 1;
			PORT_M1_UL = 0;
			PORT_M1_VL = 1;
			PORT_M1_WL = 0;
			MTU_M1_UH = CNT_OUTOFF;
			MTU_M1_VH = CNT_OUTOFF;
			MTU_M1_WH = cntduty;
			break;

			case ID_ALLSET:
			PORT_M1_UH = 1;
			PORT_M1_VH = 1;
			PORT_M1_WH = 1;
			PORT_M1_UL = 1;
			PORT_M1_VL = 1;
			PORT_M1_WL = 1;
			break;
			
			case ID_LOW_ON:
			PORT_M1_UH = 0;
			PORT_M1_VH = 0;
			PORT_M1_WH = 0;
			PORT_M1_UL = 1;
			PORT_M1_VL = 1;
			PORT_M1_WL = 1;
			MTU_M1_UH = CNT_OUTOFF;
			MTU_M1_VH = CNT_OUTOFF;
			MTU_M1_WH = CNT_OUTOFF;
			break;
			
			case ID_HIGH_ON:
			PORT_M1_UH = 1;
			PORT_M1_VH = 1;
			PORT_M1_WH = 1;
			PORT_M1_UL = 0;
			PORT_M1_VL = 0;
			PORT_M1_WL = 0;
			MTU_M1_UH = cntduty;
			MTU_M1_VH = cntduty;
			MTU_M1_WH = cntduty;
			break;
			
			case ID_ALLOFF:
			PORT_M1_UH = 0;
			PORT_M1_VH = 0;
			PORT_M1_WH = 0;
			PORT_M1_UL = 0;
			PORT_M1_VL = 0;
			PORT_M1_WL = 0;
			MTU_M1_UH = CNT_OUTOFF;
			MTU_M1_VH = CNT_OUTOFF;
			MTU_M1_WH = CNT_OUTOFF;
			break;
		}
		break;

		case ID_MOTOR2:
		switch(mode)
		{
			case ID_STAGE1:		//UH �� VL
			PORT_M2_UH = 1;
			PORT_M2_VH = 0;
			PORT_M2_WH = 0;
			PORT_M2_UL = 0;
			PORT_M2_VL = 1;
			PORT_M2_WL = 0;
			MTU_M2_UH = cntduty;
			MTU_M2_VH = CNT_OUTOFF;
			MTU_M2_WH = CNT_OUTOFF;
			break;

			case ID_STAGE2:		//UH �� WL
			PORT_M2_UH = 1;
			PORT_M2_VH = 0;
			PORT_M2_WH = 0;
			PORT_M2_UL = 0;
			PORT_M2_VL = 0;
			PORT_M2_WL = 1;
			MTU_M2_UH = cntduty;
			MTU_M2_VH = CNT_OUTOFF;
			MTU_M2_WH = CNT_OUTOFF;
			break;
			
			case ID_STAGE3:		//VH �� WL
			PORT_M2_UH = 0;
			PORT_M2_VH = 1;
			PORT_M2_WH = 0;
			PORT_M2_UL = 0;
			PORT_M2_VL = 0;
			PORT_M2_WL = 1;
			MTU_M2_UH = CNT_OUTOFF;
			MTU_M2_VH = cntduty;
			MTU_M2_WH = CNT_OUTOFF;
			break;
			
			case ID_STAGE4:		//VH �� UL
			PORT_M2_UH = 0;
			PORT_M2_VH = 1;
			PORT_M2_WH = 0;
			PORT_M2_UL = 1;
			PORT_M2_VL = 0;
			PORT_M2_WL = 0;
			MTU_M2_UH = CNT_OUTOFF;
			MTU_M2_VH = cntduty;
			MTU_M2_WH = CNT_OUTOFF;
			break;
			
			case ID_STAGE5:		//WH �� UL
			PORT_M2_UH = 0;
			PORT_M2_VH = 0;
			PORT_M2_WH = 1;
			PORT_M2_UL = 1;
			PORT_M2_VL = 0;
			PORT_M2_WL = 0;
			MTU_M2_UH = CNT_OUTOFF;
			MTU_M2_VH = CNT_OUTOFF;
			MTU_M2_WH = cntduty;
			break;

			case ID_STAGE6:		//WH �� VL
			PORT_M2_UH = 0;
			PORT_M2_VH = 0;
			PORT_M2_WH = 1;
			PORT_M2_UL = 0;
			PORT_M2_VL = 1;
			PORT_M2_WL = 0;
			MTU_M2_UH = CNT_OUTOFF;
			MTU_M2_VH = CNT_OUTOFF;
			MTU_M2_WH = cntduty;
			break;

			case ID_ALLSET:
			PORT_M2_UH = 1;
			PORT_M2_VH = 1;
			PORT_M2_WH = 1;
			PORT_M2_UL = 1;
			PORT_M2_VL = 1;
			PORT_M2_WL = 1;
			break;
			
			case ID_LOW_ON:
			PORT_M2_UH = 0;
			PORT_M2_VH = 0;
			PORT_M2_WH = 0;
			PORT_M2_UL = 1;
			PORT_M2_VL = 1;
			PORT_M2_WL = 1;
			MTU_M2_UH = CNT_OUTOFF;
			MTU_M2_VH = CNT_OUTOFF;
			MTU_M2_WH = CNT_OUTOFF;
			break;
			
			case ID_HIGH_ON:
			PORT_M2_UH = 1;
			PORT_M2_VH = 1;
			PORT_M2_WH = 1;
			PORT_M2_UL = 0;
			PORT_M2_VL = 0;
			PORT_M2_WL = 0;
			MTU_M2_UH = cntduty;
			MTU_M2_VH = cntduty;
			MTU_M2_WH = cntduty;
			break;
			
			case ID_ALLOFF:
			PORT_M2_UH = 0;
			PORT_M2_VH = 0;
			PORT_M2_WH = 0;
			PORT_M2_UL = 0;
			PORT_M2_VL = 0;
			PORT_M2_WL = 0;
			MTU_M2_UH = CNT_OUTOFF;
			MTU_M2_VH = CNT_OUTOFF;
			MTU_M2_WH = CNT_OUTOFF;
			break;
		}
		break;
	}
}

//�͍s�A�񐶔���
void vdg_mtcnt_orthantjdg()
{
	//Motor1
	//��Nm�F���A�ڕW-���F���@�ː��]�͍s
	if((s4g_mtcnt_nmsm1>=0) && (s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1>=0))
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_FWDPWR;
	}
	//��Nm�F���A�ڕW-���F���@�ː��]��
	else if((s4g_mtcnt_nmsm1>=0) && (s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1<0))
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_FWDREG;
	}
	//��Nm�F���A�ڕW-���F���@�ˋt�]�͍s
	else if((s4g_mtcnt_nmsm1<0) && (s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1<=0))
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_REVPWR;
	}
	//��Nm�F���A�ڕW-���F���@�ˋt�]��
	else	// if(s4g_mtcnt_nmsm1<0 && s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1 > 0)
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_REVREG;
	}

	//Motor2
	//��Nm�F���A�ڕW-���F���@�ː��]�͍s
	if((s4g_mtcnt_nmsm2>=0) && (s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2>=0))
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_FWDPWR;
	}
	//��Nm�F���A�ڕW-���F���@�ː��]��
	else if((s4g_mtcnt_nmsm2>=0) && (s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2<0))
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_FWDREG;
	}
	//��Nm�F���A�ڕW-���F���@�ˋt�]�͍s
	else if((s4g_mtcnt_nmsm2<0) && (s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2<=0))
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_REVPWR;
	}
	//��Nm�F���A�ڕW-���F���@�ˋt�]��
	else	// if(s4g_mtcnt_nmsm2<0 && s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2 > 0)
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_REVREG;
	}
}

void vdg_mtcnt_stagephasejdg()
{
	//�ی���Ԃ��烍�[�^�p�ɑ΂���e���o�͗L���𔻒f
	//���]�͍s�A�t�]�񐶂̂Ƃ��͑O�i�����ɐi�p������
	if((u1g_mtcnt_idorthantm1==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm1==ID_MTRUN_REVREG))
	{
		vdg_mtcnt_stagejdg(ID_MOTOR1,ID_MT_ADV);	//���ݓd�C�p����O�i�����֐i�p�����鑊�o�͐ݒ�
	}
	else
	{
		vdg_mtcnt_stagejdg(ID_MOTOR1,ID_MT_BACK);	//���ݓd�C�p�����i�����֐i�p�����鑊�o�͐ݒ�
	}

	if((u1g_mtcnt_idorthantm2==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm2==ID_MTRUN_REVREG))
	{
		vdg_mtcnt_stagejdg(ID_MOTOR2,ID_MT_ADV);	//���ݓd�C�p����O�i�����֐i�p�����鑊�o�͐ݒ�
	}
	else
	{
		vdg_mtcnt_stagejdg(ID_MOTOR2,ID_MT_BACK);	//���ݓd�C�p�����i�����֐i�p�����鑊�o�͐ݒ�
	}
}


//�͍s����
void vdg_mtcnt_tgrpwrcalm1()
{
	/***************static�ϐ���`***************/
	volatile static float f4s_mtcnt_dutym1ff;
	volatile static float f4s_mtcnt_dutym1fb;
	volatile static float f4s_mtcnt_dutym1fbilim;
	volatile static float f4s_mtcnt_dutym1;
	volatile static unsigned long u4s_mtcnt_nm1diffabs;

	/***************�e���|�����ϐ���`***************/
	volatile unsigned short u2s_mtcnt_cntm1pre;


	u4s_mtcnt_nm1diffabs = (unsigned long)(abs(s4g_rspicnt_nm1tgt - s4g_mtcnt_nmsm1));

	//FF���Z�o
	/***���[�^�ő��]��/���݉�]���~FF���p�ő�Duty***/
	f4s_mtcnt_dutym1ff = DUTY_MINFF + (DUTY_MAXFF - DUTY_MINFF) * (float)(s4g_mtcnt_nmsm1) / (float)(NM_MAX);

	//FB���Z�o
	f4s_mtcnt_dutym1fb = (float)(u4s_mtcnt_nm1diffabs) * KP_FB;
	//FB���㉺������
	if(f4s_mtcnt_dutym1fb >= DUTY_MAXFB)
	{
		f4s_mtcnt_dutym1fb = DUTY_MAXFB;
	}

	//�d�����ߎ�FB���Z�o
	if (f4g_current_imsm1 >= I_MAX)
	{
		f4s_mtcnt_dutym1fbilim = (I_MAX - f4g_current_imsm1) * KP_ILIM;
		if(f4s_mtcnt_dutym1fbilim < DUTY_MINILIM)
		{
			f4s_mtcnt_dutym1fbilim = DUTY_MINILIM;
		}
		else if(f4s_mtcnt_dutym1fbilim >= 0)
		{
			f4s_mtcnt_dutym1fbilim = 0;
		}
	}
	else
	{
		f4s_mtcnt_dutym1fbilim = 0;
	}

	//Duty���Z
	f4s_mtcnt_dutym1 = f4s_mtcnt_dutym1ff + f4s_mtcnt_dutym1fb + f4s_mtcnt_dutym1fbilim;

	//Duty�̏㉺���K�[�h�B�|�J���P
	if(f4s_mtcnt_dutym1 > DUTY_MAX) { f4s_mtcnt_dutym1 = DUTY_MAX; }
	else if(f4s_mtcnt_dutym1 < DUTY_MIN) { f4s_mtcnt_dutym1 = DUTY_MIN; }

	//�J�E���g���Z
	u2s_mtcnt_cntm1pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym1));
	//�J�E���g�㉺������
	//����K�[�h�͍ŏ�Duty�ɑ΂��鏈���B�t�ł��邱�Ƃɒ��ӁB
	if(u2s_mtcnt_cntm1pre > CNT_MAX)
	{
		u2g_mtcnt_cntm1 = CNT_MAX;
	}
	//�����K�[�h�͍ő�Duty�ɑ΂��鏈���B�t�ł��邱�Ƃɒ��ӁB
	else if(u2s_mtcnt_cntm1pre < CNT_MIN)
	{
		u2g_mtcnt_cntm1 = CNT_MIN;
	}
	else
	{
		u2g_mtcnt_cntm1 = u2s_mtcnt_cntm1pre;
	}
}

void vdg_mtcnt_tgrpwrcalm2()
{
	/***************static�ϐ���`***************/
	volatile static float f4s_mtcnt_dutym2ff;
	volatile static float f4s_mtcnt_dutym2fb;
	volatile static float f4s_mtcnt_dutym2fbilim;
	volatile static float f4s_mtcnt_dutym2;
	volatile static unsigned long u4s_mtcnt_nm2diffabs;

	/***************�e���|�����ϐ���`***************/
	volatile unsigned short u2s_mtcnt_cntm2pre;


	u4s_mtcnt_nm2diffabs = (unsigned long)(abs(s4g_rspicnt_nm2tgt - s4g_mtcnt_nmsm2));

	//FF���Z�o
	/***���[�^�ő��]��/���݉�]���~FF���p�ő�Duty***/
	f4s_mtcnt_dutym2ff = DUTY_MINFF + (DUTY_MAXFF - DUTY_MINFF) * (float)(s4g_mtcnt_nmsm2) / (float)(NM_MAX);

	//FB���Z�o
	f4s_mtcnt_dutym2fb = (float)(u4s_mtcnt_nm2diffabs) * KP_FB;
	//FB���㉺������
	if(f4s_mtcnt_dutym2fb >= DUTY_MAXFB)
	{
		f4s_mtcnt_dutym2fb = DUTY_MAXFB;
	}

	//�d�����ߎ�FB���Z�o
	if (f4g_current_imsm2 >= I_MAX)
	{
		f4s_mtcnt_dutym2fbilim = (I_MAX - f4g_current_imsm2) * KP_ILIM;
		if(f4s_mtcnt_dutym2fbilim < DUTY_MINILIM)
		{
			f4s_mtcnt_dutym2fbilim = DUTY_MINILIM;
		}
		else if(f4s_mtcnt_dutym2fbilim >= 0)
		{
			f4s_mtcnt_dutym2fbilim = 0;
		}
	}
	else
	{
		f4s_mtcnt_dutym2fbilim = 0;
	}

	//Duty���Z
	f4s_mtcnt_dutym2 = f4s_mtcnt_dutym2ff + f4s_mtcnt_dutym2fb + f4s_mtcnt_dutym2fbilim;

	//Duty�̏㉺���K�[�h�B�|�J���P
	if(f4s_mtcnt_dutym2 > DUTY_MAX) { f4s_mtcnt_dutym2 = DUTY_MAX; }
	else if(f4s_mtcnt_dutym2 < DUTY_MIN) { f4s_mtcnt_dutym2 = DUTY_MIN; }
	
	//�J�E���g���Z
	u2s_mtcnt_cntm2pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym2));
	//�J�E���g�㉺������
	if(u2s_mtcnt_cntm2pre > CNT_MAX)
	{
		u2g_mtcnt_cntm2 = CNT_MAX;
	}
	else if(u2s_mtcnt_cntm2pre < CNT_MIN)
	{
		u2g_mtcnt_cntm2 = CNT_MIN;
	}
	else
	{
		u2g_mtcnt_cntm2 = u2s_mtcnt_cntm2pre;
	}
}

//�񐶏���
void vdg_mtcnt_tgrregcalm1()
{
	/***************static�ϐ���`***************/
	volatile static float f4s_mtcnt_dutym1ff;
	volatile static float f4s_mtcnt_dutym1fb;
	volatile static float f4s_mtcnt_dutym1fbilim;
	volatile static float f4s_mtcnt_dutym1;
	volatile static unsigned long u4s_mtcnt_nm1diffabs;

	/***************�e���|�����ϐ���`***************/
	volatile unsigned short u2s_mtcnt_cntm1pre;


	u4s_mtcnt_nm1diffabs = (unsigned long)(abs(s4g_rspicnt_nm1tgt - s4g_mtcnt_nmsm1));

	//FF���Z�o
	/***���[�^�ő��]��/���݉�]���~FF���p�ő�Duty***/
	f4s_mtcnt_dutym1ff = DUTY_MIN + DUTY_MAXFFREG * (float)(s4g_mtcnt_nmsm1) / (float)(NM_MAX);

	//FB���Z�o
	f4s_mtcnt_dutym1fb = (float)(u4s_mtcnt_nm1diffabs) * KP_FB;
	//FB���㉺������
	if(f4s_mtcnt_dutym1fb >= DUTY_MAXFBREG)
	{
		f4s_mtcnt_dutym1fb = DUTY_MAXFBREG;
	}

	//�d�����ߎ�FB���Z�o
	if (f4g_current_imsm1 <= I_MIN)
	{
		f4s_mtcnt_dutym1fbilim = (f4g_current_imsm1 - I_MIN) * KP_ILIM;
		if(f4s_mtcnt_dutym1fbilim < DUTY_MINILIMREG)
		{
			f4s_mtcnt_dutym1fbilim = DUTY_MINILIMREG;
		}
		else if(f4s_mtcnt_dutym1fbilim >= 0)
		{
			f4s_mtcnt_dutym1fbilim = 0;
		}
	}
	else
	{
		f4s_mtcnt_dutym1fbilim = 0;
	}

	//Duty���Z
	f4s_mtcnt_dutym1 = f4s_mtcnt_dutym1ff + f4s_mtcnt_dutym1fb + f4s_mtcnt_dutym1fbilim;

	//�J�E���g���Z
	u2s_mtcnt_cntm1pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym1));
	//�J�E���g�㉺������
	if(u2s_mtcnt_cntm1pre > CNT_MAX)
	{
		u2g_mtcnt_cntm1 = CNT_MAX;
	}
	else if(u2s_mtcnt_cntm1pre < CNT_MIN)
	{
		u2g_mtcnt_cntm1 = CNT_MIN;
	}
	else
	{
		u2g_mtcnt_cntm1 = u2s_mtcnt_cntm1pre;
	}
}

void vdg_mtcnt_tgrregcalm2()
{
	/***************static�ϐ���`***************/
	volatile static float f4s_mtcnt_dutym2ff;
	volatile static float f4s_mtcnt_dutym2fb;
	volatile static float f4s_mtcnt_dutym2fbilim;
	volatile static float f4s_mtcnt_dutym2;
	volatile static unsigned long u4s_mtcnt_nm2diffabs;

	/***************�e���|�����ϐ���`***************/
	volatile unsigned short u2s_mtcnt_cntm2pre;


	u4s_mtcnt_nm2diffabs = (unsigned long)(abs(s4g_rspicnt_nm2tgt - s4g_mtcnt_nmsm2));

	//FF���Z�o
	/***���[�^�ő��]��/���݉�]���~FF���p�ő�Duty***/
	f4s_mtcnt_dutym2ff = DUTY_MIN + DUTY_MAXFFREG * (float)(s4g_mtcnt_nmsm2) / (float)(NM_MAX);

	//FB���Z�o
	f4s_mtcnt_dutym2fb = (float)(u4s_mtcnt_nm2diffabs) * KP_FB;
	//FB���㉺������
	if(f4s_mtcnt_dutym2fb >= DUTY_MAXFBREG)
	{
		f4s_mtcnt_dutym2fb = DUTY_MAXFBREG;
	}

	//�d�����ߎ�FB���Z�o
	if (f4g_current_imsm2 <= I_MIN)
	{
		f4s_mtcnt_dutym2fbilim = (f4g_current_imsm2 - I_MIN) * KP_ILIM;
		if(f4s_mtcnt_dutym2fbilim < DUTY_MINILIMREG)
		{
			f4s_mtcnt_dutym2fbilim = DUTY_MINILIMREG;
		}
		else if(f4s_mtcnt_dutym2fbilim >= 0)
		{
			f4s_mtcnt_dutym2fbilim = 0;
		}
	}
	else
	{
		f4s_mtcnt_dutym2fbilim = 0;
	}

	//Duty���Z
	f4s_mtcnt_dutym2 = f4s_mtcnt_dutym2ff + f4s_mtcnt_dutym2fb + f4s_mtcnt_dutym2fbilim;
	
	//�J�E���g���Z
	u2s_mtcnt_cntm2pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym2));
	//�J�E���g�㉺������
	if(u2s_mtcnt_cntm2pre > CNT_MAX)
	{
		u2g_mtcnt_cntm2 = CNT_MAX;
	}
	else if(u2s_mtcnt_cntm2pre < CNT_MIN)
	{
		u2g_mtcnt_cntm2 = CNT_MIN;
	}
	else
	{
		u2g_mtcnt_cntm2 = u2s_mtcnt_cntm2pre;
	}
}

void vdg_mtcnt_mtorigin()
{
	/***************static�ϐ���`***************/
	volatile static unsigned char u1s_mtcnt_cntoriginrot = 0;
	volatile static unsigned char u1s_mtcnt_idstage = 0;

	/***************�e���|�����ϐ���`***************/
	
	u1g_mtcnt_xnormal = 0;
	u1g_mtcnt_xmtorigin = 0;
	u1g_mtcnt_xstop = 0;
	while (u1s_mtcnt_cntoriginrot < 7)
	{
		u1s_mtcnt_cntoriginrot++;
		u1s_mtcnt_idstage++;

		vdg_mtcnt_outset(ID_MOTOR1, u1s_mtcnt_idstage, CNT_ORIGINDUTY);
		vdg_mtcnt_outset(ID_MOTOR2, u1s_mtcnt_idstage, CNT_ORIGINDUTY);

		if(u1s_mtcnt_idstage >= 6){u1s_mtcnt_idstage = 0;}

		//�X�e�[�W���ɋK�莞�ԑ҂�
		vdg_wait_nop(2000000);
	}

	u1g_mtcnt_xmtorigin = 1;
	u1g_mtcnt_xnormal = 1;

	//�G���R�[�_TCNT�J�E���g���u�J�E���^���_�{���_�w�K��p�x���J�E���g�v����n�߂邽�߂ɏ�������
	MTU_M1_ENCTCNT = TCNT_ENC_DEFAULT;
	MTU_M2_ENCTCNT = TCNT_ENC_DEFAULT;

	vdg_wait_nop(2000000);			//���_�w�K�������G���R�[�_�J�E���g�����������㏊�莞�ԑ҂�

	//���[�v�Ɉڍs����O�ɏo�͖͂����I��OFF�ɂ��Ă���
	vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
}



// void vdg_mtcnt_mtorigin()
// {
//     //
// 	volatile unsigned char i;
// 	volatile unsigned char j;
	
// 	volatile unsigned short u2t_mtorigin_cntv;
// 	volatile unsigned short dummy;
	
// 	volatile static unsigned long u4s_mtorigin_cntcycle = 0;
// 	volatile static unsigned char u1s_mtorigin_cntstage = 0;
// 	volatile static unsigned char u1s_mtorigin_cntrot = 0;

// 	//MTU.TOER.BYTE = 0xFF;		//�S���o�͋���
// 	//MTU.TSTR.BYTE = 0xC7;		//MTU0,1,2,3,4��TCNT�J�E���g�J�n
	
// 	u1g_mtcnt_xmtorgfin = 0;
// 	u2t_mtorigin_cntv = CNT_MTORIGIN;
// 	dummy = 1000;
	
// 	if(u4s_mtorigin_cntcycle > CNT_MTORG*6)
// 	{
// 		u4s_mtorigin_cntcycle = 0;
// 		u1s_mtorigin_cntrot++;
// 	}
	
// 	if(u1s_mtorigin_cntrot < POLE_PAIRS)
// 	{
// 		u4s_mtorigin_cntcycle++;

// 		//CNT_MTORG�~�L�����AT�b����Stage�؂�ւ��ĉ�]������
// 		if(u4s_mtorigin_cntcycle <= CNT_MTORG) 	{ u1s_mtorigin_cntstage = 1; }
// 		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*2) 	{ u1s_mtorigin_cntstage = 2; }
// 		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*3) 	{ u1s_mtorigin_cntstage = 3; }
// 		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*4) { u1s_mtorigin_cntstage = 4; }
// 		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*5) { u1s_mtorigin_cntstage = 5; }
// 		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*6) { u1s_mtorigin_cntstage = 6; }
		
// 		switch(u1s_mtorigin_cntstage)
// 		{
// 			case 1:
// 			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
// 			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
// 			PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)��MTIOC4B
// 			PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)��MTIOC4D
// 			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
// 			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
// 			MTU.TOER.BYTE = STAGE1;
// 			MTU3.TGRD = u2t_mtorigin_cntv;
// 			MTU4.TGRC = 0;
// 			MTU4.TGRD = dummy;		//�_�~�[��ݒ�
// 			break;
		
// 			case 2:
// 			PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)��MTIOC4C
// 			PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)��MTIOC4A
// 			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
// 			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
// 			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
// 			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
// 			MTU.TOER.BYTE = STAGE2;
// 			MTU3.TGRD = u2t_mtorigin_cntv;
// 			MTU4.TGRD = 0;
// 			break;
		
// 			case 3:
// 			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
// 			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
// 			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
// 			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
// 			PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)��MTIOC3D
// 			PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)��MTIOC3B
// 			MTU.TOER.BYTE = STAGE3;
// 			MTU4.TGRC = u2t_mtorigin_cntv;
// 			MTU4.TGRD = 0;
// 			break;
			
// 			case 4:
// 			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
// 			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
// 			PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)��MTIOC4B
// 			PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)��MTIOC4D
// 			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
// 			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
// 			MTU.TOER.BYTE = STAGE4;
// 			MTU3.TGRD = 0;
// 			MTU4.TGRC = u2t_mtorigin_cntv;
// 			MTU4.TGRD = dummy;		//�_�~�[��ݒ�
// 			break;
			
// 			case 5:
// 			PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)��MTIOC4C
// 			PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)��MTIOC4A
// 			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
// 			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
// 			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
// 			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
// 			MTU.TOER.BYTE = STAGE5;
// 			MTU3.TGRD = 0;
// 			MTU4.TGRD = u2t_mtorigin_cntv;
// 			break;
			
// 			case 6:
// 			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
// 			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
// 			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
// 			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
// 			PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)��MTIOC3D
// 			PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)��MTIOC3B
// 			MTU.TOER.BYTE = STAGE6;
// 			MTU4.TGRC = 0;
// 			MTU4.TGRD = u2t_mtorigin_cntv;
// 			break;
// 		}
// 	}
	
// 	else
// 	{
// 		SCI12.TDR = 'Z';		//���_�w�K�I�����}�X�^�[�ɑ��M
		
// 		/***Stage1�Ńz�[���h***/
// 		PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
// 		PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
// 		PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)��MTIOC4B
// 		PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)��MTIOC4D
// 		PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
// 		PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
// 		MTU.TOER.BYTE = STAGE1;
// 		MTU3.TGRD = u2t_mtorigin_cntv;
// 		MTU4.TGRC = 0;
// 		MTU4.TGRD = dummy;		//�_�~�[��ݒ�
		
// 		u4s_mtorigin_cntcycle = 0;
// 		u1s_mtorigin_cntstage = 0;
// 		u1s_mtorigin_cntrot = 0;
		
// 		u1g_mtcnt_xmtorgfin = 1;		//�����݂ł̉�]���Z�o������
// 		u1g_rxi12_xmtorgrq = 0;		//���_�w�K�v�������Z�b�g
// 	}

// 	//�G���R�[�_TCNT�J�E���g���u���_+���_�w�K��p�x���J�E���g�v����n�߂邽�߂ɏ�������
// 	MTU1.TCNT = TCNT_ENC_DEFAULT;
// 	MTU2.TCNT = TCNT_ENC_DEFAULT;
// }



// void vdg_mtcnt_tgrset()
// {
// 	/***************static�ϐ���`***************/
// 	volatile static unsigned short u2s_mtcnt_cntu;
// 	volatile static unsigned short u2s_mtcnt_cntv;
// 	volatile static unsigned short u2s_mtcnt_cntw;

// 	/***************�e���|�����ϐ���`***************/
// 	volatile unsigned short u2t_mtcnt_cntel360;
// 	volatile float f4t_mtcnt_delangle;
// 	volatile float f4t_mtcnt_duty;
// 	volatile float f4t_mtcnt_cnt;
// 	volatile float f4t_mtcnt_imerror;
	
// 	volatile float f4t_mtcnt_cntmid;
// 	volatile float f4t_mtcnt_cntu;
// 	volatile float f4t_mtcnt_cntv;
// 	volatile float f4t_mtcnt_cntw;
	
// 	volatile static float f4s_mtcnt_elanglez = 0;
// 	volatile static float f4s_mtcnt_elangle = 0;
// 	volatile static float f4s_mtcnt_dutyimp = 0;
	
// 	//f4g_mtcnt_nmtgt = (float)s2g_rxi12_nmtgt;					//���b�`
	
// 	f4g_mtcnt_nmtgt = f4g_mtcnt_nmtgt + 0.1;	//�������f�o�b�O�p������
	
// 	if(f4g_mtcnt_nmtgt > 1000){ f4g_mtcnt_nmtgt = 1000;}
	
// 	/***�ڕW�d�C�p�Z�o***/
// 	f4t_mtcnt_delangle = f4g_mtcnt_nmtgt * T_CARRIER * 6 * POLE_PAIRS;		//[rpm]�~2��/60�~t�~360/2�� [deg] �� [rpm]�~6�~t [deg]
// 	f4s_mtcnt_elangle = f4s_mtcnt_elanglez + f4t_mtcnt_delangle;			//�O��l�d�C�p+����^�X�N���d�C�p
// 	u2t_mtcnt_cntel360 = f4s_mtcnt_elangle / 360;					//�d�C�p1�������������ǂ���
// 	f4s_mtcnt_elangle = f4s_mtcnt_elangle - (float)u2t_mtcnt_cntel360 * 360;	//�d�C�p1�����Ɏ��߂鏈��
// 	f4s_mtcnt_elanglez = f4s_mtcnt_elangle;						//�O��l�ێ�
	
// 	if(f4g_mtcnt_nmtgt != 0)
// 	{
		
// 		vdg_mtcnt_outset(ID_MOTOR1,ID_ALLSET);	//�����g���䂷�邽�ߑS��MTU�֊���
// 		MTU.TOER.BYTE = 0xFF;		//�S���o�͋���
		
// 		/***�ߓd���̏ꍇ�͓d���}������FB������***/
// 		if(f4g_current_imsm1 >= MAX_I)
// 		{
// 			f4t_mtcnt_imerror = f4g_current_imsm1 - MAX_I;
// 		}
// 		else
// 		{
// 			f4t_mtcnt_imerror = 0;
// 		}
		
// 		f4s_mtcnt_dutyimp = f4t_mtcnt_imerror * KP_ILIM_OL;
		
// 		/***�ڕWDuty�Z�o***/
// 		f4t_mtcnt_duty = DUTY_MIN_OL + f4g_mtcnt_nmtgt * KP_OL + f4s_mtcnt_dutyimp;		//Duty��Z�o
		
// 		f4t_mtcnt_duty = DUTY_MIN_OL;	//�������f�o�b�O�p������
		
// 		/***�ߓd�������l�������㉺�������B�������͏���l�ȉ��ɂ��Ȃ�***/
// 		if(f4t_mtcnt_duty < DUTY_MIN_ILIM_OL) { f4t_mtcnt_duty = DUTY_MIN_ILIM_OL; }
// 		else if(f4t_mtcnt_duty > DUTY_MAX) { f4t_mtcnt_duty = DUTY_MAX; }	

		
// 		/***�ڕW�J�E���g���Z�o***/
// 		f4t_mtcnt_cnt = (float)CNT_HALF_CARR * f4t_mtcnt_duty + (float)CNT_DT;		//Duty�䂩��J�E���g���Z�o
// 		f4t_mtcnt_cntmid = f4t_mtcnt_cnt / 2;
		
// 		/***�e�����Ƃɐ����g�J�E���g���Z�o***/
// 		f4t_mtcnt_cntu = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle+ADV_ELANGLE) * KDEG2RAD);
// 		f4t_mtcnt_cntv = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle-120+ADV_ELANGLE) * KDEG2RAD);
// 		f4t_mtcnt_cntw = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle-240+ADV_ELANGLE) * KDEG2RAD);
		
		
// 		//�㉺���K�[�h�������^�ɕϊ�
// 		if(f4t_mtcnt_cntu > CNT_MAX) { u2s_mtcnt_cntu = (unsigned short)CNT_MAX; }
// 		else if(f4t_mtcnt_cntu < CNT_MIN) { u2s_mtcnt_cntu = 0; }
// 		else { u2s_mtcnt_cntu = (unsigned short)f4t_mtcnt_cntu; }
		
// 		if(f4t_mtcnt_cntv > CNT_MAX) { u2s_mtcnt_cntv = (unsigned short)CNT_MAX; }
// 		else if(f4t_mtcnt_cntv < CNT_MIN) { u2s_mtcnt_cntv = 0; }
// 		else { u2s_mtcnt_cntv = (unsigned short)f4t_mtcnt_cntv; }
		
// 		if(f4t_mtcnt_cntw > CNT_MAX) { u2s_mtcnt_cntw = (unsigned short)CNT_MAX; }
// 		else if(f4t_mtcnt_cntw < CNT_MIN) { u2s_mtcnt_cntw = 0; }
// 		else { u2s_mtcnt_cntw = (unsigned short)f4t_mtcnt_cntw; }
		
// 		MTU3.TGRD = u2s_mtcnt_cntu;
// 		MTU4.TGRC = u2s_mtcnt_cntv;
// 		MTU4.TGRD = u2s_mtcnt_cntw;
// 	}
	
// 	/***nmtgt = 0�̂Ƃ��͕ʂŏ�������***/
// 	else
// 	{
		
// 	}
// }
