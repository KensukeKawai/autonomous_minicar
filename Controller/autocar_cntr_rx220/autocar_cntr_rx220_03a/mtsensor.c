
#include "common.h"

/***************グローバル変数定義***************/
volatile unsigned char u1g_mtsensor_rotdirm1;
volatile unsigned char u1g_mtsensor_rotdirm2;
volatile float f4g_mtsensor_manglem1;
volatile float f4g_mtsensor_manglem2;
volatile float f4g_mtsensor_eanglem1;
volatile float f4g_mtsensor_eanglem2;
volatile signed long s4g_mtsensor_nm1;
volatile signed long s4g_mtsensor_nm2;

void vdg_mtsensor_encoder()
{
    /**********static変数定義**********/
    volatile static unsigned short u2s_mtsensor_enccntm1;
    volatile static unsigned short u2s_mtsensor_enccntm2;
    volatile static unsigned short u2s_mtsensor_enccntm1z = 0;
    volatile static unsigned short u2s_mtsensor_enccntm2z = 0;

    /**********テンポラリ変数定義**********/
    volatile signed long s4t_mtsensor_denccntm1;
    volatile signed long s4t_mtsensor_denccntm2;
    volatile unsigned short u2t_mtsensor_cnte360;
    volatile float f4t_mtsensor_eanglepre;
    volatile signed long s4t_mtsensor_nm1pre;
    volatile signed long s4t_mtsensor_nm2pre;

    //MTU2：Motor1、MTU1：Motor2
    //MTUとモータ番号が逆であることに注意

    //モータ回転方向の検出
    // u1g_mtsensor_rotdirm1 = MTU2.TSR.BIT.TCFD;  //正転：0、逆転：1
    // u1g_mtsensor_rotdirm2 = MTU1.TSR.BIT.TCFD;  //正転：0、逆転：1

    //Motor1
    /*****回転方向検出、カウント前回値差分算出*****/
    u2s_mtsensor_enccntm1 = MTU_M1_ENCTCNT;
    s4t_mtsensor_denccntm1 = (signed long)(u2s_mtsensor_enccntm1)-(signed long)(u2s_mtsensor_enccntm1z);
    if(s4t_mtsensor_denccntm1>=0){u1g_mtsensor_rotdirm1=0;}
    else{u1g_mtsensor_rotdirm1=1;}

    /*****エンコーダパルスカウント処理～機械角算出*****/
    //カウント上下限から外れている場合は中点に戻す。カウント値取得後即座に実施必要なためここで実施
    //HIGH≦カウント
    if(u2s_mtsensor_enccntm1>=TCNT_ENC_HIGH)
    {
        u2s_mtsensor_enccntm1 = u2s_mtsensor_enccntm1 - TCNT_ENC_360M;
        MTU_M1_ENCTCNT = u2s_mtsensor_enccntm1;
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //カウント≦LOW
    else if(u2s_mtsensor_enccntm1<=TCNT_ENC_LOW)
    {
        u2s_mtsensor_enccntm1 = u2s_mtsensor_enccntm1 + TCNT_ENC_360M;
        MTU_M1_ENCTCNT = u2s_mtsensor_enccntm1;
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }
    //MID≦カウント＜HIGH
    else if(u2s_mtsensor_enccntm1>=TCNT_ENC_MID && u2s_mtsensor_enccntm1 < TCNT_ENC_HIGH)
    {
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //LOW＜カウント＜MID
    else if(u2s_mtsensor_enccntm1<TCNT_ENC_MID && u2s_mtsensor_enccntm1 > TCNT_ENC_LOW)
    {
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }

    //Motor2
    /*****回転方向検出、カウント前回値差分算出*****/
    u2s_mtsensor_enccntm2 = MTU_M2_ENCTCNT;
    s4t_mtsensor_denccntm2 = (signed long)(u2s_mtsensor_enccntm2)-(signed long)(u2s_mtsensor_enccntm2z);
    if(s4t_mtsensor_denccntm2>=0){u1g_mtsensor_rotdirm2=0;}
    else{u1g_mtsensor_rotdirm2=1;}

    /*****エンコーダパルスカウント処理～機械角算出*****/
    //カウント上下限から外れている場合は中点に戻す。カウント値取得後即座に実施必要なためここで実施
    //HIGH≦カウント
    if(u2s_mtsensor_enccntm2>=TCNT_ENC_HIGH)
    {
        u2s_mtsensor_enccntm2 = u2s_mtsensor_enccntm2 - TCNT_ENC_360M;
        MTU_M2_ENCTCNT = u2s_mtsensor_enccntm2;
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //カウント≦LOW
    else if(u2s_mtsensor_enccntm2<=TCNT_ENC_LOW)
    {
        u2s_mtsensor_enccntm2 = u2s_mtsensor_enccntm2 + TCNT_ENC_360M;
        MTU_M2_ENCTCNT = u2s_mtsensor_enccntm2;
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }
    //MID≦カウント＜HIGH
    else if(u2s_mtsensor_enccntm2>=TCNT_ENC_MID && u2s_mtsensor_enccntm2 < TCNT_ENC_HIGH)
    {
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //LOW＜カウント＜MID
    else if(u2s_mtsensor_enccntm2<TCNT_ENC_MID && u2s_mtsensor_enccntm2 > TCNT_ENC_LOW)
    {
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }

    /*****カウント値の前回値ラッチ処理*****/
    u2s_mtsensor_enccntm1z = u2s_mtsensor_enccntm1;
    u2s_mtsensor_enccntm2z = u2s_mtsensor_enccntm2;

    /*****回転数算出*****/
    //Δ機械角(1周期当たり)分カウント×カウントto機械角係数×キャリア周波数/360×60
    // s4t_mtsensor_nm1pre = (signed long)((float)s4t_mtsensor_denccntm1*KPLS2MANGLE*HZ_CARRIER/6);
    s4t_mtsensor_nm1pre = (signed long)((float)(s4t_mtsensor_denccntm1)*KPLS2MANGLE*(float)(HZ_CARRIER)/(float)(6));
    s4t_mtsensor_nm2pre = (signed long)((float)(s4t_mtsensor_denccntm2)*KPLS2MANGLE*(float)(HZ_CARRIER)/(float)(6));

    //実モータ回転数に上下限処理。Jetsonへの送信bit数都合上ここでレンジ内に収める。
    if (s4t_mtsensor_nm1pre > NM_MAX_RANGE) { s4g_mtsensor_nm1 = NM_MAX_RANGE; }
    else if(s4t_mtsensor_nm1pre < NM_MIN_RANGE) { s4g_mtsensor_nm1 = NM_MIN_RANGE; }
    else { s4g_mtsensor_nm1 = s4t_mtsensor_nm1pre; }
    if (s4t_mtsensor_nm2pre > NM_MAX_RANGE) { s4g_mtsensor_nm2 = NM_MAX_RANGE; }
    else if(s4t_mtsensor_nm2pre < NM_MIN_RANGE) { s4g_mtsensor_nm2 = NM_MIN_RANGE; }
    else { s4g_mtsensor_nm2 = s4t_mtsensor_nm2pre; }

    /*****電気角算出*****/
    //mangleから電気角算出
    //Motor1
    f4t_mtsensor_eanglepre = f4g_mtsensor_manglem1*(float)POLE_PAIRS;
    u2t_mtsensor_cnte360 = (unsigned short)(f4t_mtsensor_eanglepre/360);
    f4g_mtsensor_eanglem1 = f4t_mtsensor_eanglepre - (float)u2t_mtsensor_cnte360*360;
    //Motor2
    f4t_mtsensor_eanglepre = f4g_mtsensor_manglem2*(float)POLE_PAIRS;
    u2t_mtsensor_cnte360 = (unsigned short)(f4t_mtsensor_eanglepre/360);
    f4g_mtsensor_eanglem2 = f4t_mtsensor_eanglepre - (float)u2t_mtsensor_cnte360*360;
}