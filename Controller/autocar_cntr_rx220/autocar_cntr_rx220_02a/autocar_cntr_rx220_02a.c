
#include "iodefine.h"
#include "vect.h"
#include "mathf.h"
#include "machine.h"
#include "global_data.h"
#include "api_init.h"
#include "excep.h"

/**********グローバル変数定義**********/

/***電流センサADC関係RAM定義***/
volatile unsigned short u2g_s12adio_result;
volatile unsigned char u1g_s12adio_xadcex;

volatile float f4g_current_imsm;

/***回転数関係RAM定義***/
volatile float f4g_tgib2_nmsm;
volatile unsigned short u2g_tgib2_citrpt;
volatile unsigned char u1g_tgib2_xzero;
volatile float f4g_tgib2_elangle;
volatile float f4g_tgib2_mcangle;

volatile unsigned char u1g_tciv2_xovflw;
volatile unsigned char u1g_tciv2_xdnmrst;

volatile float f4g_mtcnt_nmsm;
volatile float f4g_mtcnt_vtgt;
volatile unsigned char u1g_mtcnt_xrgmode;
volatile static float f4g_mtcnt_nmtgt;


volatile unsigned char u1g_rxi12_xmtorgrq;
volatile unsigned char u1g_rxi12_xrcnmt;
volatile signed short s2g_rxi12_nmtgt;
volatile unsigned char u1g_rxi12_nmtary[4] = {0};

volatile unsigned char u1g_mtorigin_xmtorgfin;

/**********グローバル変数定義**********/

void vdg_wait_nop(unsigned long N)
{
	unsigned long i;
	for(i=0;i<N;i++){ nop();}
}

void vdg_variable_init()
{
	
}

void vdg_current_cal()
{
	volatile unsigned short i;
	volatile float f4t_current_vout;
	volatile float f4t_current_imadd = 0;
	volatile static float f4s_current_imary[ADC_AVE_NUM] = {0};	//全て初期化
	
	f4t_current_vout = (float)u2g_s12adio_result * ADC_CNT2V;
	
	for( i = 1; i < ADC_AVE_NUM; i++ )
	{
		f4s_current_imary[ADC_AVE_NUM-i] = f4s_current_imary[ADC_AVE_NUM-i-1];
	}
	
	f4s_current_imary[0] = (f4t_current_vout - ACS724_VMED) * ACS724_IRATE;	//(ADC電圧-2.5V)*5
	
	for( i = 0; i < ADC_AVE_NUM; i++ )
	{
		f4t_current_imadd += f4s_current_imary[i];
	}
	f4g_current_imsm = f4t_current_imadd / (float)ADC_AVE_NUM;
}

//APIでのOFF出力がGNDにならず浮いているため、PMRを汎用出力LOWにする
void vdg_mtioc_outset(unsigned char mode)
{
	switch(mode)
	{
		case ID_ALLSET:
		PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)MTIOC4C
		PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)MTIOC4A
		PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)MTIOC4B
		PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)MTIOC4D
		PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)MTIOC3D
		PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)MTIOC3B
		break;
		
		case ID_HIGHOFF:
		PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)MTIOC4CをOFF
		PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)MTIOC4A
		PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)MTIOC4B
		PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)MTIOC4DをOFF
		PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)MTIOC3DをOFF
		PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)MTIOC3B
		break;
		
		case ID_LOWOFF:
		PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)MTIOC4C
		PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)MTIOC4AをOFF
		PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)MTIOC4BをOFF
		PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)MTIOC4D
		PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)MTIOC3D
		PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)MTIOC3BをOFF
		break;
		
		case ID_ALLOFF:
		PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)MTIOC4CをOFF
		PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)MTIOC4AをOFF
		PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)MTIOC4BをOFF
		PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)MTIOC4DをOFF
		PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)MTIOC3DをOFF
		PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)MTIOC3BをOFF
		break;
	}
}

//void vdg_nmtgt()
//{
//	if(u1g_rxi12_xrcnmt == 1)
//	{
//		u1g_rxi12_xrcnmt = 0;
		
//	}
//}

void vdg_mtcnt()
{
	volatile unsigned short u2t_mtcnt_cntel360;
	volatile float f4t_mtcnt_delangle;
	volatile float f4t_mtcnt_duty;
	volatile float f4t_mtcnt_cnt;
	volatile float f4t_mtcnt_imerror;
	
	volatile float f4t_mtcnt_cntmid;
	volatile float f4t_mtcnt_cntu;
	volatile float f4t_mtcnt_cntv;
	volatile float f4t_mtcnt_cntw;
	
	volatile static float f4s_mtcnt_elanglez = 0;
	volatile static float f4s_mtcnt_elangle = 0;
	volatile static float f4s_mtcnt_dutyimp = 0;
	
	volatile static unsigned short u2s_mtcnt_cntu;
	volatile static unsigned short u2s_mtcnt_cntv;
	volatile static unsigned short u2s_mtcnt_cntw;
	
	
	//f4g_mtcnt_nmtgt = (float)s2g_rxi12_nmtgt;					//ラッチ
	
	f4g_mtcnt_nmtgt = f4g_mtcnt_nmtgt + 0.1;	//★★★デバッグ用★★★
	
	if(f4g_mtcnt_nmtgt > 1000){ f4g_mtcnt_nmtgt = 1000;}
	
	/***目標電気角算出***/
	f4t_mtcnt_delangle = f4g_mtcnt_nmtgt * CARRIER_T * 6 * POLE_PAIRS;		//[rpm]×2π/60×t×360/2π [deg] → [rpm]×6×t [deg]
	f4s_mtcnt_elangle = f4s_mtcnt_elanglez + f4t_mtcnt_delangle;			//前回値電気角+今回タスク分電気角
	u2t_mtcnt_cntel360 = f4s_mtcnt_elangle / 360;					//電気角1周期超えたかどうか
	f4s_mtcnt_elangle = f4s_mtcnt_elangle - (float)u2t_mtcnt_cntel360 * 360;	//電気角1周期に収める処理
	f4s_mtcnt_elanglez = f4s_mtcnt_elangle;						//前回値保持
	
	if(f4g_mtcnt_nmtgt != 0)
	{
		
		vdg_mtioc_outset(ID_ALLSET);	//正弦波制御するため全相MTUへ割当
		MTU.TOER.BYTE = 0xFF;		//全相出力許可
		
		/***過電流の場合は電圧抑制分のFB項準備***/
		if(f4g_current_imsm >= MAX_I)
		{
			f4t_mtcnt_imerror = f4g_current_imsm - MAX_I;
		}
		else
		{
			f4t_mtcnt_imerror = 0;
		}
		
		f4s_mtcnt_dutyimp = f4t_mtcnt_imerror * KP_ILIM_OL;
		
		/***目標Duty算出***/
		f4t_mtcnt_duty = DUTY_MIN_OL + f4g_mtcnt_nmtgt * KP_OL + f4s_mtcnt_dutyimp;		//Duty比算出
		
		f4t_mtcnt_duty = DUTY_MIN_OL;	//★★★デバッグ用★★★
		
		/***過電流時も考慮した上下限処理。下限側は所定値以下にしない***/
		if(f4t_mtcnt_duty < DUTY_MIN_ILIM_OL) { f4t_mtcnt_duty = DUTY_MIN_ILIM_OL; }
		else if(f4t_mtcnt_duty > DUTY_MAX) { f4t_mtcnt_duty = DUTY_MAX; }	

		
		/***目標カウント数算出***/
		f4t_mtcnt_cnt = (float)CNT_HALF_CARR * f4t_mtcnt_duty + (float)CNT_DT;		//Duty比からカウント数算出
		f4t_mtcnt_cntmid = f4t_mtcnt_cnt / 2;
		
		/***各相ごとに正弦波カウント数算出***/
		f4t_mtcnt_cntu = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle+ADV_ELANGLE) * KDEG2RAD);
		f4t_mtcnt_cntv = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle-120+ADV_ELANGLE) * KDEG2RAD);
		f4t_mtcnt_cntw = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle-240+ADV_ELANGLE) * KDEG2RAD);
		
		
		//上下限ガードし整数型に変換
		if(f4t_mtcnt_cntu > CNT_MAX) { u2s_mtcnt_cntu = (unsigned short)CNT_MAX; }
		else if(f4t_mtcnt_cntu < CNT_MIN) { u2s_mtcnt_cntu = 0; }
		else { u2s_mtcnt_cntu = (unsigned short)f4t_mtcnt_cntu; }
		
		if(f4t_mtcnt_cntv > CNT_MAX) { u2s_mtcnt_cntv = (unsigned short)CNT_MAX; }
		else if(f4t_mtcnt_cntv < CNT_MIN) { u2s_mtcnt_cntv = 0; }
		else { u2s_mtcnt_cntv = (unsigned short)f4t_mtcnt_cntv; }
		
		if(f4t_mtcnt_cntw > CNT_MAX) { u2s_mtcnt_cntw = (unsigned short)CNT_MAX; }
		else if(f4t_mtcnt_cntw < CNT_MIN) { u2s_mtcnt_cntw = 0; }
		else { u2s_mtcnt_cntw = (unsigned short)f4t_mtcnt_cntw; }
		
		MTU3.TGRD = u2s_mtcnt_cntu;
		MTU4.TGRC = u2s_mtcnt_cntv;
		MTU4.TGRD = u2s_mtcnt_cntw;
	}
	
	/***nmtgt = 0のときは別で処理する***/
	else
	{
		
	}
}

void vdg_scisend(unsigned short senddata)
{
	switch(senddata)
	{
		case 0: SCI1.TDR = '0'; break;
		case 1: SCI1.TDR = '1'; break;
		case 2: SCI1.TDR = '2'; break;
		case 3: SCI1.TDR = '3'; break;
		case 4: SCI1.TDR = '4'; break;
		case 5: SCI1.TDR = '5'; break;
		case 6: SCI1.TDR = '6'; break;
		case 7: SCI1.TDR = '7'; break;
		case 8: SCI1.TDR = '8'; break;
		case 9: SCI1.TDR = '9'; break;
	}
}


/***PCへシリアル通信で送信するための処理***/
void vdg_sciset()
{	
	volatile unsigned char i;
	
	volatile unsigned short u2t_sciset_vtabs;
	volatile unsigned short u2t_sciset_nmtabs;
	volatile unsigned short u2t_sciset_nmabs;
	
	volatile static unsigned short u1s_sciset_sendnum;
	
	volatile static float f4s_sciset_vtgt;
	volatile static signed short s2s_sciset_nmtgt;
	volatile static float f4s_sciset_nmsm;
	volatile static unsigned short u2s_sciset_stringarray[10] = {0};

	switch(u1s_sciset_sendnum)
	{
		case 0:
		f4s_sciset_vtgt = f4g_mtcnt_vtgt;
		s2s_sciset_nmtgt = s2g_rxi12_nmtgt;
		f4s_sciset_nmsm = f4g_mtcnt_nmsm;
		
		u2t_sciset_vtabs = (unsigned short)abs((int)(f4s_sciset_vtgt*10));
		u2t_sciset_nmtabs = (unsigned short)abs(s2s_sciset_nmtgt);
		u2t_sciset_nmabs = (unsigned short)abs((int)f4s_sciset_nmsm);
		
		u2s_sciset_stringarray[0] = u2t_sciset_vtabs / 100;
		u2s_sciset_stringarray[1] = (u2t_sciset_vtabs % 100) / 10;
		u2s_sciset_stringarray[2] = u2t_sciset_vtabs % 10;
		
		u2s_sciset_stringarray[3] = u2t_sciset_nmtabs / 100;
		u2s_sciset_stringarray[4] = (u2t_sciset_nmtabs % 100) / 10;
		u2s_sciset_stringarray[5] = u2t_sciset_nmtabs % 10;
		
		u2s_sciset_stringarray[6] = u2t_sciset_nmabs / 100;
		u2s_sciset_stringarray[7] = (u2t_sciset_nmabs % 100) / 10;
		u2s_sciset_stringarray[8] = u2t_sciset_nmabs % 10;
		
		
		if(f4s_sciset_vtgt < 0){ SCI1.TDR = '-';}
		else{SCI1.TDR = '+';}
		while(SCI1.SSR.BIT.TEND == 0);
		vdg_scisend(u2s_sciset_stringarray[0]);
		u1s_sciset_sendnum++;
		break;
		
		
		case 1:
		for(i = 1; i < 3; i++)
		{
			vdg_scisend(u2s_sciset_stringarray[i]);
			while(SCI1.SSR.BIT.TEND == 0);
		}
		u1s_sciset_sendnum++;
		break;
		
		
		case 2:
		if(s2s_sciset_nmtgt < 0){ SCI1.TDR = '-';}
		else{SCI1.TDR = '+';}
		while(SCI1.SSR.BIT.TEND == 0);
		vdg_scisend(u2s_sciset_stringarray[3]);
		u1s_sciset_sendnum++;
		break;
		
		
		case 3:
		for(i = 4; i < 6; i++)
		{
			vdg_scisend(u2s_sciset_stringarray[i]);
			while(SCI1.SSR.BIT.TEND == 0);
		}
		u1s_sciset_sendnum++;
		break;
		
		
		case 4:
		if(f4s_sciset_nmsm < 0){ SCI1.TDR = '-';}
		else{SCI1.TDR = '+';}
		while(SCI1.SSR.BIT.TEND == 0);
		vdg_scisend(u2s_sciset_stringarray[6]);
		u1s_sciset_sendnum++;
		break;
		
		
		case 5:
		for(i = 7; i < 9; i++)
		{
			vdg_scisend(u2s_sciset_stringarray[i]);
			while(SCI1.SSR.BIT.TEND == 0);
		}
		u1s_sciset_sendnum = 0;
		break;
	}
}

void vdg_mtorigin()
{
	volatile unsigned char i;
	volatile unsigned char j;
	
	volatile unsigned short u2t_mtorigin_cntv;
	volatile unsigned short dummy;
	
	volatile static unsigned long u4s_mtorigin_cntcycle = 0;
	volatile static unsigned char u1s_mtorigin_cntstage = 0;
	volatile static unsigned char u1s_mtorigin_cntrot = 0;
	
	u1g_mtorigin_xmtorgfin = 0;
	u2t_mtorigin_cntv = CNT_ZLRN;
	dummy = 1000;
	
	if(u4s_mtorigin_cntcycle > CNT_MTORG*6)
	{
		u4s_mtorigin_cntcycle = 0;
		u1s_mtorigin_cntrot++;
	}
	
	if(u1s_mtorigin_cntrot < POLE_PAIRS)
	{
		u4s_mtorigin_cntcycle++;

		//CNT_MTORG×キャリアT秒毎にStage切り替えて回転させる
		if(u4s_mtorigin_cntcycle <= CNT_MTORG) 	{ u1s_mtorigin_cntstage = 1; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*2) 	{ u1s_mtorigin_cntstage = 2; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*3) 	{ u1s_mtorigin_cntstage = 3; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*4) { u1s_mtorigin_cntstage = 4; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*5) { u1s_mtorigin_cntstage = 5; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*6) { u1s_mtorigin_cntstage = 6; }
		
		switch(u1s_mtorigin_cntstage)
		{
			case 1:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をMTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)をMTIOC4A
			PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)をMTIOC4B
			PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)をMTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)をMTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTIOC3B
			MTU.TOER.BYTE = STAGE1;
			MTU3.TGRD = u2t_mtorigin_cntv;
			MTU4.TGRC = 0;
			MTU4.TGRD = dummy;		//ダミーを設定
			break;
		
			case 2:
			PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)をMTIOC4C
			PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)をMTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)をMTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)をMTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)をMTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTIOC3B
			MTU.TOER.BYTE = STAGE2;
			MTU3.TGRD = u2t_mtorigin_cntv;
			MTU4.TGRD = 0;
			break;
		
			case 3:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をMTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)をMTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)をMTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)をMTIOC4D
			PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)をMTIOC3D
			PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)をMTIOC3B
			MTU.TOER.BYTE = STAGE3;
			MTU4.TGRC = u2t_mtorigin_cntv;
			MTU4.TGRD = 0;
			break;
			
			case 4:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をMTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)をMTIOC4A
			PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)をMTIOC4B
			PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)をMTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)をMTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTIOC3B
			MTU.TOER.BYTE = STAGE4;
			MTU3.TGRD = 0;
			MTU4.TGRC = u2t_mtorigin_cntv;
			MTU4.TGRD = dummy;		//ダミーを設定
			break;
			
			case 5:
			PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)をMTIOC4C
			PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)をMTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)をMTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)をMTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)をMTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTIOC3B
			MTU.TOER.BYTE = STAGE5;
			MTU3.TGRD = 0;
			MTU4.TGRD = u2t_mtorigin_cntv;
			break;
			
			case 6:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をMTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)をMTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)をMTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)をMTIOC4D
			PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)をMTIOC3D
			PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)をMTIOC3B
			MTU.TOER.BYTE = STAGE6;
			MTU4.TGRC = 0;
			MTU4.TGRD = u2t_mtorigin_cntv;
			break;
		}
	}
	
	else
	{
		SCI12.TDR = 'Z';		//原点学習終了をマスターに送信
		
		/***Stage1でホールド***/
		PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をMTIOC4C
		PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)をMTIOC4A
		PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)をMTIOC4B
		PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)をMTIOC4D
		PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)をMTIOC3D
		PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTIOC3B
		MTU.TOER.BYTE = STAGE1;
		MTU3.TGRD = u2t_mtorigin_cntv;
		MTU4.TGRC = 0;
		MTU4.TGRD = dummy;		//ダミーを設定
		
		u4s_mtorigin_cntcycle = 0;
		u1s_mtorigin_cntstage = 0;
		u1s_mtorigin_cntrot = 0;
		
		u1g_mtorigin_xmtorgfin = 1;		//割込みでの回転数算出を許可
		u1g_rxi12_xmtorgrq = 0;		//原点学習要求をリセット
	}
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
	vdg_SCI_init();
	vdg_IPR_init();
	
	vdg_mtioc_outset(ID_ALLOFF);
	
	//while(u1g_rxi12_xmtorgrq == 0);	//マスターから信号受信まで待つ
	u1g_rxi12_xmtorgrq = 1;
	SCI12.TDR = 'A';			//モニタPCに通信開始連絡
	
	//MTU.TOER.BYTE = 0xFF;		//全相出力許可
	MTU.TSTR.BYTE = 0xC4;		//MTU2.TCNT,MTU3.TCNTとMTU4.TCNTのカウント動作を開始
	
	while(1)
	{
		if(u1g_s12adio_xadcex == 1)
		{
//			MTU3.TGRD = 1000;
//			MTU4.TGRC = 1000;
//			MTU4.TGRD = 1000;
			
			//マスターから'A'が送られてきた場合、原点学習処理を優先して実施
			if(u1g_rxi12_xmtorgrq == 1)		//マスターから原点学習要求があった場合
			{
				vdg_mtorigin();
			}
			
			else
			{
				//vdg_current_cal();		//電流センサADC結果から電流値算出
				vdg_mtcnt();			//各相への指令
				//vdg_sciset();			//制御状態を外部へ送信
				
				if(s2g_rxi12_nmtgt == 100){ TRG1 = !TRG1;}
				
//				if(u1g_mtcnt_xrgmode == 1){ TRG1 = 1;}
//				else{ TRG1 = 0; }
			}
			
			u1g_s12adio_xadcex = 0;
		}
		
	}//while
	
}//main