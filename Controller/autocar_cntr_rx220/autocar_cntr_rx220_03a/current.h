
#ifndef _CURRENT_H_
#define _CURRENT_H_

/***************関数extern**************/
extern void vdg_current_cal();

/***************グローバル変数extern**************/
extern volatile float f4g_current_imsm1;
extern volatile float f4g_current_imsm2;

/**********マクロ定義*********/
#define ADC_CNV_COUNT 3		                	//ADC変換1タスク実施回数
#define ADC_CNT2V 5/4096/(float)ADC_CNV_COUNT	//V = 5/4096/ADC_CNV_COUNT
#define ADC_AVE_NUM 5		                  	//電流値移動平均回数
#define ACS724_VMED ((float)(2.5))			    //電流センサの0A電圧
#define ACS724_IRATE ((float)(1/0.2))		    //A/V

#endif