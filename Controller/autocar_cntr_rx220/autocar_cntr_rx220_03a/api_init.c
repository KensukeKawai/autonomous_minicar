
#include "common.h"

//使用ハードウェア設定関数
void vdg_HW_init()
{
	/*****************************************/
	// ICLK = PCLKD = 32MHs,		 //
	// PCLKB = FCLK = BCLK = 32MHz		 //
	/*****************************************/
	
	/***プロテクト解除***/
   	SYSTEM.PRCR.WORD = 0xA503;					//クロック発生回路、消費電力低減機能関連レジスタへの書込み許可
	/***プロテクト解除***/

	/***サブクロック停止設定***/
	SYSTEM.SOSCCR.BYTE = 0x01;					//サブクロック発振器停止
	RTC.RCR3.BIT.RTCEN = 0;						//サブクロック発振器停止
	/***サブクロック停止設定***/
	
	/***HOCOクロックの発振設定***/
  	SYSTEM.HOCOCR2.BYTE = 0x00;					//高速オンチップオシレータHOCRの動作周波数=32MHz
	SYSTEM.HOCOWTCR2.BYTE = 0x14;				//HOCOクロック発生までの待機時間設定。180サイクル(約5.625us)待ち。
	SYSTEM.HOCOCR.BYTE = 0x00;					//HOCO動作。ここからHOCOWTCR2で設定した時間待たなければいけない
	vdg_wait_nop(100);							//9216サイクル分待ち
	/***HOCOクロックの発振設定***/
	
	/***内部クロック分周比設定***/
	SYSTEM.SCKCR.LONG = 0x0A;					//ｼｽﾃﾑｸﾛｯｸ(ICLK),周辺ﾓｼﾞｭｰﾙｸﾛｯｸB/D(PCLKB/D),外部ﾊﾞｽｸﾛｯｸ(BCLK),FlashIFｸﾛｯｸ)FCLK)を1分周
	//while (0x0A != SYSTEM.SCKCR.LONG){}		//SCKCRの書き込み完了してるか確認待ち
	/***内部クロック分周比設定***/
	
	/***システムクロック切り替え***/
	SYSTEM.SCKCR3.BIT.CKSEL = 1;				//HOCO選択
	//while (1 != SYSTEM.SCKCR3.BIT.CKSEL){}	//書き込まれてるか確認待ち
	/***システムクロック切り替え***/
	
	/***動作電力制御モード設定***/
	SYSTEM.OPCCR.BIT.OPCM = 2;					//中速動作モード1A
	while (SYSTEM.OPCCR.BIT.OPCM != 2){}		//遷移完了待ち
	/***動作電力制御モード設定***/
	
	/***モジュールストップ解除設定***/
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;				//MTUモジュールストップ状態解除
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;				//S12ADモジュールストップ状態解除
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;				//SCI1モジュールストップ状態解除
	SYSTEM.MSTPCRB.BIT.MSTPB17 = 0;				//RSPI0モジュールストップ状態解除
	//CMPAはアナログモジュールのため設定不要
	/***モジュールストップ解除設定***/
	
	/***プロテクト設定***/
	SYSTEM.PRCR.WORD = 0xA500;					//PRCRレジスタに、各レジスタへの書込み禁止設定
	/***プロテクト設定***/
}

//入出力設定関数
void vdg_IO_init()
{	
	PORT0.PDR.BYTE = 0xFF;
	PORT1.PDR.BYTE = 0x9F;		//P14(MTIOC3A)出力、P15(IRQ5)入力、P16(MOSIA)入力、P17(MISOA)出力
	PORT2.PDR.BYTE = 0xFF;		//P26(TXD1)出力
	PORT3.PDR.BYTE = 0xFA;		//P30(RXD1)入力、P32(IRQ2)入力
	PORT4.PDR.BYTE = 0xFC;		//P40(AN000)入力、P41(AN001)入力
	PORT5.PDR.BYTE = 0xFF;
	PORTA.PDR.BYTE = 0xA5;		//PA0(MTIOC4A)出力、PA1(MTCLKC)入力、PA3(IRQ6)入力、PA4(MTCLKA)入力、PA6(MTCLKB)入力
	PORTB.PDR.BYTE = 0xFE;		//PB0(RSPCKA)入力、PB1(MTIOC0C)出力、PB3(MTIOC0A)出力
	PORTC.PDR.BYTE = 0x4F;		//PC4(SSLA0)入力、PC5(MTCLKD)入力、PC6(MTIOC3C)出力、PC7(BOOT用)入力
	PORTE.PDR.BYTE = 0xFB;		//PE1(MTIOC4C)出力、PE2(IRQ7)入力
	PORTH.PDR.BYTE = 0xF9;		//PH1(IRQ0)入力、PH2(IRQ1)入力
	
	PORT2.PODR.BIT.B6 = 1;		//TXD1はHIGH
	PORT3.PCR.BIT.B0 = 1;		//RXD1はプルアップ
}

void vdg_MPC_init()
{
	/***MPC設定のために書き込みプロテクトレジスタPWPR設定***/
	MPC.PWPR.BIT.B0WI = 0;		//PFSWEビットに1を書き込むためにNOWIに0書き込み
	MPC.PWPR.BIT.PFSWE = 1;		//P**PFSに書き込むためにPFSWEに1を書き込み
	/***MPC設定のために書き込みプロテクトレジスタPWPR設定***/
	
	/***APIに合わせてMPCでピン割り付け***/
	MPC.PH1PFS.BIT.ISEL = 1;	//PH1(CN2-16)をIRQ0
	MPC.PH2PFS.BIT.ISEL = 1;	//PH2(CN2-17)をIRQ1
	MPC.P32PFS.BIT.ISEL = 1;	//P32(CN2-27)をIRQ2
	MPC.P15PFS.BIT.ISEL = 1;	//P15(CN2-20)をIRQ5
	MPC.PA3PFS.BIT.ISEL = 1;	//PA3(CN1-15)をIRQ6
	MPC.PE2PFS.BIT.ISEL = 1;	//PE2(CN1-09)をIRQ7
	
	MPC.PB3PFS.BIT.PSEL = 1;	//PB3(CN2-03)をMTIOC0A
	MPC.PB1PFS.BIT.PSEL = 1;	//PB1(CN2-02)をMTIOC0C
	MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)をMTIOC3A
	MPC.PC6PFS.BIT.PSEL = 1;	//PC6(CN2-11)をMTIOC3C
	MPC.PA0PFS.BIT.PSEL = 1;	//PA0(CN1-13)をMTIOC4A
	MPC.PE1PFS.BIT.PSEL = 1;	//PE1(CN1-08)をMTIOC4C

	MPC.PA4PFS.BIT.PSEL = 2;	//PA4(CN1-16)をMTCLKA
	MPC.PA6PFS.BIT.PSEL = 2;	//PA6(CN1-17)をMTCLKB
	MPC.PA1PFS.BIT.PSEL = 2;	//PA1(CN1-14)をMTCLKC
	MPC.PC5PFS.BIT.PSEL = 2;	//PC5(CN2-10)をMTCLKD

	MPC.P30PFS.BIT.PSEL = 10;	//P30(CN2-25)をRXD1
	MPC.P26PFS.BIT.PSEL = 10;	//P26(CN2-23)をTXD1

	MPC.PB0PFS.BIT.PSEL = 13;	//PB0(CN2-01)をRSPCKA
	MPC.P16PFS.BIT.PSEL = 13;	//P16(CN2-21)をMOSIA
	MPC.P17PFS.BIT.PSEL = 13;	//P17(CN2-22)をMISOA
	MPC.PC4PFS.BIT.PSEL = 13;	//PC4(CN2-09)をSSLA0

	MPC.P40PFS.BIT.ASEL = 1;	//P40(CN1-01)をAN000
	MPC.P41PFS.BIT.ASEL = 1;	//P41(CN1-00)をAN001

	//MPC.P14PFS.BIT.PSEL = 1;	//P14(CN2-19)をMTIOC3Aトグル出力
	/***APIに合わせてMPCでピン割り付け***/
	
	//P**PFSレジスタへの書き込み禁止
	MPC.PWPR.BIT.PFSWE = 0;		

	/***ポートモードレジスタ設定(汎用入出力or周辺機能)。P**PFSのうち”PSEL”に対してのみPMRを定義***/
	PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-03)をMTIOC0A
	PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-02)をMTIOC0C
	PORT1.PMR.BIT.B4 = 1;		//P14(CN2-19)をMTIOC3A
	PORTC.PMR.BIT.B6 = 1;		//PC6(CN2-11)をMTIOC3C
	PORTA.PMR.BIT.B0 = 1;		//PA0(CN1-13)をMTIOC4A
	PORTE.PMR.BIT.B1 = 1;		//PE1(CN1-08)をMTIOC4C

	PORTA.PMR.BIT.B4 = 1;		//PA4(CN1-16)をMTCLKA
	PORTA.PMR.BIT.B6 = 1;		//PA6(CN1-17)をMTCLKB
	PORTA.PMR.BIT.B1 = 1;		//PA1(CN1-14)をMTCLKC
	PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)をMTCLKD

	PORT3.PMR.BIT.B0 = 1;		//P30(CN2-25)をRXD1
	PORT2.PMR.BIT.B6 = 1;		//P26(CN2-23)をTXD1

	PORTB.PMR.BIT.B0 = 1;		//PB0(CN2-01)をRSPCKA
	PORT1.PMR.BIT.B6 = 1;		//P16(CN2-21)をMOSIA
	PORT1.PMR.BIT.B7 = 1;		//P17(CN2-22)をMISOA
	PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-09)をSSLA0
	/***ポートモードレジスタ設定(汎用入出力or周辺機能)***/
}

void vdg_MTU2a_init()
{
	/**************PWM1モード設定*****************/
	MTU.TSYR.BIT.SYNC0 = 1;		//MTU0.TCNTは同期動作
	MTU.TSYR.BIT.SYNC3 = 1;		//MTU3.TCNTは同期動作
	MTU.TSYR.BIT.SYNC4 = 1;		//MTU4.TCNTは同期動作

	MTU0.TCR.BYTE = 0x20;		//00100000：PCLK/1,1分周,TGRAコンペアマッチでTCNTクリア
	MTU3.TCR.BYTE = 0x60;		//01100000：PCLK/1,1分周,MTU0のカウンタクリアでTCNTクリア
	MTU4.TCR.BYTE = 0x60;		//01100000：PCLK/1,1分周,MTU0のカウンタクリアでTCNTクリア

	// MTU0.TCR.BYTE = 0x20;		//00100000：PCLK/1,1分周,TGRAコンペアマッチでTCNTクリア
	// MTU3.TCR.BYTE = 0x20;		//01100000：PCLK/1,1分周,MTU0のカウンタクリアでTCNTクリア
	// MTU4.TCR.BYTE = 0x20;		//01100000：PCLK/1,1分周,MTU0のカウンタクリアでTCNTクリア
	
	MTU0.TMDR.BIT.MD = 2;		//0010：PWM1モード
	MTU3.TMDR.BIT.MD = 2;		//0010：PWM1モード
	MTU4.TMDR.BIT.MD = 2;		//0010：PWM1モード

	MTU0.TIORH.BYTE = 0x21;		//MTIOC0A初期LoマッチHi,MTIOC0B出力禁止
	MTU0.TIORL.BYTE = 0x21;		//MTIOC0C初期LoマッチHi,MTIOC0D出力禁止
	MTU3.TIORH.BYTE = 0x21;		//MTIOC3A初期LoマッチHi,MTIOC3B出力禁止
	MTU3.TIORL.BYTE = 0x21;		//MTIOC3C初期LoマッチHi,MTIOC3D出力禁止
	MTU4.TIORH.BYTE = 0x21;		//MTIOC4A初期LoマッチHi,MTIOC4B出力禁止
	MTU4.TIORL.BYTE = 0x21;		//MTIOC4C初期LoマッチHi,MTIOC4D出力禁止

	MTU0.TIER.BIT.TGIEA = 1;	//TGIA0(MTU0.TGRA)のコンペアマッチ割込み要求許可
	MTU0.TIER.BIT.TTGE = 1;		//MTU0.TGRAコンペアマッチでADC開始要求許可

	MTU0.TGRA = CNT_CARRIER;	//キャリアカウント設定
	MTU3.TGRA = CNT_CARRIER;	//キャリアカウント設定
	MTU4.TGRA = CNT_CARRIER;	//キャリアカウント設定
	MTU0.TGRC = CNT_CARRIER;	//キャリアカウント設定
	MTU3.TGRC = CNT_CARRIER;	//キャリアカウント設定
	MTU4.TGRC = CNT_CARRIER;	//キャリアカウント設定

	MTU0.TGRB = CNT_CARRIER + 10;	//初期はキャリアカウント数より大きくしDuty0%
	MTU3.TGRB = CNT_CARRIER + 10;	//初期はキャリアカウント数より大きくしDuty0%
	MTU4.TGRB = CNT_CARRIER + 10;	//初期はキャリアカウント数より大きくしDuty0%
	MTU0.TGRD = CNT_CARRIER + 10;	//初期はキャリアカウント数より大きくしDuty0%
	MTU3.TGRD = CNT_CARRIER + 10;	//初期はキャリアカウント数より大きくしDuty0%
	MTU4.TGRD = CNT_CARRIER + 10;	//初期はキャリアカウント数より大きくしDuty0%

	//割込み許可設定
	ICU.IER[0x0E].BIT.IEN2 = 1;		//TGIA0割込み許可

//	MTU.TSTR.BYTE = 0xC1;		//MTU0.TCNT,MTU3.TCNT,MTU4.TCNTのカウント動作開始
	/**************PWM1モード設定*****************/

	/**************位相計数モード設定*****************/
	MTU1.TMDR.BIT.MD = 4;		//位相計数モード1
	MTU2.TMDR.BIT.MD = 4;		//位相計数モード1
	MTU1.TIER.BIT.TCIEV = 1;	//MTU1位相計数オーバーフロー割込み許可
	MTU1.TIER.BIT.TCIEU = 1;	//MTU1位相計数アンダーフロー割込み許可
	MTU2.TIER.BIT.TCIEV = 1;	//MTU2位相計数オーバーフロー割込み許可
	MTU2.TIER.BIT.TCIEU = 1;	//MTU2位相計数アンダーフロー割込み許可
	//MTU.TSTR.BYTE = 0X06;		//MTU1,2の位相計数動作開始
	/**************位相計数モード設定*****************/

	//MTU.TSTR.BYTE = 0xC7;		//MTU0,1,2,3,4のTCNTカウント開始
}

void vdg_S12AD_init()
{
	S12AD.ADCSR.BIT.ADCS = 1;		//グループスキャンモード
	S12AD.ADCSR.BIT.ADIE = 1;		//スキャン終了割込み許可→割込みで完了フラグONにするだけ
	S12AD.ADANSA.WORD = 0x03;		//AN000,AN001を変換対象にする(グループA)
	S12AD.ADCER.BIT.ACE = 1;		//データ読み出しによる自動クリアを許可
	S12AD.ADSTRGR.BIT.TRSA = 1;		//要因：TRG0AN。MTU0.TGRAコンペアマッチでADC開始トリガ設定
	S12AD.ADCSR.BIT.TRGE = 1;		//トリガ(MTU0.TCNTコンペアマッチ)によるADC開始許可

	ICU.IER[0x0C].BIT.IEN6 = 1;		//S12ADのS12ADI0割込み要求を許可
}

void vdg_SCI_init()
{
	/***PCとの通信用***/
	SCI1.SCR.BYTE = 0x00;			//初期化
	SCI1.SCR.BIT.CKE = 0;			//SCK端子はI/Oポートとして使用
	//SCI1.SIMR1.BIT.IICM = 0;		//シリアルインターフェースモード
	SCI1.SMR.BYTE = 0x00;			//PCLK、1ストップビット、パリティ無、データ長8bit、調歩同期式
	//SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = 1;			//基本クロック8サイクルで1ビット分
	SCI1.SEMR.BIT.NFEN = 1;			//ディジタルノイズフィルタ有効
	SCI1.SNFR.BIT.NFCS = 4;			//8分周分のクロックをノイズフィルタに使用
	SCI1.BRR = 9;					//3:500000bps,9:200000bps
	SCI1.SCR.BIT.RE = 1;			//受信動作を許可
	SCI1.SCR.BIT.TE = 1;			//送信動作を許可
	SCI1.SCR.BIT.RIE = 1;			//受信及びエラー割込み要求を許可
	SCI1.SCR.BIT.TIE = 1;			//送信割込み要求を許可
	
	ICU.IER[0x1B].BIT.IEN4 = 1;		//送信割込み許可
	ICU.IER[0x1B].BIT.IEN3 = 1;		//受信割込み許可
	//ICU.IER[0x1B].BIT.IEN5 = 1;	//送信完了割込み許可
}

void vdg_SPI_init()
{
	//SSLPレジスタ設定
	RSPI0.SSLP.BYTE = 0x00;			//SSL0~3のチップセレクト時極性を「アクティブLow」
	//SPDCRレジスタ設定
	RSPI0.SPDCR.BIT.SPFC = 1;		//フレーム数1（使用するSPCMD*の数を指定する）
	RSPI0.SPDCR.BIT.SPRDTD = 0;		//SPDRの読み出し先バッファを受信バッファに設定
	RSPI0.SPDCR.BIT.SPLW = 1;		//SPDRレジスタへロングワードアクセス
	//SPCR2レジスタ設定
	RSPI0.SPCR2.BIT.SPPE = 0;		//送信データにパリティビット付与しない、受信データにパリティチェックを行わない
	RSPI0.SPCR2.BIT.SPIIE =  0;		//アイドル割込み要求の発生を禁止
	//SPCMD0レジスタ設定
	RSPI0.SPCMD0.BIT.CPHA = 0;		//SPIモード0
	RSPI0.SPCMD0.BIT.CPOL = 0;		//アイドル時のRSPCKをLow設定
	RSPI0.SPCMD0.BIT.SSLA = 0;		//スレーブモードのため0設定
	RSPI0.SPCMD0.BIT.SSLKP = 0;		//スレーブモードのため0設定
	RSPI0.SPCMD0.BIT.SPB = 2;		//データ長を32bit
	RSPI0.SPCMD0.BIT.LSBF = 0;		//MSBファースト
	
	//割込みコントローラの設定
	ICU.IER[0x05].BIT.IEN5 = 1;		//受信バッファフル割込み許可

	//SPCRレジスタ設定
	RSPI0.SPCR.BYTE = 0xc0;
	//上記0xC0の内訳は以下
	// RSPI0.SPCR.BIT.SPMS = 0;		//SPI動作
	// RSPI0.SPCR.BIT.TXMD = 0;		//全二重同期式シリアル通信
	// RSPI0.SPCR.BIT.MODFEN = 0;		//モードフォルトエラー検出を禁止
	// RSPI0.SPCR.BIT.MSTR = 0;		//スレーブモード
	// RSPI0.SPCR.BIT.SPEIE = 0;		//RSPIエラー割込み要求の発生を禁止
	// RSPI0.SPCR.BIT.SPTIE = 0;		//送信割込み要求の発生を禁止
	// RSPI0.SPCR.BIT.SPRIE = 1;		//受信割込み要求の発生を許可
	// RSPI0.SPCR.BIT.SPE = 1;			//RSPI機能を有効

	//ロングワードアクセスする場合は以下で行う
	//RSPI0.SPDR.LONG
}

void vdg_IRQ_init()
{
	//使用する番号は0,1,2,5,6,7
	//外部割込みはインタラプタもしくはホールセンサのどちらか
	//いずれも、両エッジトリガ検出
	ICU.IER[0x08].BYTE = 0x00;		//全IRQを割込み要求禁止
	ICU.IRQFLTE0.BYTE = 0x00;		//全IRQをディジタルフィルタ無効
	//ディジタルフィルタは以下設定クロック3回分連続一致で検出
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 1;	//サンプリングクロックPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL1 = 1;	//サンプリングクロックPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL2 = 1;	//サンプリングクロックPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL5 = 1;	//サンプリングクロックPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL6 = 1;	//サンプリングクロックPCLK/8
	ICU.IRQFLTC0.BIT.FCLKSEL7 = 1;	//サンプリングクロックPCLK/8
	//全て両エッジ検出
	ICU.IRQCR[0].BIT.IRQMD = 3;		//両エッジ検出
	ICU.IRQCR[1].BIT.IRQMD = 3;		//両エッジ検出
	ICU.IRQCR[2].BIT.IRQMD = 3;		//両エッジ検出
	ICU.IRQCR[5].BIT.IRQMD = 3;		//両エッジ検出
	ICU.IRQCR[6].BIT.IRQMD = 3;		//両エッジ検出
	ICU.IRQCR[7].BIT.IRQMD = 3;		//両エッジ検出
	//ベクタ番号ごとに割込み要求フラグ0書き込み
	ICU.IR[64].BIT.IR = 0;			//IRQ0割込み要求フラグ"0"（[]内はベクタ番号）
	ICU.IR[65].BIT.IR = 0;			//IRQ1割込み要求フラグ"0"（[]内はベクタ番号）
	ICU.IR[66].BIT.IR = 0;			//IRQ2割込み要求フラグ"0"（[]内はベクタ番号）
	ICU.IR[69].BIT.IR = 0;			//IRQ5割込み要求フラグ"0"（[]内はベクタ番号）
	ICU.IR[70].BIT.IR = 0;			//IRQ6割込み要求フラグ"0"（[]内はベクタ番号）
	ICU.IR[71].BIT.IR = 0;			//IRQ7割込み要求フラグ"0"（[]内はベクタ番号）
	//ディジタルフィルタ有効
	ICU.IRQFLTE0.BYTE = 0xE7;		//ディジタルフィルタ有効

	//IRQ割込み要求許可。外部割込み使用する場合は以下行を有効化
	// ICU.IER[0x08].BYTE = 0xE7;		//IRQ割込み要求許可
}

void vdg_IPR_init()
{
	ICU.IPR[114].BIT.IPR = 15;		//TGIA0(MTU0.TGRA)のコンペアマッチ割込み優先度
	ICU.IPR[102].BIT.IPR = 13;		//S12ADI0外部割込み優先度
	// ICU.IPR[218].BIT.IPR = 12;		//SCI1割込み優先度（送受信共通）
	ICU.IPR[44].BIT.IPR = 0;		//SPI受信割込み優先度
	// ICU.IPR[64].BIT.IPR = 1;		//IRQ0割込み優先度
	// ICU.IPR[65].BIT.IPR = 1;		//IRQ1割込み優先度
	// ICU.IPR[66].BIT.IPR = 1;		//IRQ2割込み優先度
	// ICU.IPR[69].BIT.IPR = 1;		//IRQ5割込み優先度
	// ICU.IPR[70].BIT.IPR = 1;		//IRQ6割込み優先度
	// ICU.IPR[71].BIT.IPR = 1;		//IRQ7割込み優先度
}