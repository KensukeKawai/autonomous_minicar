
#include "common.h"

/**********�O���[�o���ϐ���`**********/
volatile float f4g_current_imsm1;
volatile float f4g_current_imsm2;

void vdg_current_cal()
{
	/**********static�ϐ���`**********/
	volatile static unsigned char u1s_current_arycnt = 0;
	volatile static float f4s_current_imary1[ADC_AVE_NUM] = {0};	//S
	volatile static float f4s_current_imary2[ADC_AVE_NUM] = {0};	//S

	/**********�e���|�����ϐ���`**********/
	volatile unsigned char i;
	volatile float f4t_current_vout1;
	volatile float f4t_current_vout2;
	volatile float f4t_current_imadd1 = 0;
	volatile float f4t_current_imadd2 = 0;

	//ADC�J�E���g�l����d���l�ɕϊ��BCNT2V�ɂ͕��ω񐔂������Z���ꂽ���̏��Z�����邽��result�͉��Z�l�̂܂܂ŗǂ�
	f4t_current_vout1 = (float)u2g_exs12adi0_adccntm1ad * ADC_CNT2V;
	f4t_current_vout2 = (float)u2g_exs12adi0_adccntm2ad * ADC_CNT2V;
	
	//�d���˓d���ϊ����z��Ɋi�[
	f4s_current_imary1[u1s_current_arycnt] = (f4t_current_vout1 - (float)ACS724_VMED) * (float)ACS724_IRATE;	//(ADC�d��-2.5V)*5
	f4s_current_imary2[u1s_current_arycnt] = (f4t_current_vout2 - (float)ACS724_VMED) * (float)ACS724_IRATE;	//(ADC�d��-2.5V)*5

	//���ω񐔕������z��̓d���l�����Z
	for( i = 0; i < ADC_AVE_NUM; i++ )
	{
		f4t_current_imadd1 += f4s_current_imary1[i];
		f4t_current_imadd2 += f4s_current_imary2[i];
	}
	//�d�����ϒl�Z�o
	f4g_current_imsm1 = f4t_current_imadd1 / (float)ADC_AVE_NUM;
	f4g_current_imsm2 = f4t_current_imadd2 / (float)ADC_AVE_NUM;

	//�d���l�i�[�z��J�E���g�ϐ�����
	if( u1s_current_arycnt >= ADC_AVE_NUM){ u1s_current_arycnt = 0;}
	else{ u1s_current_arycnt++;}
}