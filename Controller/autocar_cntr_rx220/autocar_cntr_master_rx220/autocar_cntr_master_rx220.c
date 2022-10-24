
#include "iodefine.h"
#include "vect.h"
#include "mathf.h"
#include "machine.h"
#include "global_data.h"
#include "api_init.h"
#include "excep.h"

/**********グローバル変数定義**********/

volatile unsigned char u1g_rxi1_xmtorgrq;
volatile unsigned char u1g_rxi1_xrcnmt;
volatile signed short s2g_rxi1_nmtgtfl;
volatile signed short s2g_rxi1_nmtgtfr;
volatile signed short s2g_rxi1_nmtgtrl;
volatile signed short s2g_rxi1_nmtgtrr;

/////////////
volatile unsigned char u1g_rxi5_xflmtorgfin;
volatile unsigned char u1g_rxi6_xfrmtorgfin;
volatile unsigned char u1g_rxi9_xrlmtorgfin;
volatile unsigned char u1g_rxi12_xrrmtorgfin;

volatile unsigned char u1g_slaveinit_xmtorgfin;

volatile unsigned char u1g_cmt0_xperiod;
volatile unsigned char u1g_nmtset_xnmtset1;
volatile unsigned char u1g_nmtset_xnmtset2;
volatile unsigned char u1g_nmtset_xnmtset3;
volatile unsigned char u1g_nmtset_xnmtset4;

volatile unsigned short u2g_nmtset_nmtary1[4] = {0};
volatile unsigned short u2g_nmtset_nmtary2[4] = {0};
volatile unsigned short u2g_nmtset_nmtary3[4] = {0};
volatile unsigned short u2g_nmtset_nmtary4[4] = {0};

/**********グローバル変数定義**********/

void vdg_wait_nop(unsigned long N)
{
	unsigned long i;
	for(i=0;i<N;i++){ nop();}
}

void vdg_variable_init()
{
	
}

void vdg_sci1_send(unsigned short senddata)
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

void vdg_sci5_send(unsigned short senddata)
{
	switch(senddata)
	{
		case 0: SCI5.TDR = '0'; break;
		case 1: SCI5.TDR = '1'; break;
		case 2: SCI5.TDR = '2'; break;
		case 3: SCI5.TDR = '3'; break;
		case 4: SCI5.TDR = '4'; break;
		case 5: SCI5.TDR = '5'; break;
		case 6: SCI5.TDR = '6'; break;
		case 7: SCI5.TDR = '7'; break;
		case 8: SCI5.TDR = '8'; break;
		case 9: SCI5.TDR = '9'; break;
	}
}

void vdg_sci6_send(unsigned short senddata)
{
	switch(senddata)
	{
		case 0: SCI6.TDR = '0'; break;
		case 1: SCI6.TDR = '1'; break;
		case 2: SCI6.TDR = '2'; break;
		case 3: SCI6.TDR = '3'; break;
		case 4: SCI6.TDR = '4'; break;
		case 5: SCI6.TDR = '5'; break;
		case 6: SCI6.TDR = '6'; break;
		case 7: SCI6.TDR = '7'; break;
		case 8: SCI6.TDR = '8'; break;
		case 9: SCI6.TDR = '9'; break;
	}
}

void vdg_sci9_send(unsigned short senddata)
{
	switch(senddata)
	{
		case 0: SCI9.TDR = '0'; break;
		case 1: SCI9.TDR = '1'; break;
		case 2: SCI9.TDR = '2'; break;
		case 3: SCI9.TDR = '3'; break;
		case 4: SCI9.TDR = '4'; break;
		case 5: SCI9.TDR = '5'; break;
		case 6: SCI9.TDR = '6'; break;
		case 7: SCI9.TDR = '7'; break;
		case 8: SCI9.TDR = '8'; break;
		case 9: SCI9.TDR = '9'; break;
	}
}

void vdg_sci12_send(unsigned short senddata)
{
	switch(senddata)
	{
		case 0: SCI12.TDR = '0'; break;
		case 1: SCI12.TDR = '1'; break;
		case 2: SCI12.TDR = '2'; break;
		case 3: SCI12.TDR = '3'; break;
		case 4: SCI12.TDR = '4'; break;
		case 5: SCI12.TDR = '5'; break;
		case 6: SCI12.TDR = '6'; break;
		case 7: SCI12.TDR = '7'; break;
		case 8: SCI12.TDR = '8'; break;
		case 9: SCI12.TDR = '9'; break;
	}
}

void vdg_nmtset()
{
	u2g_nmtset_nmtary1[0] = (unsigned short)abs((int)(s2g_rxi1_nmtgtfl) / 100);
	u2g_nmtset_nmtary1[1] = (unsigned short)abs(((int)(s2g_rxi1_nmtgtfl) % 100) / 10);
	u2g_nmtset_nmtary1[2] = (unsigned short)abs((int)(s2g_rxi1_nmtgtfl) % 10);
	if(s2g_rxi1_nmtgtfl < 0) { u2g_nmtset_nmtary1[3] = 1; }
	else { u2g_nmtset_nmtary1[3] = 0; }
	
	u2g_nmtset_nmtary2[0] = (unsigned short)abs((int)(s2g_rxi1_nmtgtfr) / 100);
	u2g_nmtset_nmtary2[1] = (unsigned short)abs(((int)(s2g_rxi1_nmtgtfr) % 100) / 10);
	u2g_nmtset_nmtary2[2] = (unsigned short)abs((int)(s2g_rxi1_nmtgtfr) % 10);
	if(s2g_rxi1_nmtgtfr < 0) { u2g_nmtset_nmtary2[3] = 1; }
	else { u2g_nmtset_nmtary2[3] = 0; }
	
	u2g_nmtset_nmtary3[0] = (unsigned short)abs((int)(s2g_rxi1_nmtgtrl) / 100);
	u2g_nmtset_nmtary3[1] = (unsigned short)abs(((int)(s2g_rxi1_nmtgtrl) % 100) / 10);
	u2g_nmtset_nmtary3[2] = (unsigned short)abs((int)(s2g_rxi1_nmtgtrl) % 10);
	if(s2g_rxi1_nmtgtrl < 0) { u2g_nmtset_nmtary3[3] = 1; }
	else { u2g_nmtset_nmtary3[3] = 0; }
	
	u2g_nmtset_nmtary4[0] = (unsigned short)abs((int)(s2g_rxi1_nmtgtrr) / 100);
	u2g_nmtset_nmtary4[1] = (unsigned short)abs(((int)(s2g_rxi1_nmtgtrr) % 100) / 10);
	u2g_nmtset_nmtary4[2] = (unsigned short)abs((int)(s2g_rxi1_nmtgtrr) % 10);
	if(s2g_rxi1_nmtgtrr < 0) { u2g_nmtset_nmtary4[3] = 1; }
	else { u2g_nmtset_nmtary4[3] = 0; }
	
	//TXD割込み関数にnmtgtセットであることを知らせる
	u1g_nmtset_xnmtset1 = 1;
	u1g_nmtset_xnmtset2 = 1;
	u1g_nmtset_xnmtset3 = 1;
	u1g_nmtset_xnmtset4 = 1;
	
	vdg_sci5_send(u2g_nmtset_nmtary1[0]);
	vdg_sci6_send(u2g_nmtset_nmtary2[0]);
	vdg_sci9_send(u2g_nmtset_nmtary3[0]);
	vdg_sci12_send(u2g_nmtset_nmtary4[0]);
}

void vdg_slave_init()
{
//	SCI5.TDR = 'A';
//	SCI6.TDR = 'A';
//	SCI9.TDR = 'A';
	SCI12.TDR = 'A';
	
	/***各スレーブから原点学習完了連絡来るまで待つ***/
	//while(u1g_rxi5_xflmtorgfin == 0);
	//while(u1g_rxi6_xfrmtorgfin == 0);
	//while(u1g_rxi9_xrlmtorgfin == 0);
	while(u1g_rxi12_xrrmtorgfin == 0);
	
	u1g_slaveinit_xmtorgfin = 1;
	
	SCI1.TDR = 'Z';
}

void main()
{
	vdg_HW_init();
	vdg_IO_init();
	vdg_MPC_init();
	vdg_variable_init();
	
	vdg_SCI_init();
	vdg_CMT_init();
	vdg_IPR_init();
	
	vdg_slave_init();
	CMT.CMSTR0.BIT.STR0 = 1;		//カウント開始
	
	while(1)
	{
		if(u1g_cmt0_xperiod == 1)
		{
			if(u1g_rxi1_xrcnmt == 1)
			{
				TRG1 = 1;
				u1g_rxi1_xrcnmt = 0;
				vdg_nmtset();		//スレーブにnmtgt送信
				TRG1 = 0;
			}
			u1g_cmt0_xperiod = 0;
		}
	}//while
	
}//main