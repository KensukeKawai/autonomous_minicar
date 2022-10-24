
#include "iodefine.h"
#include "vect.h"
#include <mathf.h>
#include <machine.h>
#include "global_data.h"


//使用ハードウェア設定関数
void vdg_HW_init()
{
	/*****************************************/
	// ICLK = PCLKD = 32MHs,		 //
	// PCLKB = FCLK = BCLK = 32MHz		 //
	/*****************************************/
	
	unsigned int i;
	
	/***プロテクト解除***/
   	SYSTEM.PRCR.WORD = 0xA503;			//クロック発生回路、消費電力低減機能関連レジスタへの書込み許可
	/***プロテクト解除***/

	/***サブクロック停止設定***/
	SYSTEM.SOSCCR.BYTE = 0x01;			//サブクロック発振器停止
	RTC.RCR3.BIT.RTCEN = 0;				//サブクロック発振器停止
	/***サブクロック停止設定***/
	
	/***HOCOクロックの発振設定***/
  	SYSTEM.HOCOCR2.BYTE = 0x00;			//高速オンチップオシレータHOCRの動作周波数=32MHz
    	SYSTEM.HOCOWTCR2.BYTE = 0x14;			//HOCOクロック発生までの待機時間設定。180サイクル(約5.625us)待ち。
    	SYSTEM.HOCOCR.BYTE = 0x00;			//HOCO動作。ここからHOCOWTCR2で設定した時間待たなければいけない
    	for(i=0;i<100;i++){ nop();}			//9216サイクル分待ち
	/***HOCOクロックの発振設定***/
	
	/***内部クロック分周比設定***/
	SYSTEM.SCKCR.LONG = 0x0A;			//ｼｽﾃﾑｸﾛｯｸ(ICLK),周辺ﾓｼﾞｭｰﾙｸﾛｯｸB/D(PCLKB/D),外部ﾊﾞｽｸﾛｯｸ(BCLK),FlashIFｸﾛｯｸ)FCLK)を1分周
	//while (0x0A != SYSTEM.SCKCR.LONG){}		//SCKCRの書き込み完了してるか確認待ち
	/***内部クロック分周比設定***/
	
	/***システムクロック切り替え***/
	SYSTEM.SCKCR3.BIT.CKSEL = 1;			//HOCO選択
	//while (1 != SYSTEM.SCKCR3.BIT.CKSEL){}	//書き込まれてるか確認待ち
	/***システムクロック切り替え***/
	
	/***動作電力制御モード設定***/
	SYSTEM.OPCCR.BIT.OPCM = 2;			//中速動作モード1A
	while (SYSTEM.OPCCR.BIT.OPCM != 2){}		//遷移完了待ち
	/***動作電力制御モード設定***/
	
	/***モジュールストップ解除設定***/
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;			//MTU2aモジュールストップ状態解除
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;			//S12ADモジュールストップ状態解除
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;			//SCI1モジュールストップ状態解除
	//CMPAはアナログモジュールのため設定不要
	/***モジュールストップ解除設定***/
	
	/***プロテクト設定***/
	SYSTEM.PRCR.WORD = 0xA500;			//PRCRレジスタに、各レジスタへの書込み禁止設定
	/***プロテクト設定***/
}

//入出力設定関数
void vdg_IO_init()
{	
	PORT0.PDR.BYTE = 0xFF;
	PORT1.PDR.BYTE = 0xDF;		//P15(RXD1)入力
	PORT2.PDR.BYTE = 0xFF;
	PORT3.PDR.BYTE = 0xFE;		//P30(RXD1)入力
	PORT4.PDR.BYTE = 0xFE;		//P40(AN000)入力
	PORT5.PDR.BYTE = 0xFF;
	//PORTA.PDR.BYTE = 0xFD;		//PA1(CVREFA)入力
	//PORTA.PDR.BYTE = 0xEF;		//PA4(IRQ5)を入力
	PORTA.PDR.BYTE = 0xFF;
	PORTB.PDR.BYTE = 0xFF;
	PORTC.PDR.BYTE = 0x7F;		//PC7はボード上入力設定にしておく
	//PORTE.PDR.BYTE = 0xF7;		//PE3(CMPA1)入力
	PORTE.PDR.BYTE = 0xDF;		//PE5(MTIOC2B)を入力
	PORTH.PDR.BYTE = 0xFF;
	
	
	//PORTA.PDR.BIT.B1 = 0;		//PA1(CVREFA)入力
	//PORTE.PDR.BIT.B3 = 0;		//PE3(CMPA1)入力
}

void vdg_MPC_init()
{
	/***MPC設定のために書き込みプロテクトレジスタPWPR設定***/
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	/***MPC設定のために書き込みプロテクトレジスタPWPR設定***/
	
	//PORTA.PMR.BIT.B1 = 0;
	//PORTE.PMR.BIT.B3 = 0;
	
	/***APIに合わせてMPCでピン割り付け***/
	MPC.P15PFS.BIT.PSEL = 10;	//P15(CN2-20)をRXD1
	MPC.P16PFS.BIT.PSEL = 10;	//P16(CN2-21)をTXD1
	MPC.P40PFS.BIT.ASEL = 1;	//P40(CN1-1)をAN000
	//MPC.PA1PFS.BIT.ASEL = 1;	//PA1(CN1-14)をCVREFA
	MPC.PB1PFS.BIT.PSEL = 2;	//PB1(CN2-2)をMTIOC4C
	MPC.PB3PFS.BIT.PSEL = 2;	//PB3(CN2-3)をMTIOC4A
	MPC.PC2PFS.BIT.PSEL = 1;	//PC2(CN2-7)をMTIOC4B
	MPC.PC3PFS.BIT.PSEL = 1;	//PC3(CN2-8)をMTIOC4D
	MPC.PC4PFS.BIT.PSEL = 1;	//PC4(CN2-9)をMTIOC3D
	MPC.PC5PFS.BIT.PSEL = 1;	//PC5(CN2-10)をMTIOC3B
	MPC.PE5PFS.BIT.PSEL = 2;	//PE5(CN1-12)をMTIOC2B
	//MPC.PE3PFS.BIT.ASEL = 1;	//PE3(CN1-10)をCMPA1
	
	MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)をMTIOC3Aトグル出力
	//MPC.PA4PFS.BIT.ISEL = 1;	//PA4(CN1-16)をIRQ5
	/***APIに合わせてMPCでピン割り付け***/
	
	//P**PFSレジスタへの書き込み禁止
	MPC.PWPR.BIT.PFSWE = 0;		

	/***ポートモードレジスタ設定(汎用入出力or周辺機能)。P**PFSのうち”PSEL”に対してのみPMRを定義***/
	PORT1.PMR.BIT.B5 = 1;		//P15(CN2-20)をRXD1
	PORT1.PMR.BIT.B6 = 1;		//P16(CN2-21)をTXD1
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をMTIOC4C
	PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)をMTIOC4A
	PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)をMTIOC4B
	PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)をMTIOC4D
	PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)をMTIOC3D
	PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTIOC3B
	PORTE.PMR.BIT.B5 = 1;		//PE5(CN1-12)をMTIOC2B
	
	PORT1.PMR.BIT.B4 = 1;		//P14(CN2-19)をMTIOC3A
	/***ポートモードレジスタ設定(汎用入出力or周辺機能)***/
}

void vdg_wait_nop(unsigned long N)
{
	unsigned long i;
	for(i=0;i<N;i++){ nop();}
}

void vdg_MTU2a_init()
{
	MTU.TSTR.BYTE = 0x00;		//TCNTカウントを停止
	MTU3.TCR.BYTE = 0x00;		//PCLK/1、立ち上がりエッジでカウント、TCNTクリア禁止
	MTU4.TCR.BYTE = 0x00;		//PCLK/1、立ち上がりエッジでカウント、TCNTクリア禁止
	
	/***TCNTの初期設定***/
	MTU3.TCNT = DEADTIME;		//デッドタイム設定
	MTU4.TCNT = 0;
	/***TCNTの初期設定***/
	
	/***デューティ比の初期設定***/
	/*コンペアレジスタ*/
	MTU3.TGRB = DEFAULT_DUTY;
	MTU4.TGRA = DEFAULT_DUTY;
	MTU4.TGRB = DEFAULT_DUTY;
	/*コンペアレジスタ*/
	/*バッファレジスタ*/
	MTU3.TGRD = DEFAULT_DUTY;
	MTU4.TGRC = DEFAULT_DUTY;
	MTU4.TGRD = DEFAULT_DUTY;
	/*バッファレジスタ*/
	/***デューティ比の初期設定***/
	
	/***デッドタイム、キャリア周期の設定***/
	MTU.TDDR = DEADTIME;
	MTU.TCDR = CARRIER_HALF_COUNT;
	MTU.TCBR = CARRIER_HALF_COUNT;
	MTU3.TGRA = CARRIER_HALF_COUNT + DEADTIME;
	MTU3.TGRC =  CARRIER_HALF_COUNT + DEADTIME;
	/***デッドタイム、キャリア周期の設定***/
	
	/***PWM出力レベル設定：正相をローサイド、逆相をハイサイド***/
	MTU.TOCR1.BIT.OLSP = 1;		//正相出力を初期LOWアクティブHIGH、UP(HIGH)DOWN(LOW)
	MTU.TOCR1.BIT.OLSN = 1;		//逆相出力を初期LOWアクティブHIGH、UP(LOW)DOWN(HIGH)
	/***PWM出力レベル設定：正相をローサイド、逆相をハイサイド***/
	
	MTU.TOCR1.BIT.PSYE =1;		//MTIOC3Aトグル出力
	
	/***相補PWMモード設定(MTU3のみに行う)***/
	MTU3.TMDR.BIT.MD = 13;		//相補PWMモード1（山で転送）
	MTU3.TMDR.BIT.BFA = 1;		//TGRAとTGRCをバッファ動作
	MTU3.TMDR.BIT.BFB = 1;		//TGRBとTGRDをバッファ動作
	/***相補PWMモード設定***/
	
	//MTU4.TIER.BIT.TTGE2 = 1;	//MTU4.TCNTアンダーフローでAD変換要求出力
	//MTU3.TIER.BIT.TGIEA = 1;	//MTU3.TGRAの割込み要求(TGIA)を許可
	MTU3.TIER.BIT.TTGE = 1;		//山でAD変換開始要求発生を許可
	
	/***出力許可禁止設定***/
	MTU.TOER.BYTE = 0xC0;		//初期設定では出力禁止に設定
//	MTU.TOER.BYTE = 0xFF;		//全相出力許可
	/***出力許可禁止設定***/
	
//	MTU.TSTR.BYTE = 0xC0;		//MTU3.TCNTとMTU4.TCNTのカウント動作を開始
	

	/***MTU2インプットキャプチャの設定***/
	MTU2.TCR.BIT.TPSC = 7;		//PCLK/1024でカウント
	MTU2.TCR.BIT.CKEG = 0;		//立ち上がりエッジでカウント
	MTU2.TCR.BIT.CCLR = 2;		//TGRBインプットキャプチャでTCNTクリア
	
	MTU2.TIOR.BIT.IOB = 0;		//アウトプットコンペア、出力禁止(フィルタ有効時内部エッジ考慮)
	MTU2.NFCR.BIT.NFBEN = 1;	//MTIOCnB端子のノイズフィルタを許可
	MTU2.NFCR.BIT.NFCS = 2;		//ノイズフィルタクロックをPCLK/32
	
	vdg_wait_nop(100);		//クロック設定後、設定したクロック2周期分程度待ってからインプットキャプチャに設定
	MTU2.TIOR.BIT.IOB = 10;		//MTIOC2Bを両エッジでインプットキャプチャ
	
	MTU2.TIER.BIT.TGIEB = 1;	//割込み要求TGIBを許可
	MTU2.TIER.BIT.TCIEV = 1;	//オーバーフロー割込み要求TCIVを許可
	ICU.IER[0X0F].BIT.IEN6 = 1;	//TGIB2の割込み要求を許可
	ICU.IER[0x0F].BIT.IEN7 = 1;	//TCIV2の割込み要求を許可
	
	MTU.TSTR.BIT.CST2 = 0;		//これを1にするとカウント開始
	/***MTU2インプットキャプチャの設定***/
}

void vdg_S12AD_init()
{
	S12AD.ADCSR.BIT.ADCS = 0;		//シングルスキャンモード
	S12AD.ADCSR.BIT.ADIE = 1;		//スキャン終了割込み許可
	S12AD.ADANSA.WORD = 0x01;		//AN000を変換対象にする
	S12AD.ADCER.BIT.ACE = 1;		//データ読み出しによる自動クリアを許可
	S12AD.ADSTRGR.BIT.TRSA = 3;		//MTU3.TGRAコンペアマッチでADC開始トリガ設定
	S12AD.ADCSR.BIT.TRGE = 1;		//トリガ開始許可

	ICU.IER[0x0C].BIT.IEN6 = 1;		//S12ADのS12ADI0割込み要求を許可
}

void vdg_SCI1_init()
{
	SCI1.SCR.BYTE = 0x00;		//初期化
	SCI1.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	//SCI1.SIMR1.BIT.IICM = 0;	//シリアルインターフェースモード
	SCI1.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI1.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI1.SNFR.BIT.NFCS = 0;		//1分周分のクロックをノイズフィルタに使用
	SCI1.BRR = 4;			//4:312500bps, 9:156250bps
	SCI1.SCR.BIT.RE = 1;		//受信動作を許可
	SCI1.SCR.BIT.TE = 1;		//送信動作を許可
	SCI1.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI1.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	
	ICU.IER[0x1B].BIT.IEN4 = 1;	//送信割込み許可
	ICU.IER[0x1B].BIT.IEN3 = 1;	//受信割込み許可
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//送信完了割込み許可
}


void vdg_CMPA_init()
{
	SYSTEM.PRCR.BIT.PRC3 = 1;		//LVD関連レジスタへの書き込み許可
	//SYSTEM.LVCMPCR.BIT.EXVREFINP1 = 1;	//コンパレータA1外部ref電圧入力選択：CVREFA端子入力電圧
	//SYSTEM.LVCMPCR.BIT.EXVCCINP1 = 1;	//コンパレータA1比較電圧入力選択：CMPA1端子入力電圧
	SYSTEM.LVCMPCR.BYTE = 0x03;		//CVREFA,CMPA1端子入力電圧
	//SYSTEM.LVD1CR0.BIT.LVD1FSAMP = 
	SYSTEM.LVD1CR0.BIT.LVD1DFDIS = 0;	//ディジタルフィルタ”無効”
	SYSTEM.LVD1CR0.BIT.LVD1RI = 0;		//コンパレータA1割込み
	SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 2;	//割込みELC発生条件：CMPA1とCVREFAが上昇下降両トリガ
	SYSTEM.LVD1CR1.BIT.LVD1IRQSEL = 1;	//割込み種類を「マスカブル割込み」に設定
	SYSTEM.LVCMPCR.BIT.LVD1E = 1;		//コンパレータA1回路有効
	vdg_wait_nop(50000);
	
	SYSTEM.LVD1CR0.BIT.LVD1CMPE = 1;	//コンパレータA1回路比較結果出力許可
	SYSTEM.LVD1SR.BIT.LVD1DET = 0;		//コンパレータA1電圧変化検出フラグ。初期設定で0。
	SYSTEM.LVD1CR0.BIT.LVD1RIE = 1;		//コンパレータA1割込み要求許可
	
	ICU.IER[0x0B].BIT.IEN0 = 1;		//コンパレータA1割込み許可
}


//void vdg_IRQ_init()
//{
//	ICU.IER[0x08].BYTE = 0x00;		//全IRQを割込み要求禁止
//	ICU.IRQFLTE0.BYTE = 0x00;		//全IRQをディジタルフィルタ無効
//	ICU.IRQFLTC0.BIT.FCLKSEL5 = IRQ_FLT_8;	//サンプリングクロックPCLK/8
//	ICU.IRQCR[5].BIT.IRQMD = 3;		//IRQ5両エッジ検出
//	ICU.IR[69].BIT.IR = 0;			//IRQ5割込み要求フラグ"0"（[]内はベクタ番号）
//	ICU.IRQFLTE0.BIT.FLTEN5 = 1;		//ディジタルフィルタ有効
//	//ICU.FIR.BIT.FVCT = 69;			//IRQ5を高速割込みに設定。ベクタ番号
//	//ICU.FIR.BIT.FIEN = 1;			//高速割込みを許可
//	ICU.IER[0x08].BIT.IEN5 = 1;		//IRQ5割込み要求許可
//}

void vdg_IPR_init()
{
	ICU.IPR[102].BIT.IPR = 13;	//S12ADI0外部割込み優先度
	ICU.IPR[218].BIT.IPR = 12;	//SCI1割込み優先度（送受信共通）
	ICU.IPR[125].BIT.IPR = 14;	//MTIOC2Bインプットキャプチャ割込み優先度
	ICU.IPR[127].BIT.IPR = 15;	//MTU2.TCNTオーバーフロー割込み優先度
	
	//ICU.IPR[69].BIT.IPR = 15;	//IRQ5割込み優先度
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

	
	u2t_tgib2_count = MTU2.TGRB;			//TGRBカウント数を取得
	
	f4t_tgib2_time = (float)u2t_tgib2_count / PCLK1024;	//カウント数×クロック時間
	f4t_tgib2_nm = K1PLST2RPM / f4t_tgib2_time;	//1パルス時間から回転数算出
	
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
	/***オーバーフローは約2.09sで発生***/
	u1g_tciv2_xovflw = 1;			//配列過去要素の初期化のためにフラグ公開
	f4g_tgib2_nmsm = 0;			//オーバーフローしたら回転数0
	f4g_tgib2_dnm = 0;			//オーバーフローしたら回転数変化量0
}


//void Excep_CMPA_CMPA1()
//{
//	SYSTEM.LVD1SR.BIT.LVD1DET = 0;		//コンパレータA1電圧変化検出フラグを手動リセット
//	CMPA_TRG = !CMPA_TRG;
//	//原点学習後に角度検出開始
//	//ここでタイマと連携して割込み時間から角速度を算出
//}

void vdg_variable_init()
{

}

void vdg_current_cal()
{
	volatile unsigned short i;
	volatile float f4t_current_vout;
	volatile float f4t_current_imadd = 0;
	volatile static float f4s_current_imary[ADC_AVE_NUM] = {0};	//全て初期化
	
	f4t_current_vout = u2g_adc_result * ADC_CNT2V;
	
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
	
	f4g_current_i = f4t_current_vout;
}

void vdg_motor_cntr()
{
		
	//f4g_tgib2_dnm = f4g_tgib2_nmsm - f4s_tgib2_nmsmz;	//1キャリア毎の回転数変化量(今回値-前回値)
	
	//f4s_tgib2_nmsmz = f4g_tgib2_nmsm;		//前回値ラッチ
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
	
	MTU.TOER.BYTE = 0xFF;		//全相出力許可
	MTU.TSTR.BYTE = 0xC4;		//MTU2.TCNT,MTU3.TCNTとMTU4.TCNTのカウント動作を開始

	while(1)
	{

		if(u1g_adc_xadcex == 1)
		{
			vdg_current_cal();		//電流センサADC結果から電流値算出
			
			MTU3.TGRD = 2000;
			MTU4.TGRC = 2000;
			MTU4.TGRD = 2000;
			
			if(u2g_adc_result > 2200){TRG1 = 1;}
			else{TRG1 = 0;}
			
			u1g_adc_xadcex = 0;
		}
		
	}//while
	
}//main