
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