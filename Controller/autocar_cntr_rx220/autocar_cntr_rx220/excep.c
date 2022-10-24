
#include "iodefine.h"
#include "vect.h"
#include <machine.h>
#include "global_data.h"
#include "excep.h"


void Excep_S12AD_S12ADI0()
{
	volatile static unsigned short u2s_s12adio_addr0add = 0;
	volatile static unsigned char u1s_s12adio_adccnt = 0;
	
	u1s_s12adio_adccnt++;
	
	if(u1s_s12adio_adccnt < ADC_CNV_COUNT)
	{
		u2s_s12adio_addr0add += S12AD.ADDR0;
		S12AD.ADCSR.BIT.ADST = 0;
		S12AD.ADCSR.BIT.ADST = 1;
	}
	
	else
	{
		u2s_s12adio_addr0add += S12AD.ADDR0;
		u2g_s12adio_result = u2s_s12adio_addr0add;
		u1s_s12adio_adccnt = 0;
		u2s_s12adio_addr0add = 0;
	}
	
	u1g_s12adio_xadcex = 1;
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
	volatile static unsigned char u1s_tgib2_xinitend = 0;
	
	
	u2t_tgib2_count = MTU2.TGRB;			//TGRBカウント数を取得
	
	/***回転数算出、角度算出は原点学習終了後***/
	if(u1g_mtorigin_xmtorgfin == 1)
	{
		if(u1g_tciv2_xovflw == 1)
		{
			u1g_tciv2_xovflw = 0;
			
			for( i = 0; i < NM_AVE_NUM; i++ ) { f4s_tgib2_nmary[i] = 0; }
			
			f4t_tgib2_time = (float)u2t_tgib2_count / PCLK1024;	//カウント数×クロック時間
			f4t_tgib2_time += 2;				//オーバーフロー2s分加算
			f4t_tgib2_nm = K1PLST2RPM / f4t_tgib2_time;	//1パルス時間から回転数算出
			f4s_tgib2_nmary[0] = f4t_tgib2_nm;
			
			for( i = 0; i < NM_AVE_NUM; i++ ) { f4t_tgib2_nmadd += f4s_tgib2_nmary[i]; }
			
			f4g_tgib2_nmsm = f4t_tgib2_nmadd / (float)NM_AVE_NUM;
		}
		
		else
		{
			f4t_tgib2_time = (float)u2t_tgib2_count / PCLK1024;	//カウント数×クロック時間
			f4t_tgib2_nm = K1PLST2RPM / f4t_tgib2_time;	//1パルス時間から回転数算出
			
			for( i = 1; i < NM_AVE_NUM; i++ )
			{
				f4s_tgib2_nmary[NM_AVE_NUM-i] = f4s_tgib2_nmary[NM_AVE_NUM-i-1];
			}
			
			f4s_tgib2_nmary[0] = f4t_tgib2_nm;
			
			for( i = 0; i < NM_AVE_NUM; i++ ) { f4t_tgib2_nmadd += f4s_tgib2_nmary[i]; }
			
			f4g_tgib2_nmsm = f4t_tgib2_nmadd / (float)NM_AVE_NUM;
		}
	
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
	/***原点学習未完了状態の場合は各RAMを初期化***/
	else
	{
		if(u1s_tgib2_xinitend == 0)
		{
			f4g_tgib2_nmsm = 0;
			u2g_tgib2_citrpt = 0;
			f4g_tgib2_mcangle = 0;
			f4g_tgib2_elangle = 0;
			for( i = 0; i < NM_AVE_NUM; i++ ) { f4s_tgib2_nmary[i] = 0; }
			
			u1s_tgib2_xinitend = 1;
		}
	}
}

void Excep_MTU2_TCIV2()
{
	/***オーバーフローは約2.09sで発生***/
	u1g_tciv2_xovflw = 1;			//配列過去要素の初期化のためにフラグ公開
	u1g_tciv2_xdnmrst = 1;			//停車後オーバーフローすると前回値差分が急変するためdnmを直接リセットのためフラグセット
	f4g_tgib2_nmsm = 0;			//オーバーフローしたら回転数0
}

void Excep_SCI1_TXI1()
{
	
}

void Excep_SCI1_RXI1()
{
	
}

void Excep_SCI12_TXI12()
{
	
}

void Excep_SCI12_RXI12()
{
	volatile static unsigned char u1s_rxi12_rccnt = 0;
	volatile static unsigned char u1s_rxi12_rdata;
	volatile static signed short s2s_rxi12_nmtgt = 0;
	
	u1s_rxi12_rdata = SCI12.RDR;
	
	if(u1s_rxi12_rdata != 'A')
	{
		TRG2 = !TRG2;
		
		switch(u1s_rxi12_rccnt)
		{
			/***100の位***/
			case 0:
			switch(u1s_rxi12_rdata)
			{
				case '0': s2s_rxi12_nmtgt = 0; break;
				case '1': s2s_rxi12_nmtgt = 100; break;
				case '2': s2s_rxi12_nmtgt = 200; break;
				case '3': s2s_rxi12_nmtgt = 300; break;
				case '4': s2s_rxi12_nmtgt = 400; break;
				case '5': s2s_rxi12_nmtgt = 500; break;
				case '6': s2s_rxi12_nmtgt = 600; break;
				case '7': s2s_rxi12_nmtgt = 700; break;
				case '8': s2s_rxi12_nmtgt = 800; break;
				case '9': s2s_rxi12_nmtgt = 900; break;
			}
			u1s_rxi12_rccnt++;
			break;
		
			/***10の位***/
			case 1:
			switch(u1s_rxi12_rdata)
			{
				case '0': break;
				case '1': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 10; break;
				case '2': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 20; break;
				case '3': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 30; break;
				case '4': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 40; break;
				case '5': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 50; break;
				case '6': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 60; break;
				case '7': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 70; break;
				case '8': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 80; break;
				case '9': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 90; break;
			}
			u1s_rxi12_rccnt++;
			break;
			
			/***1の位***/
			case 2:
			switch(u1s_rxi12_rdata)
			{
				case '0': break;
				case '1': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 1; break;
				case '2': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 2; break;
				case '3': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 3; break;
				case '4': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 4; break;
				case '5': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 5; break;
				case '6': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 6; break;
				case '7': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 7; break;
				case '8': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 8; break;
				case '9': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 9; break;
			}
			u1s_rxi12_rccnt++;
			break;
			
			case 3:
			if(u1s_rxi12_rdata == '-')
			{
				s2g_rxi12_nmtgt = -1 * s2s_rxi12_nmtgt;
			}
			else
			{
				s2g_rxi12_nmtgt = s2s_rxi12_nmtgt;
			}
				u1s_rxi12_rccnt = 0;
				u1g_rxi12_xrcnmt = 1;		//目標回転数受信完了フラグ
			break;
			
		}//u1s_rxi12_rccnt
	}
	
	else
	{
		u1g_rxi12_xmtorgrq = 1;		//原点学習開始要求
	}
}


