
#include "iodefine.h"
#include "vect.h"
#include <machine.h>
#include "global_data.h"
#include "api_init.h"

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
	SYSTEM.MSTPCRB.BIT.MSTPB4 = 0;			//SCI12モジュールストップ状態解除
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
	PORT1.PDR.BYTE = 0xFF;		//P14(MTIOC3A)出力
	PORT2.PDR.BYTE = 0x7F;		//P26(TXD1)出力、P27(MTIOC2B)入力
	PORT3.PDR.BYTE = 0xFE;		//P30(RXD1)入力
	PORT4.PDR.BYTE = 0xFE;		//P40(AN000)入力
	PORT5.PDR.BYTE = 0xFF;
	//PORTA.PDR.BYTE = 0xEF;		//PA4(IRQ5)を入力
	PORTA.PDR.BYTE = 0xFF;
	PORTB.PDR.BYTE = 0xFF;		//PB1(MTIOC4C)出力、PB3(MTIOC4A)出力
	PORTC.PDR.BYTE = 0x7F;		//PC7はボード上入力設定にしておく
	PORTE.PDR.BYTE = 0xFB;		//PE1(TXD12)出力、PE2(EXD12)入力
	PORTH.PDR.BYTE = 0xFF;
	
	PORT2.PODR.BIT.B6 = 1;		//TXD端子はHIGH
	PORTE.PODR.BIT.B1 = 1;		//TXD端子はHIGH
	
	PORT3.PCR.BIT.B0 = 1;		//RXD1はプルアップ
	PORTE.PCR.BIT.B2 = 1;		//RXD12はプルアップ
}

void vdg_MPC_init()
{
	/***MPC設定のために書き込みプロテクトレジスタPWPR設定***/
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	/***MPC設定のために書き込みプロテクトレジスタPWPR設定***/
	
	/***APIに合わせてMPCでピン割り付け***/
	MPC.P26PFS.BIT.PSEL = 10;	//P26(CN2-23)をTXD1
	MPC.P30PFS.BIT.PSEL = 10;	//P30(CN2-25)をRXD1
	MPC.P27PFS.BIT.PSEL = 1;	//P27(CN2-24)をMTIOC2B
	MPC.P40PFS.BIT.ASEL = 1;	//P40(CN1-1)をAN000
	MPC.PB1PFS.BIT.PSEL = 2;	//PB1(CN2-2)をMTIOC4C
	MPC.PB3PFS.BIT.PSEL = 2;	//PB3(CN2-3)をMTIOC4A
	MPC.PC2PFS.BIT.PSEL = 1;	//PC2(CN2-7)をMTIOC4B
	MPC.PC3PFS.BIT.PSEL = 1;	//PC3(CN2-8)をMTIOC4D
	MPC.PC4PFS.BIT.PSEL = 1;	//PC4(CN2-9)をMTIOC3D
	MPC.PC5PFS.BIT.PSEL = 1;	//PC5(CN2-10)をMTIOC3B
	MPC.PE1PFS.BIT.PSEL = 12;	//PE1(CN1-8)をTXD12
	MPC.PE2PFS.BIT.PSEL = 12;	//PE2(CN1-9)をRXD12
	
	MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)をMTIOC3Aトグル出力
	//MPC.PA4PFS.BIT.ISEL = 1;	//PA4(CN1-16)をIRQ5
	/***APIに合わせてMPCでピン割り付け***/
	
	//P**PFSレジスタへの書き込み禁止
	MPC.PWPR.BIT.PFSWE = 0;		

	/***ポートモードレジスタ設定(汎用入出力or周辺機能)。P**PFSのうち”PSEL”に対してのみPMRを定義***/
	PORT2.PMR.BIT.B6 = 1;		//P26(CN2-23)をTXD1
	PORT3.PMR.BIT.B0 = 1;		//P30(CN2-25)をRXD1
	PORT2.PMR.BIT.B7 = 1;		//P27(CN2-24)をMTIOC2B
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をMTIOC4C
	PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)をMTIOC4A
	PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)をMTIOC4B
	PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)をMTIOC4D
	PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)をMTIOC3D
	PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTIOC3B
	PORT1.PMR.BIT.B4 = 1;		//P14(CN2-19)をMTIOC3A
	PORTE.PMR.BIT.B1 = 1;		//PE1(CN1-8)をTXD12
	PORTE.PMR.BIT.B2 = 1;		//PE2(CN1-9)をRXD12
	/***ポートモードレジスタ設定(汎用入出力or周辺機能)***/
}

void vdg_MTU2a_init()
{
	MTU.TSTR.BYTE = 0x00;		//TCNTカウントを停止
	MTU3.TCR.BYTE = 0x00;		//PCLK/1、立ち上がりエッジでカウント、TCNTクリア禁止
	MTU4.TCR.BYTE = 0x00;		//PCLK/1、立ち上がりエッジでカウント、TCNTクリア禁止
	
	/***TCNTの初期設定***/
	MTU3.TCNT = CNT_DT;		//デッドタイム設定
	MTU4.TCNT = 0;
	/***TCNTの初期設定***/
	
	/***デューティ比の初期設定***/
	/*コンペアレジスタ*/
	MTU3.TGRB = CNT_DT;
	MTU4.TGRA = CNT_DT;
	MTU4.TGRB = CNT_DT;
	/*コンペアレジスタ*/
	/*バッファレジスタ*/
	MTU3.TGRD = CNT_DT;
	MTU4.TGRC = CNT_DT;
	MTU4.TGRD = CNT_DT;
	/*バッファレジスタ*/
	/***デューティ比の初期設定***/
	
	/***デッドタイム、キャリア周期の設定***/
	MTU.TDDR = CNT_DT;
	MTU.TCDR = CNT_HALF_CARR;
	MTU.TCBR = CNT_HALF_CARR;
	MTU3.TGRA = CNT_HALF_CARR + CNT_DT;
	MTU3.TGRC =  CNT_HALF_CARR + CNT_DT;
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

void vdg_SCI_init()
{
	/***PCとの通信用***/
	SCI1.SCR.BYTE = 0x00;		//初期化
	SCI1.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	//SCI1.SIMR1.BIT.IICM = 0;	//シリアルインターフェースモード
	SCI1.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI1.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI1.SNFR.BIT.NFCS = 4;		//8分周分のクロックをノイズフィルタに使用
	SCI1.BRR = 3;			//3:500000bps,9:200000bps
	SCI1.SCR.BIT.RE = 1;		//受信動作を許可
	SCI1.SCR.BIT.TE = 1;		//送信動作を許可
	SCI1.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI1.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	
	ICU.IER[0x1B].BIT.IEN4 = 1;	//送信割込み許可
	ICU.IER[0x1B].BIT.IEN3 = 1;	//受信割込み許可
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//送信完了割込み許可
	
	/***マスターマイコンとの通信用***/
	SCI12.SCR.BYTE = 0x00;		//初期化
	SCI12.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	//SCI1.SIMR1.BIT.IICM = 0;	//シリアルインターフェースモード
	SCI12.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI12.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI12.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI12.SNFR.BIT.NFCS = 4;		//8分周分のクロックをノイズフィルタに使用
	SCI12.BRR = 7;			//3:500000bps,9:200000bps,7:250000bps
	SCI12.SCR.BIT.RE = 1;		//受信動作を許可
	SCI12.SCR.BIT.TE = 1;		//送信動作を許可
	SCI12.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI12.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	
	ICU.IER[0x1E].BIT.IEN0 = 1;	//送信割込み許可
	ICU.IER[0x1D].BIT.IEN7 = 1;	//受信割込み許可
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
	ICU.IPR[238].BIT.IPR = 11;	//SCI12割込み優先度（送受信共通）
	ICU.IPR[125].BIT.IPR = 14;	//MTIOC2Bインプットキャプチャ割込み優先度
	ICU.IPR[127].BIT.IPR = 15;	//MTU2.TCNTオーバーフロー割込み優先度
	
	//ICU.IPR[69].BIT.IPR = 15;	//IRQ5割込み優先度
}