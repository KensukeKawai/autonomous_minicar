
#include "common.h"

/***************グローバル変数定義***************/
//excep内は先頭に"ex"を付与する

/***MTU0.TGIA0割込み変数定義***/
volatile unsigned char u1g_extgia0_interrupt;

/***電流センサADC関係変数定義***/
volatile unsigned short u2g_exs12adi0_adccntm1ad;
volatile unsigned short u2g_exs12adi0_adccntm2ad;
volatile unsigned char u1g_exs12adi0_xadcex;

/***SPI関係変数定義***/
volatile unsigned char u1g_exspri0_xrspirec;


//以下、今時点で未使用
/***回転数関係RAM定義***/
volatile float f4g_tgib2_nmsm;
volatile unsigned short u2g_tgib2_citrpt;
volatile unsigned char u1g_tgib2_xzero;
volatile float f4g_tgib2_elangle;
volatile float f4g_tgib2_mcangle;

volatile unsigned char u1g_tciv2_xovflw;
volatile unsigned char u1g_tciv2_xdnmrst;

volatile unsigned char u1g_rxi12_xmtorgrq;
volatile unsigned char u1g_rxi12_xrcnmt;
volatile signed short s2g_rxi12_nmtgt;
volatile unsigned char u1g_rxi12_nmtary[4] = {0};

/***************グローバル変数定義***************/



/**********TGRAのコンペアマッチ割込み処理**********/
//これがカウンタの山トリガ。ここから処理開始していく。
void Excep_MTU0_TGIA0()
{
	u1g_extgia0_interrupt = 1;
}

/**********AD変換の割込み処理**********/
//所定回数分のAD変換実施し加算
//所定回数実施したら完了フラグを立てる
//ADDR0：M1、ADDR1：M2
void Excep_S12AD_S12ADI0()	//完了
{
	/***************static変数定義***************/
	volatile static unsigned short u2s_s12adi0_addrm1add = 0;
	volatile static unsigned short u2s_s12adi0_addrm2add = 0;
	volatile static unsigned char u1s_s12adi0_adccnt = 0;
	
	u1s_s12adi0_adccnt++;
	
	//ADC平均回数未満の場合
	if(u1s_s12adi0_adccnt < ADC_CNV_COUNT)
	{
		u2s_s12adi0_addrm1add += S12AD.ADDR0;
		u2s_s12adi0_addrm2add += S12AD.ADDR1;
		S12AD.ADCSR.BIT.ADST = 0;
		S12AD.ADCSR.BIT.ADST = 1;
	}
	
	//ADC平均回数に到達した場合
	else
	{
		u2s_s12adi0_addrm1add += S12AD.ADDR0;
		u2s_s12adi0_addrm2add += S12AD.ADDR1;
		//resultに平均回数分加算したADCカウントを格納。ADC_CNT2Vで平均回数分も含めて計算するため加算値で良い。
		u2g_exs12adi0_adccntm1ad = u2s_s12adi0_addrm1add;
		u2g_exs12adi0_adccntm2ad = u2s_s12adi0_addrm2add;
		//static変数をリセット
		u1s_s12adi0_adccnt = 0;
		u2s_s12adi0_addrm1add = 0;
		u2s_s12adi0_addrm2add = 0;

		//ADC_CNV_COUNT回ADC実施完了後、フラグを立てる
		u1g_exs12adi0_xadcex = 1;
	}
}

/**********SPI受信バッファフルの割込み処理**********/
//JetsonからのチップセレクトアクティブLowから4Bite分受信完了したらここに来る
//Jetson指令受信後次タスクでJetsonに送信したいデータをLong Wordで入れておく
void Excep_RSPI0_SPRI0()
{
	// RSPI0.SPCR.BIT.SPRIE = 0;		//受信割込み要求の発生を許可
	// PORT_GENERAL_P1 != PORT_GENERAL_P1;
	u1g_exspri0_xrspirec = 1;		//JetsonからのSPI受信完了フラグON
	// RSPI0.SPDR.LONG = 0;
}


void Excep_SCI1_TXI1()
{
	
}

void Excep_SCI1_RXI1()
{
	
}



// void Excep_MTU2_TCIV2()
// {
// 	/***オーバーフローは約2.09sで発生***/
// 	u1g_tciv2_xovflw = 1;			//配列過去要素の初期化のためにフラグ公開
// 	u1g_tciv2_xdnmrst = 1;			//停車後オーバーフローすると前回値差分が急変するためdnmを直接リセットのためフラグセット
// 	f4g_tgib2_nmsm = 0;			//オーバーフローしたら回転数0
// }



// void Excep_MTU2_TGIB2()
// {
// 	volatile unsigned char i;
// 	volatile unsigned short u2t_tgib2_count;
// 	volatile float f4t_tgib2_time;
// 	volatile float f4t_tgib2_nm;
// 	volatile float f4t_tgib2_nmadd = 0;
// 	volatile float f4t_tgib2_elanglefull;
// 	volatile unsigned short u2t_tgib2_countel360;
	
// 	volatile static float f4s_tgib2_nmary[NM_AVE_NUM] = {0};
// 	volatile static float f4s_tgib2_nmsmz = 0;
// 	volatile static float f4s_tgib2_cel360 = 1;
// 	volatile static unsigned char u1s_tgib2_xinitend = 0;
	
	
// 	u2t_tgib2_count = MTU2.TGRB;			//TGRBカウント数を取得
	
// 	/***回転数算出、角度算出は原点学習終了後***/
// 	if(u1g_mtcnt_xmtorgfin == 1)
// 	{
// 		if(u1g_tciv2_xovflw == 1)
// 		{
// 			u1g_tciv2_xovflw = 0;
			
// 			for( i = 0; i < NM_AVE_NUM; i++ ) { f4s_tgib2_nmary[i] = 0; }
			
// 			f4t_tgib2_time = (float)u2t_tgib2_count / PCLK1024;	//カウント数×クロック時間
// 			f4t_tgib2_time += 2;				//オーバーフロー2s分加算
// 			f4t_tgib2_nm = K1PLST2RPM / f4t_tgib2_time;	//1パルス時間から回転数算出
// 			f4s_tgib2_nmary[0] = f4t_tgib2_nm;
			
// 			for( i = 0; i < NM_AVE_NUM; i++ ) { f4t_tgib2_nmadd += f4s_tgib2_nmary[i]; }
			
// 			f4g_tgib2_nmsm = f4t_tgib2_nmadd / (float)NM_AVE_NUM;
// 		}
		
// 		else
// 		{
// 			f4t_tgib2_time = (float)u2t_tgib2_count / PCLK1024;	//カウント数×クロック時間
// 			f4t_tgib2_nm = K1PLST2RPM / f4t_tgib2_time;	//1パルス時間から回転数算出
			
// 			for( i = 1; i < NM_AVE_NUM; i++ )
// 			{
// 				f4s_tgib2_nmary[NM_AVE_NUM-i] = f4s_tgib2_nmary[NM_AVE_NUM-i-1];
// 			}
			
// 			f4s_tgib2_nmary[0] = f4t_tgib2_nm;
			
// 			for( i = 0; i < NM_AVE_NUM; i++ ) { f4t_tgib2_nmadd += f4s_tgib2_nmary[i]; }
			
// 			f4g_tgib2_nmsm = f4t_tgib2_nmadd / (float)NM_AVE_NUM;
// 		}
	
// 		u2g_tgib2_citrpt++;
		
// 		if(u2g_tgib2_citrpt >= ENC_NTEETH*2)
// 		{
// 			u2g_tgib2_citrpt -= ENC_NTEETH*2;
// 			u1g_tgib2_xzero = 1;
// 		}
		
// 		f4g_tgib2_mcangle = (float)u2g_tgib2_citrpt * (float)KPLSN2MANGLE;
// 		f4t_tgib2_elanglefull = f4g_tgib2_mcangle * POLE_PAIRS;
		
// 		u2t_tgib2_countel360 = f4t_tgib2_elanglefull / 360;
// 		f4g_tgib2_elangle = f4t_tgib2_elanglefull - (float)u2t_tgib2_countel360 * 360;
// 	}
// 	/***原点学習未完了状態の場合は各RAMを初期化***/
// 	else
// 	{
// 		if(u1s_tgib2_xinitend == 0)
// 		{
// 			f4g_tgib2_nmsm = 0;
// 			u2g_tgib2_citrpt = 0;
// 			f4g_tgib2_mcangle = 0;
// 			f4g_tgib2_elangle = 0;
// 			for( i = 0; i < NM_AVE_NUM; i++ ) { f4s_tgib2_nmary[i] = 0; }
			
// 			u1s_tgib2_xinitend = 1;
// 		}
// 	}
// }



// void Excep_SCI12_RXI12()
// {
// 	volatile static unsigned char u1s_rxi12_rccnt = 0;
// 	volatile static unsigned char u1s_rxi12_rdata;
// 	volatile static signed short s2s_rxi12_nmtgt = 0;
	
// 	u1s_rxi12_rdata = SCI12.RDR;
	
// 	/***マスターからA以外を受信した場合は目標回転数情報のため、目標回転数処理を実施***/
// 	if(u1s_rxi12_rdata != 'A')
// 	{
// 		// TRG2 = !TRG2;
		
// 		switch(u1s_rxi12_rccnt)
// 		{
// 			/***100の位***/
// 			case 0:
// 			switch(u1s_rxi12_rdata)
// 			{
// 				case '0': s2s_rxi12_nmtgt = 0; break;
// 				case '1': s2s_rxi12_nmtgt = 100; break;
// 				case '2': s2s_rxi12_nmtgt = 200; break;
// 				case '3': s2s_rxi12_nmtgt = 300; break;
// 				case '4': s2s_rxi12_nmtgt = 400; break;
// 				case '5': s2s_rxi12_nmtgt = 500; break;
// 				case '6': s2s_rxi12_nmtgt = 600; break;
// 				case '7': s2s_rxi12_nmtgt = 700; break;
// 				case '8': s2s_rxi12_nmtgt = 800; break;
// 				case '9': s2s_rxi12_nmtgt = 900; break;
// 			}
// 			u1s_rxi12_rccnt++;
// 			break;
		
// 			/***10の位***/
// 			case 1:
// 			switch(u1s_rxi12_rdata)
// 			{
// 				case '0': break;
// 				case '1': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 10; break;
// 				case '2': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 20; break;
// 				case '3': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 30; break;
// 				case '4': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 40; break;
// 				case '5': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 50; break;
// 				case '6': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 60; break;
// 				case '7': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 70; break;
// 				case '8': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 80; break;
// 				case '9': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 90; break;
// 			}
// 			u1s_rxi12_rccnt++;
// 			break;
			
// 			/***1の位***/
// 			case 2:
// 			switch(u1s_rxi12_rdata)
// 			{
// 				case '0': break;
// 				case '1': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 1; break;
// 				case '2': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 2; break;
// 				case '3': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 3; break;
// 				case '4': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 4; break;
// 				case '5': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 5; break;
// 				case '6': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 6; break;
// 				case '7': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 7; break;
// 				case '8': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 8; break;
// 				case '9': s2s_rxi12_nmtgt = s2s_rxi12_nmtgt + 9; break;
// 			}
// 			u1s_rxi12_rccnt++;
// 			break;
			
// 			case 3:
// 			if(u1s_rxi12_rdata == '-')
// 			{
// 				s2g_rxi12_nmtgt = -1 * s2s_rxi12_nmtgt;
// 			}
// 			else
// 			{
// 				s2g_rxi12_nmtgt = s2s_rxi12_nmtgt;
// 			}
// 				u1s_rxi12_rccnt = 0;
// 				u1g_rxi12_xrcnmt = 1;		//目標回転数受信完了フラグ
// 			break;
			
// 		}//u1s_rxi12_rccnt
// 	}
	
// 	/***マスターからAを受信した場合は原点学習要求のためフラグセット***/
// 	else
// 	{
// 		u1g_rxi12_xmtorgrq = 1;		//原点学習開始要求
// 	}
// }


