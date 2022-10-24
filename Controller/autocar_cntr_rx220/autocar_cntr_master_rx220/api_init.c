
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
	//SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;			//MTU2aモジュールストップ状態解除
	//SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;			//S12ADモジュールストップ状態解除
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;			//SCI1モジュールストップ状態解除
	SYSTEM.MSTPCRB.BIT.MSTPB26 = 0;			//SCI5モジュールストップ状態解除
	SYSTEM.MSTPCRB.BIT.MSTPB25 = 0;			//SCI6モジュールストップ状態解除
	SYSTEM.MSTPCRC.BIT.MSTPC26 = 0;			//SCI9モジュールストップ状態解除
	SYSTEM.MSTPCRB.BIT.MSTPB4 = 0;			//SCI12モジュールストップ状態解除
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;			//CMT0,CMT1モジュールストップ状態解除
	SYSTEM.MSTPCRA.BIT.MSTPA5 = 0;			//TMR0,TMR1モジュールストップ状態解除
	
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
	PORT2.PDR.BYTE = 0xFF;		//P26(TXD1)出力
	PORT3.PDR.BYTE = 0xFE;		//P30(RXD1)入力
	PORT4.PDR.BYTE = 0xFF;		//
	PORT5.PDR.BYTE = 0xFF;
	PORTA.PDR.BYTE = 0xF7;		//PA3(RXD5)入力
	PORTB.PDR.BYTE = 0xBE;		//PB0(RXD6)入力、PB6(RXD9)入力
	PORTC.PDR.BYTE = 0x7F;		//PC7はボード上入力設定にしておく
	PORTE.PDR.BYTE = 0xFB;		//PE1(TXD12)出力、PE2(RXD12)入力
	PORTH.PDR.BYTE = 0xFF;
	
	PORT2.PODR.BIT.B6 = 1;		//TXD1はHIGH
	PORTA.PODR.BIT.B4 = 1;		//TXD5はHIGH
	PORTB.PODR.BIT.B1 = 1;		//TXD6はHIGH
	PORTB.PODR.BIT.B7 = 1;		//TXD9はHIGH
	PORTE.PODR.BIT.B1 = 1;		//TXD12はHIGH
	
	PORT3.PCR.BIT.B0 = 1;		//RXD1はプルアップ
	PORTA.PCR.BIT.B3 = 1;		//RXD5はプルアップ
	PORTB.PCR.BIT.B0 = 1;		//RXD6はプルアップ
	PORTB.PCR.BIT.B6 = 1;		//RXD9はプルアップ
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
	
	MPC.PA4PFS.BIT.PSEL = 10;	//PA4(CN1-16)をTXD5
	MPC.PA3PFS.BIT.PSEL = 10;	//PA3(CN1-15)をRXD5
	MPC.PB1PFS.BIT.PSEL = 11;	//PB1(CN2-2)をTXD6
	MPC.PB0PFS.BIT.PSEL = 11;	//PB0(CN2-1)をRXD6
	MPC.PB7PFS.BIT.PSEL = 10;	//PB7(CN2-6)をTXD9
	MPC.PB6PFS.BIT.PSEL = 10;	//PB6(CN2-5)をRXD9
	
	MPC.PE1PFS.BIT.PSEL = 12;	//PE1(CN1-8)をTXD12
	MPC.PE2PFS.BIT.PSEL = 12;	//PE2(CN1-9)をRXD12
	
	/***APIに合わせてMPCでピン割り付け***/
	
	//P**PFSレジスタへの書き込み禁止
	MPC.PWPR.BIT.PFSWE = 0;		

	/***ポートモードレジスタ設定(汎用入出力or周辺機能)。P**PFSのうち”PSEL”に対してのみPMRを定義***/
	PORT2.PMR.BIT.B6 = 1;		//P26(CN2-23)をTXD1
	PORT3.PMR.BIT.B0 = 1;		//P30(CN2-25)をRXD1
	
	PORTA.PMR.BIT.B4 = 1;		//PA4(CN1-16)をTXD5
	PORTA.PMR.BIT.B3 = 1;		//PA3(CN1-15)をRXD5
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)をTXD6
	PORTB.PMR.BIT.B0 = 1;		//PB0(CN2-1)をRXD6
	PORTB.PMR.BIT.B7 = 1;		//PB7(CN2-6)をTXD9
	PORTB.PMR.BIT.B6 = 1;		//PB6(CN2-5)をRXD9
	
	PORTE.PMR.BIT.B1 = 1;		//PE1(CN1-8)をTXD12
	PORTE.PMR.BIT.B2 = 1;		//PE2(CN1-9)をRXD12
	/***ポートモードレジスタ設定(汎用入出力or周辺機能)***/
}


void vdg_SCI_init()
{
	/***PC or Jetson との通信用***/
	SCI1.SCR.BYTE = 0x00;		//初期化
	SCI1.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	SCI1.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	SCI1.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI1.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI1.SNFR.BIT.NFCS = 4;		//8分周分のクロックをノイズフィルタに使用
	SCI1.BRR = 3;			//3:500000bps,9:200000bps
	SCI1.SCR.BIT.RE = 1;		//受信動作を許可
	SCI1.SCR.BIT.TE = 1;		//送信動作を許可
	SCI1.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI1.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	//SCI1.SCR.BIT.TEIE = 1;		//送信終了割込み要求を許可
	
	ICU.IER[0x1B].BIT.IEN4 = 1;	//送信割込み許可
	ICU.IER[0x1B].BIT.IEN3 = 1;	//受信割込み許可
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//送信完了割込み許可
	
	/***スレーブマイコン FL との通信用***/
	SCI5.SCR.BYTE = 0x00;		//初期化
	SCI5.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	SCI5.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	SCI5.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI5.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI5.SNFR.BIT.NFCS = 4;		//8分周分のクロックをノイズフィルタに使用
	SCI5.BRR = 3;			//3:500000bps,9:200000bps
	SCI5.SCR.BIT.RE = 1;		//受信動作を許可
	SCI5.SCR.BIT.TE = 1;		//送信動作を許可
	SCI5.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI5.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	//SCI5.SCR.BIT.TEIE = 1;		//送信終了割込み要求を許可
	
	ICU.IER[0x1C].BIT.IEN0 = 1;	//送信割込み許可
	//ICU.IER[0x1C].BIT.IEN1 = 1;	//送信終了割込み許可
	ICU.IER[0x1B].BIT.IEN7 = 1;	//受信割込み許可
	
	/***スレーブマイコン FR との通信用***/
	SCI6.SCR.BYTE = 0x00;		//初期化
	SCI6.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	SCI6.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	SCI6.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI6.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI6.SNFR.BIT.NFCS = 4;		//8分周分のクロックをノイズフィルタに使用
	SCI6.BRR = 3;			//3:500000bps,9:200000bps
	SCI6.SCR.BIT.RE = 1;		//受信動作を許可
	SCI6.SCR.BIT.TE = 1;		//送信動作を許可
	SCI6.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI6.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	//SCI6.SCR.BIT.TEIE = 1;		//送信終了割込み要求を許可
	
	ICU.IER[0x1C].BIT.IEN4 = 1;	//送信割込み許可
	//ICU.IER[0x1C].BIT.IEN5 = 1;	//送信終了割込み許可
	ICU.IER[0x1C].BIT.IEN3 = 1;	//受信割込み許可
	
	/***スレーブマイコン RL との通信用***/
	SCI9.SCR.BYTE = 0x00;		//初期化
	SCI9.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	SCI9.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	SCI9.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI9.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI9.SNFR.BIT.NFCS = 4;		//8分周分のクロックをノイズフィルタに使用
	SCI9.BRR = 3;			//3:500000bps,9:200000bps
	SCI9.SCR.BIT.RE = 1;		//受信動作を許可
	SCI9.SCR.BIT.TE = 1;		//送信動作を許可
	SCI9.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI9.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	//SCI9.SCR.BIT.TEIE = 1;		//送信終了割込み要求を許可
	
	ICU.IER[0x1D].BIT.IEN4 = 1;	//送信割込み許可
	//ICU.IER[0x1D].BIT.IEN5 = 1;	//送信終了割込み許可
	ICU.IER[0x1D].BIT.IEN3 = 1;	//受信割込み許可
	
	/***スレーブマイコン RR との通信用***/
	SCI12.SCR.BYTE = 0x00;		//初期化
	SCI12.SCR.BIT.CKE = 0;		//SCK端子はI/Oポートとして使用
	SCI12.SMR.BYTE = 0x00;		//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	SCI12.SEMR.BIT.ABCS = 1;		//基本クロック8サイクルで1ビット分
	SCI12.SEMR.BIT.NFEN = 1;		//ディジタルノイズフィルタ有効
	SCI12.SNFR.BIT.NFCS = 4;		//8分周分のクロックをノイズフィルタに使用
	SCI12.BRR = 7;			//3:500000bps,9:200000bps,7:250000bps
	SCI12.SCR.BIT.RE = 1;		//受信動作を許可
	SCI12.SCR.BIT.TE = 1;		//送信動作を許可
	SCI12.SCR.BIT.RIE = 1;		//受信及びエラー割込み要求を許可
	SCI12.SCR.BIT.TIE = 1;		//送信割込み要求を許可
	//SCI12.SCR.BIT.TEIE = 1;		//送信終了割込み要求を許可
	
	ICU.IER[0x1E].BIT.IEN0 = 1;	//送信割込み許可
	//ICU.IER[0x1E].BIT.IEN1 = 1;	//送信終了割込み許可
	ICU.IER[0x1D].BIT.IEN7 = 1;	//受信割込み許可
}

void vdg_CMT_init()
{
	CMT0.CMCR.BIT.CKS = 0;			//PCLK/8分周
	CMT0.CMCR.BIT.CMIE = 1;			//割込み許可
	CMT0.CMCOR = CNT_SLAVE_SET;		//1kHz
	//CMT.CMSTR0.BIT.STR0 = 1;		//カウント開始
	
	ICU.IER[0x03].BIT.IEN4 = 1;		//割込み許可
}

void vdg_TMR_init()
{
	
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
	//ICU.IPR[102].BIT.IPR = 13;	//S12ADI0外部割込み優先度
	ICU.IPR[218].BIT.IPR = 11;	//SCI1割込み優先度（送受信共通）
	ICU.IPR[222].BIT.IPR = 12;	//SCI5割込み優先度（送受信共通）
	ICU.IPR[226].BIT.IPR = 12;	//SCI6割込み優先度（送受信共通）
	ICU.IPR[234].BIT.IPR = 12;	//SCI9割込み優先度（送受信共通）
	ICU.IPR[238].BIT.IPR = 12;	//SCI12割込み優先度（送受信共通）
	
	ICU.IPR[004].BIT.IPR = 10;	//5kHz周期カウンタ割込み優先度
	
	//ICU.IPR[69].BIT.IPR = 15;	//IRQ5割込み優先度
}