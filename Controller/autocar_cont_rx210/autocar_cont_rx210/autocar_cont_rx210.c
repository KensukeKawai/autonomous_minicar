
#include <iodefine.h>
#include <vect.h>
#include <mathf.h>
#include <machine.h>
#include "global_data.h"


//使用ハードウェア設定関数
void vdg_HW_init()
{
	/*****************************************/
	// ICLK = PCLKD = 50MHs,		 //
	// PCLKB = FCLK = BCLK = 25MHz		 //
	/*****************************************/
	
	unsigned int i;
   
   	SYSTEM.PRCR.WORD = 0xA507;			//クロック発生回路関連レジスタ、動作モード関連レジスタ、VRCRレジスタへの書込み許可
    
//  	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;			//MTU2aモジュールストップ状態解除
 	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;			//SCI1モジュールストップ状態解除
	SYSTEM.MSTPCRA.BIT.MSTPA5 = 0;			//TMR0モジュールストップ状態解除
    
 	SYSTEM.VRCR = 0x00;				//VRCRレジスタ（電圧レギュレータ制御レジスタ）へ0x00
  	SYSTEM.HOCOCR2.BYTE = 0x03;			//高速オンチップオシレータHOCRの動作周波数=50MHz
    	SYSTEM.HOCOWTCR2.BYTE = 0x03;			//HOCOクロック発生までの待機時間設定
    	SYSTEM.HOCOCR.BYTE = 0x00;			//HOCO動作。ここからHOCOWTCR2で設定した時間待たなければいけない
    	for(i=0;i<100;i++){ nop();}			//9216サイクル分待ち
	SYSTEM.SCKCR.LONG = 0x10811110;   		//ICLK,PCLKD: no division PCLKB,BCLK,FCLK: divide-by-2 */
	while (0x10811110 != SYSTEM.SCKCR.LONG){}	//SCKCRの書き込み完了してるか確認待ち
	SYSTEM.BCKCR.BYTE = 0x01;
	while (0x01 != SYSTEM.BCKCR.BYTE){}
	SYSTEM.SCKCR3.WORD = 0x0100;			//HOCO選択
	while (0x0100 != SYSTEM.SCKCR3.WORD){}		//書き込まれてるか確認待ち
	SYSTEM.PRCR.WORD = 0xA500;			//PRCRレジスタに、各レジスタへの書込み禁止設定
    
	while(0 != SYSTEM.OPCCR.BIT.OPCMTSF){}		//高速動作モード移行の遷移状態フラグ監視
	SYSTEM.OPCCR.BIT.OPCM = 0;			//高速動作モード
}

//入出力設定関数
void vdg_IO_init()
{	
	//PORT0.PDR.BYTE = 0xFF;
	PORT1.PDR.BYTE = 0xFF;
	PORT2.PDR.BYTE = 0xFD;		//P21(RXD0)を入力。RS232Cレベル変換を使用しない限りP21は入力設定
	PORT3.PDR.BYTE = 0xDE;		//P30(RXD1),P35(入力設定のみ)のみ入力
	//PORT4.PDR.BYTE = 0xFF;
	PORT5.PDR.BYTE = 0xFF;
	PORTA.PDR.BYTE = 0xFF;
	PORTB.PDR.BYTE = 0xFF;
	PORTC.PDR.BYTE = 0xFF;
	//PORTD.PDR.BYTE = 0xFF;
	PORTE.PDR.BYTE = 0xFF;
	//PORTH.PDR.BYTE = 0xFF;
	//PORTJ.PDR.BYTE = 0xFF;
	
	//インタラプタ入力ポートを入力設定
	MOTOR1_IRQ = 0;
	
	//MPCの書き込みプロテクトレジスタPWPR設定
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	
	MPC.PH1PFS.BIT.ISEL = 1;	//IRQ0として使用 CN1-25
	
	MPC.P26PFS.BIT.PSEL = 10;	//TXD1として使用
	MPC.P30PFS.BIT.PSEL = 10;	//RXD1として使用
	
	MPC.PWPR.BIT.PFSWE = 0;		//P**PFSレジスタへの書き込み禁止

	//ポートモードレジスタ設定。汎用入出力or周辺機能
	/*** SCI1 ***/
	PORT2.PMR.BIT.B6 = 1;
	PORT3.PMR.BIT.B0 = 1;
}

void vdg_TMR_init()
{
//	TMR0.TCORA
	TMR0.TCR.BIT.CCLR = 1;	//コンペアマッチAによりクリア
	TMR0.TCCR.BIT.CKS = 0;	//分周クロックをPCLKでカウント
	TMR0.TCCR.BIT.CSS = 2;	//分周クロックをPCLKでカウント
	
	//TMR0.TCR.BIT.OVIE = 1;	//オーバーフローによる割込みを許可
	TMR0.TCR.BIT.CMIEA = 1;	//コンペアマッチAによる割込み要求を許可
	ICU.IER[0x15].BIT.IEN6 = 1;	//TMR0コンペアマッチA割込み要求許可
	TMR0.TCORA = 49;	//1MHzで割込み発生。f = PCLK/(N+1)のため、24。
}

void IRQ_init()
{
	ICU.IER[0x08].BYTE = 0x00;
	ICU.IRQFLTE0.BYTE = 0x00;
	ICU.IRQFLTC0.WORD = 0x0000;

	ICU.IRQCR[0].BIT.IRQMD = 2;	//IRQ0立ち上がりエッジ検出
	ICU.IR[64].BIT.IR = 0;		//IRQ0割込み要求フラグ"0"
	ICU.IRQFLTE0.BIT.FLTEN0 = 1;	//ディジタルフィルタ有効
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 1;	//サンプリングクロックPCLK/8
	ICU.IER[0x08].BIT.IEN0 = 1;	//IRQ0割込み要求許可
}

void IPR_init()
{
	ICU.IPR[64].BIT.IPR = 6;	//IRQ0外部割込み優先度
	ICU.IPR[218].BIT.IPR = 12;	//SCI1割込み優先度（送受信共通）
	ICU.IPR[174].BIT.IPR = 1;	//TMR0コンペアマッチA割込み優先度
}

void wait_nop()
{
	unsigned long i;
	for(i=0;i<20000000;i++){ nop();}
}

void SCI1_init()
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

void Excep_TMR0_CMIA0()
{
	u1g_tmr_trg = 1;
	
	if(u1g_tricntr_dir == CNTUP)
	{
		s2g_tricntr++;
	}
	else
	{
		s2g_tricntr--;
	}
	
	if(s2g_tricntr >= CAREER_CNT)
	{
		u1g_tricntr_dir = CNTDOWN;		//三角波トップ到達したらカウントダウン
		TRICNT_TRG_UP = !TRICNT_TRG_UP;
	}
	else if(s2g_tricntr <= 0)
	{
		u1g_tricntr_dir = CNTUP;
		//MOTOR1_VL_OUT = !MOTOR1_VL_OUT;
	}
	
}

void Excep_SCI1_RXI1()
{
}

void Excep_SCI1_TXI1()
{
}

void Excep_ICU_IRQ0()
{
	
}


void Variable_init()
{
	u1g_tricntr_dir = CNTUP;
	s2g_motor1_UH_cnt = 25;
	s2g_motor1_UL_cnt = 30;
	s2g_motor1_VH_cnt = 25;
	s2g_motor1_VL_cnt = 30;
	s2g_motor1_WH_cnt = 25;
	s2g_motor1_WL_cnt = 30;
}

void vdg_motor_set()
{
	if(s2g_tricntr <= s2g_motor1_UH_cnt)
	{
		MOTOR1_UH_OUT = 1;
	}
	else
	{
		MOTOR1_UH_OUT = 0;
	}
	if(s2g_tricntr >= s2g_motor1_UL_cnt)
	{
		MOTOR1_UL_OUT = 1;
	}
	else
	{
		MOTOR1_UL_OUT = 0;
	}
	if(s2g_tricntr <= s2g_motor1_VH_cnt)
	{
		MOTOR1_VH_OUT = 1;
	}
	else
	{
		MOTOR1_VH_OUT = 0;
	}
	if(s2g_tricntr >= s2g_motor1_VL_cnt)
	{
		MOTOR1_VL_OUT = 1;
	}
	else
	{
		MOTOR1_VL_OUT = 0;
	}
	if(s2g_tricntr <= s2g_motor1_WH_cnt)
	{
		MOTOR1_WH_OUT = 1;
	}
	else
	{
		MOTOR1_WH_OUT = 0;
	}
	if(s2g_tricntr >= s2g_motor1_WL_cnt)
	{
		MOTOR1_WL_OUT = 1;
	}
	else
	{
		MOTOR1_WL_OUT = 0;
	}
}

void main()
{
	vdg_HW_init();
	vdg_IO_init();
	Variable_init();
	vdg_TMR_init();
	//IRQ_init();
	IPR_init();
	//SCI1_init();


	while(1)
	{
		if(u1g_tmr_trg == 1)
		{
			u1g_tmr_trg = 0;
			SET_TRG = 1;
			vdg_motor_set();
			SET_TRG = 0;
		}
	}//while
	
}//main