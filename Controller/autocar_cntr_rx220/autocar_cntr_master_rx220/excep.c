
#include "iodefine.h"
#include "vect.h"
#include <machine.h>
#include "global_data.h"
#include "excep.h"

void Excep_CMT0_CMI0()
{
	u1g_cmt0_xperiod = 1;
}

void Excep_SCI1_TXI1()
{

}

void Excep_SCI1_RXI1()
{
	volatile static unsigned char u1s_rxi1_rccnt = 0;
	volatile static unsigned char u1s_rxi1_rdata;
	volatile static signed short s2s_rxi1_nmtgt = 0;
	
	volatile static unsigned  char u1s_rxi1_rccntmt = 0;
	
	
	u1s_rxi1_rdata = SCI1.RDR;
	
	if(u1s_rxi1_rdata != 'A')
	{
		switch(u1s_rxi1_rccnt)
		{
			/***100の位***/
			case 0:
			switch(u1s_rxi1_rdata)
			{
				case '0': s2s_rxi1_nmtgt = 0; break;
				case '1': s2s_rxi1_nmtgt = 100; break;
				case '2': s2s_rxi1_nmtgt = 200; break;
				case '3': s2s_rxi1_nmtgt = 300; break;
				case '4': s2s_rxi1_nmtgt = 400; break;
				case '5': s2s_rxi1_nmtgt = 500; break;
				case '6': s2s_rxi1_nmtgt = 600; break;
				case '7': s2s_rxi1_nmtgt = 700; break;
				case '8': s2s_rxi1_nmtgt = 800; break;
				case '9': s2s_rxi1_nmtgt = 900; break;
			}
			u1s_rxi1_rccnt++;
			break;
			
			/***10の位***/
			case 1:
			switch(u1s_rxi1_rdata)
			{
				case '0': break;
				case '1': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 10; break;
				case '2': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 20; break;
				case '3': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 30; break;
				case '4': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 40; break;
				case '5': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 50; break;
				case '6': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 60; break;
				case '7': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 70; break;
				case '8': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 80; break;
				case '9': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 90; break;
			}
			u1s_rxi1_rccnt++;
			break;
			
			/***1の位***/
			case 2:
			switch(u1s_rxi1_rdata)
			{
				case '0': break;
				case '1': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 1; break;
				case '2': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 2; break;
				case '3': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 3; break;
				case '4': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 4; break;
				case '5': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 5; break;
				case '6': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 6; break;
				case '7': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 7; break;
				case '8': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 8; break;
				case '9': s2s_rxi1_nmtgt = s2s_rxi1_nmtgt + 9; break;
			}
			u1s_rxi1_rccnt++;
			break;
			
			case 3:
			if(u1s_rxi1_rdata == '-') { s2s_rxi1_nmtgt = -1 * s2s_rxi1_nmtgt; }
			
			switch(u1s_rxi1_rccntmt)
			{
				case 0:
				s2g_rxi1_nmtgtfl = s2s_rxi1_nmtgt;
				u1s_rxi1_rccntmt++;
				break;
				
				case 1:
				s2g_rxi1_nmtgtfr = s2s_rxi1_nmtgt;
				u1s_rxi1_rccntmt++;
				break;
				
				case 2:
				s2g_rxi1_nmtgtrl = s2s_rxi1_nmtgt;
				u1s_rxi1_rccntmt++;
				break;
				
				case 3:
				s2g_rxi1_nmtgtrr = s2s_rxi1_nmtgt;
				u1s_rxi1_rccntmt = 0;
				u1g_rxi1_xrcnmt = 1;			//目標回転数受信完了フラグ
				break;
			}
			u1s_rxi1_rccnt = 0;
			
			break;
		}//u1s_rxi1_rccnt
	}
	
	else
	{
		u1g_rxi1_xmtorgrq = 1;		//原点学習開始要求
	}
}

 void Excep_SCI5_RXI5()
 {
	 volatile unsigned char u1t_rxi5_flmtrx;
	 
	 u1t_rxi5_flmtrx = SCI5.RDR;
	 
	 if(u1t_rxi5_flmtrx == 'Z') { u1g_rxi5_xflmtorgfin = 1; }
 }
 
 void Excep_SCI5_TXI5()
 {
	volatile static unsigned char u1s_rxi5_cnmset = 0;
	
	if(u1g_nmtset_xnmtset1 == 1)
	{
		u1s_rxi5_cnmset++;

		if(u1s_rxi5_cnmset < 4)
		{
			vdg_sci5_send(u2g_nmtset_nmtary1[u1s_rxi5_cnmset]);
		}
		else
		{
			u1s_rxi5_cnmset = 0;
			u1g_nmtset_xnmtset1 = 0;
		}
	}
 }
 
 void Excep_SCI6_RXI6()
 {
	 volatile unsigned char u1t_rxi6_flmtrx;
	 
	 u1t_rxi6_flmtrx = SCI6.RDR;
	 
	 if(u1t_rxi6_flmtrx == 'Z') { u1g_rxi6_xfrmtorgfin = 1; }
 }
 
 void Excep_SCI6_TXI6()
 {
	volatile static unsigned char u1s_rxi6_cnmset = 0;
	
	if(u1g_nmtset_xnmtset2 == 1)
	{
		u1s_rxi6_cnmset++;

		if(u1s_rxi6_cnmset < 4)
		{
			vdg_sci6_send(u2g_nmtset_nmtary2[u1s_rxi6_cnmset]);
		}
		else
		{
			u1s_rxi6_cnmset = 0;
			u1g_nmtset_xnmtset2 = 0;
		}
	}
 }
 
 void Excep_SCI9_RXI9()
 {
	 volatile unsigned char u1t_rxi9_flmtrx;
	 
	 u1t_rxi9_flmtrx = SCI9.RDR;
	 
	 if(u1t_rxi9_flmtrx == 'Z') { u1g_rxi9_xrlmtorgfin = 1; }
 }
 
 void Excep_SCI9_TXI9()
 {
	volatile static unsigned char u1s_rxi9_cnmset = 0;
	
	if(u1g_nmtset_xnmtset3 == 1)
	{
		u1s_rxi9_cnmset++;

		if(u1s_rxi9_cnmset < 4)
		{
			vdg_sci9_send(u2g_nmtset_nmtary3[u1s_rxi9_cnmset]);
		}
		else
		{
			u1s_rxi9_cnmset = 0;
			u1g_nmtset_xnmtset3 = 0;
		}
	}
 }
 


void Excep_SCI12_TXI12()
{
	volatile static unsigned char u1s_rxi12_cnmset = 0;
	
	if(u1g_nmtset_xnmtset4 == 1)
	{
		u1s_rxi12_cnmset++;
		
		if(u1s_rxi12_cnmset < 4)
		{
			vdg_sci12_send(u2g_nmtset_nmtary4[u1s_rxi12_cnmset]);
		}
		else
		{
			u1s_rxi12_cnmset = 0;
			u1g_nmtset_xnmtset4 = 0;
		}
	}
}

void Excep_SCI12_RXI12()
{
	 volatile unsigned char u1t_rxi12_flmtrx;
	 
	 u1t_rxi12_flmtrx = SCI12.RDR;
	 
	 if(u1t_rxi12_flmtrx == 'Z') { u1g_rxi12_xrrmtorgfin = 1; }
}