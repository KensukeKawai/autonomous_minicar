
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
		case 13: SCI1.TDR = 13; break;
	}
}

/***PCへシリアル通信で送信するための処理***/
void vdg_scicnt_sciset()
{	
	volatile unsigned char i;
	
	volatile unsigned short u2t_sciset_vtabs;
	volatile unsigned short u2t_sciset_nmtabs;
	volatile unsigned short u2t_sciset_nmabs;
	
	volatile static unsigned char u1s_sciset_sendnum;
	
	volatile static float f4s_sciset_vtgt;
	volatile static signed short s2s_sciset_nmtgt;
	volatile static float f4s_sciset_nmsm;
	volatile static unsigned short u2s_sciset_stringarray[10] = {0};
	volatile static unsigned short u2t_sciset_count = 0;

	u2t_sciset_count++;
	if(u2t_sciset_count>300)
	{
		switch(u1s_sciset_sendnum)
		{
			case 0:
			// u2t_sciset_nmtabs = (unsigned short)abs(s4g_rspicnt_nm1tgt);
			// u2t_sciset_nmabs = (unsigned short)abs(s4g_mtcnt_nmsm1);

			u2s_sciset_stringarray[0] = (unsigned short)abs(s4g_rspicnt_nm1tgt / 100);
			u2s_sciset_stringarray[1] = (unsigned short)abs((s4g_rspicnt_nm1tgt % 100) / 10);
			u2s_sciset_stringarray[2] = (unsigned short)abs(s4g_rspicnt_nm1tgt % 10);
			
			u2s_sciset_stringarray[3] = (unsigned short)abs(s4g_mtcnt_nmsm1 / 100);
			u2s_sciset_stringarray[4] = (unsigned short)abs((s4g_mtcnt_nmsm1 % 100) / 10);
			u2s_sciset_stringarray[5] = (unsigned short)abs(s4g_mtcnt_nmsm1 % 10);

			u1s_sciset_sendnum++;
			break;
			
			case 1:
			for(i = 0; i < 3; i++)
			{
				vdg_scicnt_scisend(u2s_sciset_stringarray[i]);
				while(SCI1.SSR.BIT.TEND == 0);
			}
			SCI1.TDR = ',';
			u1s_sciset_sendnum++;
			break;

			case 2:
			for(i = 3; i < 6; i++)
			{
				vdg_scicnt_scisend(u2s_sciset_stringarray[i]);
				while(SCI1.SSR.BIT.TEND == 0);
			}
			vdg_scicnt_scisend(13);
			u1s_sciset_sendnum = 0;
			u2t_sciset_count = 0;
			break;
		}
	}
}