
#include "common.h"

/**********グローバル変数定義**********/
//なし

void vdg_scicnt_scisend(unsigned short senddata)
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

/***PCへシリアル通信で送信するための処理***/
// void vdg_scicnt_sciset()
// {	
// 	volatile unsigned char i;
	
// 	volatile unsigned short u2t_sciset_vtabs;
// 	volatile unsigned short u2t_sciset_nmtabs;
// 	volatile unsigned short u2t_sciset_nmabs;
	
// 	volatile static unsigned char u1s_sciset_sendnum;
	
// 	volatile static float f4s_sciset_vtgt;
// 	volatile static signed short s2s_sciset_nmtgt;
// 	volatile static float f4s_sciset_nmsm;
// 	volatile static unsigned short u2s_sciset_stringarray[10] = {0};

// 	switch(u1s_sciset_sendnum)
// 	{
// 		case 0:
// 		f4s_sciset_vtgt = f4g_mtcnt_vtgt;
// 		s2s_sciset_nmtgt = s2g_rxi12_nmtgt;
// 		f4s_sciset_nmsm = s4g_mtcnt_nmsm1;
		
// 		u2t_sciset_vtabs = (unsigned short)abs((int)(f4s_sciset_vtgt*10));
// 		u2t_sciset_nmtabs = (unsigned short)abs(s2s_sciset_nmtgt);
// 		u2t_sciset_nmabs = (unsigned short)abs((int)f4s_sciset_nmsm);
		
// 		u2s_sciset_stringarray[0] = u2t_sciset_vtabs / 100;
// 		u2s_sciset_stringarray[1] = (u2t_sciset_vtabs % 100) / 10;
// 		u2s_sciset_stringarray[2] = u2t_sciset_vtabs % 10;
		
// 		u2s_sciset_stringarray[3] = u2t_sciset_nmtabs / 100;
// 		u2s_sciset_stringarray[4] = (u2t_sciset_nmtabs % 100) / 10;
// 		u2s_sciset_stringarray[5] = u2t_sciset_nmtabs % 10;
		
// 		u2s_sciset_stringarray[6] = u2t_sciset_nmabs / 100;
// 		u2s_sciset_stringarray[7] = (u2t_sciset_nmabs % 100) / 10;
// 		u2s_sciset_stringarray[8] = u2t_sciset_nmabs % 10;
		
		
// 		if(f4s_sciset_vtgt < 0){ SCI1.TDR = '-';}
// 		else{SCI1.TDR = '+';}
// 		while(SCI1.SSR.BIT.TEND == 0);
// 		vdg_scicnt_scisend(u2s_sciset_stringarray[0]);
// 		u1s_sciset_sendnum++;
// 		break;
		
		
// 		case 1:
// 		for(i = 1; i < 3; i++)
// 		{
// 			vdg_scicnt_scisend(u2s_sciset_stringarray[i]);
// 			while(SCI1.SSR.BIT.TEND == 0);
// 		}
// 		u1s_sciset_sendnum++;
// 		break;
		
		
// 		case 2:
// 		if(s2s_sciset_nmtgt < 0){ SCI1.TDR = '-';}
// 		else{SCI1.TDR = '+';}
// 		while(SCI1.SSR.BIT.TEND == 0);
// 		vdg_scicnt_scisend(u2s_sciset_stringarray[3]);
// 		u1s_sciset_sendnum++;
// 		break;
		
		
// 		case 3:
// 		for(i = 4; i < 6; i++)
// 		{
// 			vdg_scicnt_scisend(u2s_sciset_stringarray[i]);
// 			while(SCI1.SSR.BIT.TEND == 0);
// 		}
// 		u1s_sciset_sendnum++;
// 		break;
		
		
// 		case 4:
// 		if(f4s_sciset_nmsm < 0){ SCI1.TDR = '-';}
// 		else{SCI1.TDR = '+';}
// 		while(SCI1.SSR.BIT.TEND == 0);
// 		vdg_scicnt_scisend(u2s_sciset_stringarray[6]);
// 		u1s_sciset_sendnum++;
// 		break;
		
		
// 		case 5:
// 		for(i = 7; i < 9; i++)
// 		{
// 			vdg_scicnt_scisend(u2s_sciset_stringarray[i]);
// 			while(SCI1.SSR.BIT.TEND == 0);
// 		}
// 		u1s_sciset_sendnum = 0;
// 		break;
// 	}
// }