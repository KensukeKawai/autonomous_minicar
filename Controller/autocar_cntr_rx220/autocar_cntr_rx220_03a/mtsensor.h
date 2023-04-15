
#ifndef _MTSENSOR_H_
#define _MTSENSOR_H_

/***************関数extern**************/
extern void vdg_mtsensor_encoder();

/***************グローバル変数extern**************/
extern volatile unsigned char u1g_mtsensor_rotdirm1;
extern volatile unsigned char u1g_mtsensor_rotdirm2;
extern volatile float f4g_mtsensor_manglem1;
extern volatile float f4g_mtsensor_manglem2;
extern volatile float f4g_mtsensor_eanglem1;
extern volatile float f4g_mtsensor_eanglem2;
extern volatile signed long s4g_mtsensor_nm1;
extern volatile signed long s4g_mtsensor_nm2;

/***************マクロ定義***************/
#define MTORIGIN_EANGLE ((float)(60))                   //原点学習完了時電気角。Phase1ホールドした場合、どこで落ち着くのだろう・・・
#define MTORIGIN_MANGLE ((float)(MTORIGIN_EANGLE)/(float)(POLE_PAIRS))

#define KPLS2MANGLE ((float)(0.3))                      //ロータリエンコーダパルスカウント⇒機械角変換係数
#define KPLS2EANGLE ((float)(2.1))                      //ロータリエンコーダパルスカウント⇒電気角変換係数

#define TCNT_ENC_MID ((unsigned short)(32768))          //前転後転どちらに行っても機械角一周でオーバー/アンダーフローしないように中点でカウントスタート
#define TCNT_ENC_DEFAULT ((unsigned short)(TCNT_ENC_MID+(unsigned short)(MTORIGIN_EANGLE/KPLS2EANGLE))) //カウンタ中点＋原点学習後角度分カウント
#define TCNT_ENC_360M ((unsigned short)(360/KPLS2MANGLE))
#define TCNT_ENC_HIGH (TCNT_ENC_MID+TCNT_ENC_360M)      //カウント上限。これ"以上"になったらTCNTをMIDに戻す
#define TCNT_ENC_LOW (TCNT_ENC_MID-TCNT_ENC_360M)       //カウント下限。これ"以下"になったらTCNTをMIDに戻す

#define NM_MAX_RANGE ((signed long)(8192))
#define NM_MIN_RANGE ((signed long)(-8192))



#endif