
#ifndef _CURRENT_H_
#define _CURRENT_H_

/***************�֐�extern**************/
extern void vdg_current_cal();

/***************�O���[�o���ϐ�extern**************/
extern volatile float f4g_current_imsm1;
extern volatile float f4g_current_imsm2;

/**********�}�N����`*********/
#define ADC_CNV_COUNT 3		                	//ADC�ϊ�1�^�X�N���{��
#define ADC_CNT2V 5/4096/(float)ADC_CNV_COUNT	//V = 5/4096/ADC_CNV_COUNT
#define ADC_AVE_NUM 5		                  	//�d���l�ړ����ω�
#define ACS724_VMED ((float)(2.5))			    //�d���Z���T��0A�d��
#define ACS724_IRATE ((float)(1/0.2))		    //A/V

#endif