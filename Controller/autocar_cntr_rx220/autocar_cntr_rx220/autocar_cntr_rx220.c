
#include "iodefine.h"
#include "vect.h"
#include "mathf.h"
#include "machine.h"
#include "global_data.h"
#include "api_init.h"
#include "excep.h"

/**********�O���[�o���ϐ���`**********/

/***�d���Z���TADC�֌WRAM��`***/
volatile unsigned short u2g_s12adio_result;
volatile unsigned char u1g_s12adio_xadcex;

volatile float f4g_current_imsm;

/***��]���֌WRAM��`***/
volatile float f4g_tgib2_nmsm;
volatile unsigned short u2g_tgib2_citrpt;
volatile unsigned char u1g_tgib2_xzero;
volatile float f4g_tgib2_elangle;
volatile float f4g_tgib2_mcangle;

volatile unsigned char u1g_tciv2_xovflw;
volatile unsigned char u1g_tciv2_xdnmrst;

volatile float f4g_mtcnt_nmsm;
volatile float f4g_mtcnt_vtgt;
volatile unsigned char u1g_mtcnt_xrgmode;
volatile static float f4g_mtcnt_nmtgt;

volatile unsigned char u1g_rxi12_xmtorgrq;
volatile unsigned char u1g_rxi12_xrcnmt;
volatile signed short s2g_rxi12_nmtgt;
volatile unsigned char u1g_rxi12_nmtary[4] = {0};

volatile unsigned char u1g_mtorigin_xmtorgfin;

/**********�O���[�o���ϐ���`**********/

void vdg_wait_nop(unsigned long N)
{
	unsigned long i;
	for(i=0;i<N;i++){ nop();}
}

void vdg_variable_init()
{
	
}

void vdg_current_cal()
{
	volatile unsigned short i;
	volatile float f4t_current_vout;
	volatile float f4t_current_imadd = 0;
	volatile static float f4s_current_imary[ADC_AVE_NUM] = {0};	//�S�ď�����
	
	f4t_current_vout = (float)u2g_s12adio_result * ADC_CNT2V;
	
	for( i = 1; i < ADC_AVE_NUM; i++ )
	{
		f4s_current_imary[ADC_AVE_NUM-i] = f4s_current_imary[ADC_AVE_NUM-i-1];
	}
	
	f4s_current_imary[0] = (f4t_current_vout - ACS724_VMED) * ACS724_IRATE;	//(ADC�d��-2.5V)*5
	
	for( i = 0; i < ADC_AVE_NUM; i++ )
	{
		f4t_current_imadd += f4s_current_imary[i];
	}
	f4g_current_imsm = f4t_current_imadd / (float)ADC_AVE_NUM;
}

//API�ł�OFF�o�͂�GND�ɂȂ炸�����Ă��邽�߁APMR��ėp�o��LOW�ɂ���
void vdg_mtioc_outset(unsigned char mode)
{
	switch(mode)
	{
		case ID_ALLSET:
		PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)MTIOC4C
		PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)MTIOC4A
		PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)MTIOC4B
		PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)MTIOC4D
		PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)MTIOC3D
		PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)MTIOC3B
		break;
		
		case ID_HIGHOFF:
		PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)MTIOC4C��OFF
		PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)MTIOC4A
		PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)MTIOC4B
		PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)MTIOC4D��OFF
		PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)MTIOC3D��OFF
		PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)MTIOC3B
		break;
		
		case ID_LOWOFF:
		PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)MTIOC4C
		PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)MTIOC4A��OFF
		PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)MTIOC4B��OFF
		PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)MTIOC4D
		PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)MTIOC3D
		PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)MTIOC3B��OFF
		break;
		
		case ID_ALLOFF:
		PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)MTIOC4C��OFF
		PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)MTIOC4A��OFF
		PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)MTIOC4B��OFF
		PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)MTIOC4D��OFF
		PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)MTIOC3D��OFF
		PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)MTIOC3B��OFF
		break;
	}
}

void vdg_nmtgt()
{
	if(u1g_rxi12_xrcnmt == 1)
	{
		u1g_rxi12_xrcnmt = 0;
		
	}
}

void vdg_mtcnt()
{
	volatile float f4t_mtcnt_nmerror;
	volatile float f4t_mtcnt_imerror;
	volatile float f4t_mtcnt_duty;
	volatile float f4t_mtcnt_cnt;
	volatile float f4t_mtcnt_cntmid;
	volatile float f4t_mtcnt_cntu;
	volatile float f4t_mtcnt_cntv;
	volatile float f4t_mtcnt_cntw;
	volatile float f4t_mtcnt_dutyrg;
	volatile float f4t_mtcnt_cntrg;
	
	volatile static float f4s_mtcnt_nmsmz = 0;
	volatile static float f4s_mtcnt_dnm;
	volatile static float f4s_mtcnt_elangle;
	volatile static float f4s_mtcnt_vtgtrg = 0;

	volatile static unsigned short u2s_mtcnt_cntu;
	volatile static unsigned short u2s_mtcnt_cntv;
	volatile static unsigned short u2s_mtcnt_cntw;
	volatile static unsigned short u2s_mtcnt_cntrg;
	
	
	f4g_mtcnt_nmsm = f4g_tgib2_nmsm;			//���b�`
	f4s_mtcnt_elangle = f4g_tgib2_elangle;			//���b�`
	f4g_mtcnt_nmtgt = (float)s2g_rxi12_nmtgt;		//���b�`
	
	//f4g_mtcnt_nmtgt = 200;
	
	if(u1g_tciv2_xdnmrst == 1)
	{
		f4s_mtcnt_dnm = 0;
		u1g_tciv2_xdnmrst = 0;
	}
	else
	{
		f4s_mtcnt_dnm = f4g_mtcnt_nmsm - f4s_mtcnt_nmsmz;	//1�L�����A���̉�]���ω���(����l-�O��l)
	}
	
	f4s_mtcnt_nmsmz = f4g_mtcnt_nmsm;			//�O��l���b�`
	
	//�ߓd���̏ꍇ�͒ʏ�FB���������A�d���}������FB������
	if(f4g_current_imsm >= MAX_I)
	{
		f4t_mtcnt_nmerror = 0;
		f4t_mtcnt_imerror = f4g_current_imsm - MAX_I;
	}
	else
	{
		f4t_mtcnt_nmerror = f4g_mtcnt_nmtgt - f4g_mtcnt_nmsm;
		f4t_mtcnt_imerror = 0;
	}
	
	f4g_mtcnt_vtgt = f4g_mtcnt_vtgt + f4t_mtcnt_nmerror * KP + f4t_mtcnt_imerror * KP_I_LIM;
	
	if(f4g_mtcnt_vtgt >= V_MAX){f4g_mtcnt_vtgt = V_MAX;}
	else if(f4g_mtcnt_vtgt <= V_MIN){f4g_mtcnt_vtgt = V_MIN;}
	
	
	//�ڕW�d�������̏ꍇ�͓d�����
	if(f4g_mtcnt_vtgt >= 0)
	{
		u1g_mtcnt_xrgmode = 0;
		
		vdg_mtioc_outset(ID_ALLSET);
		MTU.TOER.BYTE = 0xFF;		//�S���o�͋���
		
		f4t_mtcnt_duty = f4g_mtcnt_vtgt / V_PW;		//Duty��Z�o
		f4t_mtcnt_cnt = (float)CNT_HALF_CARR * f4t_mtcnt_duty + (float)CNT_DT;		//Duty�䂩��J�E���g���Z�o
		
		if(f4t_mtcnt_cnt > CNT_MAX) { f4t_mtcnt_cnt = CNT_MAX;}
		//else if(f4t_mtcnt_cnt < CNT_MIN) { f4t_mtcnt_cnt = 0;}		//�ǂ̊p�x�ł���A�[���o��0
		
		f4t_mtcnt_cntmid = f4t_mtcnt_cnt / 2;
		
		f4t_mtcnt_cntu = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle+ADV_ELANGLE) * KDEG2RAD);
		f4t_mtcnt_cntv = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle-120+ADV_ELANGLE) * KDEG2RAD);
		f4t_mtcnt_cntw = f4t_mtcnt_cntmid + f4t_mtcnt_cntmid * sinf((f4s_mtcnt_elangle-240+ADV_ELANGLE) * KDEG2RAD);
		
		
		//�㉺���K�[�h�������^�ɕϊ�
		if(f4t_mtcnt_cntu > CNT_MAX) { u2s_mtcnt_cntu = (unsigned short)CNT_MAX; }
		else if(f4t_mtcnt_cntu < CNT_MIN) { u2s_mtcnt_cntu = 0; }
		else { u2s_mtcnt_cntu = (unsigned short)f4t_mtcnt_cntu; }
		
		if(f4t_mtcnt_cntv > CNT_MAX) { u2s_mtcnt_cntv = (unsigned short)CNT_MAX; }
		else if(f4t_mtcnt_cntv < CNT_MIN) { u2s_mtcnt_cntv = 0; }
		else { u2s_mtcnt_cntv = (unsigned short)f4t_mtcnt_cntv; }
		
		if(f4t_mtcnt_cntw > CNT_MAX) { u2s_mtcnt_cntw = (unsigned short)CNT_MAX; }
		else if(f4t_mtcnt_cntw < CNT_MIN) { u2s_mtcnt_cntw = 0; }
		else { u2s_mtcnt_cntw = (unsigned short)f4t_mtcnt_cntw; }
		
		MTU3.TGRD = u2s_mtcnt_cntu;
		MTU4.TGRC = u2s_mtcnt_cntv;
		MTU4.TGRD = u2s_mtcnt_cntw;
		
		f4s_mtcnt_vtgtrg = 0;		//����񐶃��[�h���ɑO��̎w�ߓd�����c��Ȃ��悤�N���A
	}
	
	else
	{
		u1g_mtcnt_xrgmode = 1;

		f4t_mtcnt_dutyrg = f4g_mtcnt_vtgt / V_PW;
		f4t_mtcnt_cntrg = (float)CNT_HALF_CARR * ( 1 + f4t_mtcnt_dutyrg );	//���A�[��Duty�偨�񐶑�̂���1-dutyrg

		//�㉺���K�[�h�������^�ɕϊ�
		if(f4t_mtcnt_cntrg > CNT_MAX_RG) { u2s_mtcnt_cntrg = CNT_MAX_RG; }
		else if(f4t_mtcnt_cntrg < CNT_MIN_RG) { u2s_mtcnt_cntrg = CNT_MIN_RG;}
		else { u2s_mtcnt_cntrg = (unsigned short)f4t_mtcnt_cntrg; }

		
		//�o�͏���
		vdg_mtioc_outset(ID_HIGHOFF);
		MTU.TOER.BYTE = 0xC7;	//���A�[���̂ݏo��
		
		//u2s_mtcnt_cntrg = 2000;
		
		MTU3.TGRD = u2s_mtcnt_cntrg;
		MTU4.TGRC = u2s_mtcnt_cntrg;
		MTU4.TGRD = u2s_mtcnt_cntrg;
	}	
}

void vdg_scisend(unsigned short senddata)
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

void vdg_sciset()
{	
	volatile unsigned char i;
	
	volatile unsigned short u2t_sciset_vtabs;
	volatile unsigned short u2t_sciset_nmtabs;
	volatile unsigned short u2t_sciset_nmabs;
	
	volatile static unsigned short u1s_sciset_sendnum;
	
	volatile static float f4s_sciset_vtgt;
	volatile static signed short s2s_sciset_nmtgt;
	volatile static float f4s_sciset_nmsm;
	volatile static unsigned short u2s_sciset_stringarray[10] = {0};

	switch(u1s_sciset_sendnum)
	{
		case 0:
		f4s_sciset_vtgt = f4g_mtcnt_vtgt;
		s2s_sciset_nmtgt = s2g_rxi12_nmtgt;
		f4s_sciset_nmsm = f4g_mtcnt_nmsm;
		
		u2t_sciset_vtabs = (unsigned short)abs((int)(f4s_sciset_vtgt*10));
		u2t_sciset_nmtabs = (unsigned short)abs(s2s_sciset_nmtgt);
		u2t_sciset_nmabs = (unsigned short)abs((int)f4s_sciset_nmsm);
		
		u2s_sciset_stringarray[0] = u2t_sciset_vtabs / 100;
		u2s_sciset_stringarray[1] = (u2t_sciset_vtabs % 100) / 10;
		u2s_sciset_stringarray[2] = u2t_sciset_vtabs % 10;
		
		u2s_sciset_stringarray[3] = u2t_sciset_nmtabs / 100;
		u2s_sciset_stringarray[4] = (u2t_sciset_nmtabs % 100) / 10;
		u2s_sciset_stringarray[5] = u2t_sciset_nmtabs % 10;
		
		u2s_sciset_stringarray[6] = u2t_sciset_nmabs / 100;
		u2s_sciset_stringarray[7] = (u2t_sciset_nmabs % 100) / 10;
		u2s_sciset_stringarray[8] = u2t_sciset_nmabs % 10;
		
		
		if(f4s_sciset_vtgt < 0){ SCI1.TDR = '-';}
		else{SCI1.TDR = '+';}
		while(SCI1.SSR.BIT.TEND == 0);
		vdg_scisend(u2s_sciset_stringarray[0]);
		u1s_sciset_sendnum++;
		break;
		
		
		case 1:
		for(i = 1; i < 3; i++)
		{
			vdg_scisend(u2s_sciset_stringarray[i]);
			while(SCI1.SSR.BIT.TEND == 0);
		}
		u1s_sciset_sendnum++;
		break;
		
		
		case 2:
		if(s2s_sciset_nmtgt < 0){ SCI1.TDR = '-';}
		else{SCI1.TDR = '+';}
		while(SCI1.SSR.BIT.TEND == 0);
		vdg_scisend(u2s_sciset_stringarray[3]);
		u1s_sciset_sendnum++;
		break;
		
		
		case 3:
		for(i = 4; i < 6; i++)
		{
			vdg_scisend(u2s_sciset_stringarray[i]);
			while(SCI1.SSR.BIT.TEND == 0);
		}
		u1s_sciset_sendnum++;
		break;
		
		
		case 4:
		if(f4s_sciset_nmsm < 0){ SCI1.TDR = '-';}
		else{SCI1.TDR = '+';}
		while(SCI1.SSR.BIT.TEND == 0);
		vdg_scisend(u2s_sciset_stringarray[6]);
		u1s_sciset_sendnum++;
		break;
		
		
		case 5:
		for(i = 7; i < 9; i++)
		{
			vdg_scisend(u2s_sciset_stringarray[i]);
			while(SCI1.SSR.BIT.TEND == 0);
		}
		u1s_sciset_sendnum = 0;
		break;
	}
		
//		case 1:
//		if(f4s_sciset_vtgt < 0){ SCI1.TDR = '-';}
//		else{SCI1.TDR = '+';}
			
//		while(SCI1.SSR.BIT.TEND == 0);
			
//		for(i = 0; i < 3; i++)
//		{
//			switch(u2s_sciset_stringarray[i])
//			{
//				case 0: SCI1.TDR = '0'; break;
//				case 1: SCI1.TDR = '1'; break;
//				case 2: SCI1.TDR = '2'; break;
//				case 3: SCI1.TDR = '3'; break;
//				case 4: SCI1.TDR = '4'; break;
//				case 5: SCI1.TDR = '5'; break;
//				case 6: SCI1.TDR = '6'; break;
//				case 7: SCI1.TDR = '7'; break;
//				case 8: SCI1.TDR = '8'; break;
//				case 9: SCI1.TDR = '9'; break;
//			}
//			while(SCI1.SSR.BIT.TEND == 0);
//		}
//		u1s_sciset_sendnum++;
//		break;
		
//		case 2:
//		if(s2s_sciset_nmtgt < 0){ SCI1.TDR = '-';}
//		else{SCI1.TDR = '+';}
		
//		while(SCI1.SSR.BIT.TEND == 0);
		
//		for(i = 3; i < 6; i++)
//		{
//			switch(u2s_sciset_stringarray[i])
//			{
//				case 0: SCI1.TDR = '0'; break;
//				case 1: SCI1.TDR = '1'; break;
//				case 2: SCI1.TDR = '2'; break;
//				case 3: SCI1.TDR = '3'; break;
//				case 4: SCI1.TDR = '4'; break;
//				case 5: SCI1.TDR = '5'; break;
//				case 6: SCI1.TDR = '6'; break;
//				case 7: SCI1.TDR = '7'; break;
//				case 8: SCI1.TDR = '8'; break;
//				case 9: SCI1.TDR = '9'; break;
//			}
//			while(SCI1.SSR.BIT.TEND == 0);
//		}
//		u1s_sciset_sendnum++;
//		break;
		
//		case 3:
//		if(f4s_sciset_nmsm < 0){ SCI1.TDR = '-';}
//		else{SCI1.TDR = '+';}
		
//		while(SCI1.SSR.BIT.TEND == 0);
		
//		for(i = 6; i < 9; i++)
//		{
//			switch(u2s_sciset_stringarray[i])
//			{
//				case 0: SCI1.TDR = '0'; break;
//				case 1: SCI1.TDR = '1'; break;
//				case 2: SCI1.TDR = '2'; break;
//				case 3: SCI1.TDR = '3'; break;
//				case 4: SCI1.TDR = '4'; break;
//				case 5: SCI1.TDR = '5'; break;
//				case 6: SCI1.TDR = '6'; break;
//				case 7: SCI1.TDR = '7'; break;
//				case 8: SCI1.TDR = '8'; break;
//				case 9: SCI1.TDR = '9'; break;
//			}
//			while(SCI1.SSR.BIT.TEND == 0);
//		}
//		u1s_sciset_sendnum = 0;
//	}
}

void vdg_mtorigin()
{
	volatile unsigned char i;
	volatile unsigned char j;
	
	volatile unsigned short u2t_mtorigin_cntv;
	volatile unsigned short dummy;
	
	volatile static unsigned long u4s_mtorigin_cntcycle = 0;
	volatile static unsigned char u1s_mtorigin_cntstage = 0;
	volatile static unsigned char u1s_mtorigin_cntrot = 0;
	
	u1g_mtorigin_xmtorgfin = 0;
	u2t_mtorigin_cntv = 500;
	dummy = 1000;
	
	if(u4s_mtorigin_cntcycle > CNT_MTORG*6)
	{
		u4s_mtorigin_cntcycle = 0;
		u1s_mtorigin_cntrot++;
	}
	
	if(u1s_mtorigin_cntrot < POLE_PAIRS)
	{
		u4s_mtorigin_cntcycle++;

		if(u4s_mtorigin_cntcycle <= CNT_MTORG) 	{ u1s_mtorigin_cntstage = 1; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*2) 	{ u1s_mtorigin_cntstage = 2; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*3) 	{ u1s_mtorigin_cntstage = 3; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*4) { u1s_mtorigin_cntstage = 4; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*5) { u1s_mtorigin_cntstage = 5; }
		else if(u4s_mtorigin_cntcycle <= CNT_MTORG*6) { u1s_mtorigin_cntstage = 6; }
		
		switch(u1s_mtorigin_cntstage)
		{
			case 1:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
			PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)��MTIOC4B
			PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)��MTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
			MTU.TOER.BYTE = STAGE1;
			MTU3.TGRD = u2t_mtorigin_cntv;
			MTU4.TGRC = 0;
			MTU4.TGRD = dummy;		//�_�~�[��ݒ�
			break;
		
			case 2:
			PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)��MTIOC4C
			PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)��MTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
			MTU.TOER.BYTE = STAGE2;
			MTU3.TGRD = u2t_mtorigin_cntv;
			MTU4.TGRD = 0;
			break;
		
			case 3:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
			PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)��MTIOC3D
			PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)��MTIOC3B
			MTU.TOER.BYTE = STAGE3;
			MTU4.TGRC = u2t_mtorigin_cntv;
			MTU4.TGRD = 0;
			break;
			
			case 4:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
			PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)��MTIOC4B
			PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)��MTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
			MTU.TOER.BYTE = STAGE4;
			MTU3.TGRD = 0;
			MTU4.TGRC = u2t_mtorigin_cntv;
			MTU4.TGRD = dummy;		//�_�~�[��ݒ�
			break;
			
			case 5:
			PORTB.PMR.BIT.B1 = 0;		//PB1(CN2-2)��MTIOC4C
			PORTB.PMR.BIT.B3 = 0;		//PB3(CN2-3)��MTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
			PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
			PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
			MTU.TOER.BYTE = STAGE5;
			MTU3.TGRD = 0;
			MTU4.TGRD = u2t_mtorigin_cntv;
			break;
			
			case 6:
			PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
			PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
			PORTC.PMR.BIT.B2 = 1;		//PC2(CN2-7)��MTIOC4B
			PORTC.PMR.BIT.B3 = 1;		//PC3(CN2-8)��MTIOC4D
			PORTC.PMR.BIT.B4 = 0;		//PC4(CN2-9)��MTIOC3D
			PORTC.PMR.BIT.B5 = 0;		//PC5(CN2-10)��MTIOC3B
			MTU.TOER.BYTE = STAGE6;
			MTU4.TGRC = 0;
			MTU4.TGRD = u2t_mtorigin_cntv;
			break;
		}
	}
	
	else
	{
		SCI12.TDR = 'Z';		//���_�w�K�I�����}�X�^�[�ɑ��M
		
		/***Stage1�Ńz�[���h***/
		PORTB.PMR.BIT.B1 = 1;		//PB1(CN2-2)��MTIOC4C
		PORTB.PMR.BIT.B3 = 1;		//PB3(CN2-3)��MTIOC4A
		PORTC.PMR.BIT.B2 = 0;		//PC2(CN2-7)��MTIOC4B
		PORTC.PMR.BIT.B3 = 0;		//PC3(CN2-8)��MTIOC4D
		PORTC.PMR.BIT.B4 = 1;		//PC4(CN2-9)��MTIOC3D
		PORTC.PMR.BIT.B5 = 1;		//PC5(CN2-10)��MTIOC3B
		MTU.TOER.BYTE = STAGE1;
		MTU3.TGRD = u2t_mtorigin_cntv;
		MTU4.TGRC = 0;
		MTU4.TGRD = dummy;		//�_�~�[��ݒ�
		
		u4s_mtorigin_cntcycle = 0;
		u1s_mtorigin_cntstage = 0;
		u1s_mtorigin_cntrot = 0;
		
		u1g_mtorigin_xmtorgfin = 1;		//�����݂ł̉�]���Z�o������
		u1g_rxi12_xmtorgrq = 0;		//���_�w�K�v�������Z�b�g
	}


//	for(u1s_mtorigin_cntrot=0; u1s_mtorigin_cntrot<7; i++)
//	{
//		for(u1s_mtorigin_cntstage=1; u1s_mtorigin_cntstage<7; j++)
//		{
//			switch(u1s_mtorigin_cntstage)
//			{
//				case 1:
//				MTU.TOER.BYTE = STAGE1;
//				MTU3.TGRD = u2t_mtorigin_cntv;
//				MTU4.TGRC = 0;
//				MTU4.TGRD = dummy;		//�_�~�[��ݒ�
//				break;
			
//				case 2:
//				MTU.TOER.BYTE = STAGE2;
//				MTU3.TGRD = u2t_mtorigin_cntv;
//				MTU4.TGRD = 0;
//				break;
			
//				case 3:
//				MTU.TOER.BYTE = STAGE3;
//				MTU4.TGRC = u2t_mtorigin_cntv;
//				MTU4.TGRD = 0;
//				break;
				
//				case 4:
//				MTU.TOER.BYTE = STAGE4;
//				MTU3.TGRD = 0;
//				MTU4.TGRC = u2t_mtorigin_cntv;
//				MTU4.TGRD = dummy;		//�_�~�[��ݒ�
//				break;
				
//				case 5:
//				MTU.TOER.BYTE = STAGE5;
//				MTU3.TGRD = 0;
//				MTU4.TGRD = u2t_mtorigin_cntv;
//				break;
				
//				case 6:
//				MTU.TOER.BYTE = STAGE6;
//				MTU4.TGRC = 0;
//				MTU4.TGRD = u2t_mtorigin_cntv;
//				break;
//			}
			
//			vdg_wait_nop(781250);
//			vdg_wait_nop(781250);
//		}
//	}
	

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
	vdg_SCI_init();
	vdg_IPR_init();
	
	vdg_mtioc_outset(ID_ALLOFF);
	
	while(u1g_rxi12_xmtorgrq == 0);	//�}�X�^�[����M����M�܂ő҂�
	//u1g_rxi12_xmtorgrq = 1;
	SCI12.TDR = 'A';			//���j�^PC�ɒʐM�J�n�A��
	
	//MTU.TOER.BYTE = 0xFF;		//�S���o�͋���
	MTU.TSTR.BYTE = 0xC4;		//MTU2.TCNT,MTU3.TCNT��MTU4.TCNT�̃J�E���g������J�n
	
	while(1)
	{
		if(u1g_s12adio_xadcex == 1)
		{
//			MTU3.TGRD = 1000;
//			MTU4.TGRC = 1000;
//			MTU4.TGRD = 1000;
			
			//�}�X�^�[����'A'�������Ă����ꍇ�A���_�w�K������D�悵�Ď��{
			if(u1g_rxi12_xmtorgrq == 1)		//�}�X�^�[���猴�_�w�K�v�����������ꍇ
			{
				vdg_mtorigin();
			}
			
			else
			{
				vdg_current_cal();		//�d���Z���TADC���ʂ���d���l�Z�o
				vdg_mtcnt();
				vdg_sciset();
				
				if(s2g_rxi12_nmtgt == 100){ TRG1 = !TRG1;}
				
//				if(u1g_mtcnt_xrgmode == 1){ TRG1 = 1;}
//				else{ TRG1 = 0; }
			}
			
			u1g_s12adio_xadcex = 0;
		}
		
	}//while
	
}//main