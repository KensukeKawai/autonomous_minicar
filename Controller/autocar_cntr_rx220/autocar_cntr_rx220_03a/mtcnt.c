
#include "common.h"

/***************グローバル変数定義***************/
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

//モータ回転数のLPF処理
void vdg_mtcnt_nmsmcal()
{
	/**********static変数定義**********/
	volatile static float f4s_mtcnt_nm1z = 0;
	volatile static float f4s_mtcnt_nmsm1z = 0;
	volatile static float f4s_mtcnt_nm2z = 0;
	volatile static float f4s_mtcnt_nmsm2z = 0;

	/**********テンポラリ変数定義**********/
	volatile float f4t_mtcnt_nmsm1;
	volatile float f4t_mtcnt_nmsm2;

	//1次LPF処理
	f4t_mtcnt_nmsm1 = LPF_K1*((float)(s4g_mtsensor_nm1)+f4s_mtcnt_nm1z)-LPF_K2*f4s_mtcnt_nmsm1z;
	f4t_mtcnt_nmsm2 = LPF_K1*((float)(s4g_mtsensor_nm2)+f4s_mtcnt_nm2z)-LPF_K2*f4s_mtcnt_nmsm2z;

	//前回値ラッチ処理
	f4s_mtcnt_nm1z = (float)(s4g_mtsensor_nm1);
	f4s_mtcnt_nm2z = (float)(s4g_mtsensor_nm2);
	f4s_mtcnt_nmsm1z = f4t_mtcnt_nmsm1;
	f4s_mtcnt_nmsm2z = f4t_mtcnt_nmsm2;
	s4g_mtcnt_nmsm1 = (signed long)(f4t_mtcnt_nmsm1);
	s4g_mtcnt_nmsm2 = (signed long)(f4t_mtcnt_nmsm2);
}

//現在のモータ電気角から指定モータに指定方向へ進角ステージID判定
void vdg_mtcnt_stagejdg(unsigned char motor, unsigned char id_direction)
{
	switch(motor)
	{
		//Motor1
		case ID_MOTOR1:
		switch(id_direction)
		{
			//前進方向に進角
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
			//後進方向に進角
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
			//前進方向に進角
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
			//後進方向に進角
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

//APIでのOFF出力がGNDにならず浮いているため、PMRを汎用出力LOWにする
void vdg_mtcnt_outset(unsigned char motor, unsigned char mode, unsigned short cntduty)
{

//前周期で設定されたdutyカウントと直前で判定したステージモードから出力

	switch(motor)
	{
		case ID_MOTOR1:
		switch(mode)
		{
			case ID_STAGE1:		//UH ⇒ VL
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

			case ID_STAGE2:		//UH ⇒ WL
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
			
			case ID_STAGE3:		//VH ⇒ WL
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
			
			case ID_STAGE4:		//VH ⇒ UL
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
			
			case ID_STAGE5:		//WH ⇒ UL
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

			case ID_STAGE6:		//WH ⇒ VL
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
			case ID_STAGE1:		//UH ⇒ VL
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

			case ID_STAGE2:		//UH ⇒ WL
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
			
			case ID_STAGE3:		//VH ⇒ WL
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
			
			case ID_STAGE4:		//VH ⇒ UL
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
			
			case ID_STAGE5:		//WH ⇒ UL
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

			case ID_STAGE6:		//WH ⇒ VL
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

//力行、回生判定
void vdg_mtcnt_orthantjdg()
{
	//Motor1
	//実Nm：正、目標-実：正　⇒正転力行
	if((s4g_mtcnt_nmsm1>=0) && (s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1>=0))
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_FWDPWR;
	}
	//実Nm：正、目標-実：負　⇒正転回生
	else if((s4g_mtcnt_nmsm1>=0) && (s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1<0))
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_FWDREG;
	}
	//実Nm：負、目標-実：負　⇒逆転力行
	else if((s4g_mtcnt_nmsm1<0) && (s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1<=0))
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_REVPWR;
	}
	//実Nm：負、目標-実：正　⇒逆転回生
	else	// if(s4g_mtcnt_nmsm1<0 && s4g_rspicnt_nm1tgt-s4g_mtcnt_nmsm1 > 0)
	{
		u1g_mtcnt_idorthantm1 = ID_MTRUN_REVREG;
	}

	//Motor2
	//実Nm：正、目標-実：正　⇒正転力行
	if((s4g_mtcnt_nmsm2>=0) && (s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2>=0))
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_FWDPWR;
	}
	//実Nm：正、目標-実：負　⇒正転回生
	else if((s4g_mtcnt_nmsm2>=0) && (s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2<0))
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_FWDREG;
	}
	//実Nm：負、目標-実：負　⇒逆転力行
	else if((s4g_mtcnt_nmsm2<0) && (s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2<=0))
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_REVPWR;
	}
	//実Nm：負、目標-実：正　⇒逆転回生
	else	// if(s4g_mtcnt_nmsm2<0 && s4g_rspicnt_nm2tgt-s4g_mtcnt_nmsm2 > 0)
	{
		u1g_mtcnt_idorthantm2 = ID_MTRUN_REVREG;
	}
}

void vdg_mtcnt_stagephasejdg()
{
	//象限状態からロータ角に対する各相出力有無を判断
	//正転力行、逆転回生のときは前進方向に進角させる
	if((u1g_mtcnt_idorthantm1==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm1==ID_MTRUN_REVREG))
	{
		vdg_mtcnt_stagejdg(ID_MOTOR1,ID_MT_ADV);	//現在電気角から前進方向へ進角させる相出力設定
	}
	else
	{
		vdg_mtcnt_stagejdg(ID_MOTOR1,ID_MT_BACK);	//現在電気角から後進方向へ進角させる相出力設定
	}

	if((u1g_mtcnt_idorthantm2==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm2==ID_MTRUN_REVREG))
	{
		vdg_mtcnt_stagejdg(ID_MOTOR2,ID_MT_ADV);	//現在電気角から前進方向へ進角させる相出力設定
	}
	else
	{
		vdg_mtcnt_stagejdg(ID_MOTOR2,ID_MT_BACK);	//現在電気角から後進方向へ進角させる相出力設定
	}
}


//力行処理
void vdg_mtcnt_tgrpwrcalm1()
{
	/***************static変数定義***************/
	volatile static float f4s_mtcnt_dutym1ff;
	volatile static float f4s_mtcnt_dutym1fb;
	volatile static float f4s_mtcnt_dutym1fbilim;
	volatile static float f4s_mtcnt_dutym1;
	volatile static unsigned long u4s_mtcnt_nm1diffabs;

	/***************テンポラリ変数定義***************/
	volatile unsigned short u2s_mtcnt_cntm1pre;


	u4s_mtcnt_nm1diffabs = (unsigned long)(abs(s4g_rspicnt_nm1tgt - s4g_mtcnt_nmsm1));

	//FF項算出
	/***モータ最大回転数/現在回転数×FF項用最大Duty***/
	f4s_mtcnt_dutym1ff = DUTY_MINFF + (DUTY_MAXFF - DUTY_MINFF) * (float)(s4g_mtcnt_nmsm1) / (float)(NM_MAX);

	//FB項算出
	f4s_mtcnt_dutym1fb = (float)(u4s_mtcnt_nm1diffabs) * KP_FB;
	//FB項上下限処理
	if(f4s_mtcnt_dutym1fb >= DUTY_MAXFB)
	{
		f4s_mtcnt_dutym1fb = DUTY_MAXFB;
	}

	//電流超過時FB項算出
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

	//Duty合算
	f4s_mtcnt_dutym1 = f4s_mtcnt_dutym1ff + f4s_mtcnt_dutym1fb + f4s_mtcnt_dutym1fbilim;

	//Dutyの上下限ガード。ポカヨケ
	if(f4s_mtcnt_dutym1 > DUTY_MAX) { f4s_mtcnt_dutym1 = DUTY_MAX; }
	else if(f4s_mtcnt_dutym1 < DUTY_MIN) { f4s_mtcnt_dutym1 = DUTY_MIN; }

	//カウント換算
	u2s_mtcnt_cntm1pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym1));
	u2g_mtcnt_cntm1 = u2s_mtcnt_cntm1pre;
}

void vdg_mtcnt_tgrpwrcalm2()
{
	/***************static変数定義***************/
	volatile static float f4s_mtcnt_dutym2ff;
	volatile static float f4s_mtcnt_dutym2fb;
	volatile static float f4s_mtcnt_dutym2fbilim;
	volatile static float f4s_mtcnt_dutym2;
	volatile static unsigned long u4s_mtcnt_nm2diffabs;

	/***************テンポラリ変数定義***************/
	volatile unsigned short u2s_mtcnt_cntm2pre;


	u4s_mtcnt_nm2diffabs = (unsigned long)(abs(s4g_rspicnt_nm2tgt - s4g_mtcnt_nmsm2));

	//FF項算出
	/***モータ最大回転数/現在回転数×FF項用最大Duty***/
	f4s_mtcnt_dutym2ff = DUTY_MINFF + (DUTY_MAXFF - DUTY_MINFF) * (float)(s4g_mtcnt_nmsm2) / (float)(NM_MAX);

	//FB項算出
	f4s_mtcnt_dutym2fb = (float)(u4s_mtcnt_nm2diffabs) * KP_FB;
	//FB項上下限処理
	if(f4s_mtcnt_dutym2fb >= DUTY_MAXFB)
	{
		f4s_mtcnt_dutym2fb = DUTY_MAXFB;
	}

	//電流超過時FB項算出
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

	//Duty合算
	f4s_mtcnt_dutym2 = f4s_mtcnt_dutym2ff + f4s_mtcnt_dutym2fb + f4s_mtcnt_dutym2fbilim;

	//Dutyの上下限ガード。ポカヨケ
	if(f4s_mtcnt_dutym2 > DUTY_MAX) { f4s_mtcnt_dutym2 = DUTY_MAX; }
	else if(f4s_mtcnt_dutym2 < DUTY_MIN) { f4s_mtcnt_dutym2 = DUTY_MIN; }
	
	//カウント換算
	u2s_mtcnt_cntm2pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym2));
	u2g_mtcnt_cntm2 = u2s_mtcnt_cntm2pre;
}

//回生処理
void vdg_mtcnt_tgrregcalm1()
{
	/***************static変数定義***************/
	volatile static float f4s_mtcnt_dutym1ff;
	volatile static float f4s_mtcnt_dutym1fb;
	volatile static float f4s_mtcnt_dutym1fbilim;
	volatile static float f4s_mtcnt_dutym1;
	volatile static unsigned long u4s_mtcnt_nm1diffabs;

	/***************テンポラリ変数定義***************/
	volatile unsigned short u2s_mtcnt_cntm1pre;


	u4s_mtcnt_nm1diffabs = (unsigned long)(abs(s4g_rspicnt_nm1tgt - s4g_mtcnt_nmsm1));

	//FF項算出
	/***モータ最大回転数/現在回転数×FF項用最大Duty***/
	f4s_mtcnt_dutym1ff = DUTY_MINFFREG + (DUTY_MAXFFREG - DUTY_MINFFREG) * (float)(s4g_mtcnt_nmsm1) / (float)(NM_MAX);

	//FB項算出
	f4s_mtcnt_dutym1fb = (float)(u4s_mtcnt_nm1diffabs) * KP_FB;
	//FB項上下限処理
	if(f4s_mtcnt_dutym1fb >= DUTY_MAXFBREG)
	{
		f4s_mtcnt_dutym1fb = DUTY_MAXFBREG;
	}

	//電流超過時FB項算出
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

	//Duty合算
	f4s_mtcnt_dutym1 = f4s_mtcnt_dutym1ff + f4s_mtcnt_dutym1fb + f4s_mtcnt_dutym1fbilim;

	//Dutyの上下限ガード。ポカヨケ
	if(f4s_mtcnt_dutym1 > DUTY_MAXREG) { f4s_mtcnt_dutym1 = DUTY_MAXREG; }
	else if(f4s_mtcnt_dutym1 < DUTY_MINREG) { f4s_mtcnt_dutym1 = DUTY_MINREG; }

	//カウント換算
	u2s_mtcnt_cntm1pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym1));
	u2g_mtcnt_cntm1 = u2s_mtcnt_cntm1pre;
}

void vdg_mtcnt_tgrregcalm2()
{
	/***************static変数定義***************/
	volatile static float f4s_mtcnt_dutym2ff;
	volatile static float f4s_mtcnt_dutym2fb;
	volatile static float f4s_mtcnt_dutym2fbilim;
	volatile static float f4s_mtcnt_dutym2;
	volatile static unsigned long u4s_mtcnt_nm2diffabs;

	/***************テンポラリ変数定義***************/
	volatile unsigned short u2s_mtcnt_cntm2pre;


	u4s_mtcnt_nm2diffabs = (unsigned long)(abs(s4g_rspicnt_nm2tgt - s4g_mtcnt_nmsm2));

	//FF項算出
	/***モータ最大回転数/現在回転数×FF項用最大Duty***/
	f4s_mtcnt_dutym2ff = DUTY_MINFFREG + (DUTY_MAXFFREG - DUTY_MINFFREG) * (float)(s4g_mtcnt_nmsm2) / (float)(NM_MAX);

	//FB項算出
	f4s_mtcnt_dutym2fb = (float)(u4s_mtcnt_nm2diffabs) * KP_FB;
	//FB項上下限処理
	if(f4s_mtcnt_dutym2fb >= DUTY_MAXFBREG)
	{
		f4s_mtcnt_dutym2fb = DUTY_MAXFBREG;
	}

	//電流超過時FB項算出
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

	//Duty合算
	f4s_mtcnt_dutym2 = f4s_mtcnt_dutym2ff + f4s_mtcnt_dutym2fb + f4s_mtcnt_dutym2fbilim;

	//Dutyの上下限ガード。ポカヨケ
	if(f4s_mtcnt_dutym2 > DUTY_MAXREG) { f4s_mtcnt_dutym2 = DUTY_MAXREG; }
	else if(f4s_mtcnt_dutym2 < DUTY_MINREG) { f4s_mtcnt_dutym2 = DUTY_MINREG; }

	//カウント換算
	u2s_mtcnt_cntm2pre = (unsigned short)((float)(CNT_CARRIER) * (1 - f4s_mtcnt_dutym2));
	u2g_mtcnt_cntm2 = u2s_mtcnt_cntm2pre;
}

void vdg_mtcnt_mtorigin()
{
	/***************static変数定義***************/
	volatile static unsigned char u1s_mtcnt_cntoriginrot = 0;
	volatile static unsigned char u1s_mtcnt_idstage = 0;

	/***************テンポラリ変数定義***************/
	
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

		//ステージ毎に規定時間待つ
		vdg_wait_nop(2000000);
	}

	u1g_mtcnt_xmtorigin = 1;
	u1g_mtcnt_xnormal = 1;

	//エンコーダTCNTカウントを「カウンタ中点＋原点学習後角度分カウント」から始めるために書き込み
	MTU_M1_ENCTCNT = TCNT_ENC_DEFAULT;
	MTU_M2_ENCTCNT = TCNT_ENC_DEFAULT;

	vdg_wait_nop(2000000);			//原点学習処理しエンコーダカウント初期化完了後所定時間待ち

	//ループに移行する前に出力は明示的にOFFにしておく
	vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
}