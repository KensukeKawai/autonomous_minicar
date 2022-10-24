
#include "common.h"

/**********グローバル変数定義**********/
volatile float f4g_current_imsm1;
volatile float f4g_current_imsm2;

void vdg_current_cal()
{
	/**********static変数定義**********/
	volatile static unsigned char u1s_current_arycnt = 0;
	volatile static float f4s_current_imary1[ADC_AVE_NUM] = {0};	//S
	volatile static float f4s_current_imary2[ADC_AVE_NUM] = {0};	//S

	/**********テンポラリ変数定義**********/
	volatile unsigned char i;
	volatile float f4t_current_vout1;
	volatile float f4t_current_vout2;
	volatile float f4t_current_imadd1 = 0;
	volatile float f4t_current_imadd2 = 0;

	//ADCカウント値から電圧値に変換。CNT2Vには平均回数だけ加算された分の除算があるためresultは加算値のままで良い
	f4t_current_vout1 = (float)u2g_exs12adi0_adccntm1ad * ADC_CNT2V;
	f4t_current_vout2 = (float)u2g_exs12adi0_adccntm2ad * ADC_CNT2V;
	
	//電圧⇒電流変換し配列に格納
	f4s_current_imary1[u1s_current_arycnt] = (f4t_current_vout1 - (float)ACS724_VMED) * (float)ACS724_IRATE;	//(ADC電圧-2.5V)*5
	f4s_current_imary2[u1s_current_arycnt] = (f4t_current_vout2 - (float)ACS724_VMED) * (float)ACS724_IRATE;	//(ADC電圧-2.5V)*5

	//平均回数分だけ配列の電流値を加算
	for( i = 0; i < ADC_AVE_NUM; i++ )
	{
		f4t_current_imadd1 += f4s_current_imary1[i];
		f4t_current_imadd2 += f4s_current_imary2[i];
	}
	//電流平均値算出
	f4g_current_imsm1 = f4t_current_imadd1 / (float)ADC_AVE_NUM;
	f4g_current_imsm2 = f4t_current_imadd2 / (float)ADC_AVE_NUM;

	//電流値格納配列カウント変数処理
	if( u1s_current_arycnt >= ADC_AVE_NUM){ u1s_current_arycnt = 0;}
	else{ u1s_current_arycnt++;}
}