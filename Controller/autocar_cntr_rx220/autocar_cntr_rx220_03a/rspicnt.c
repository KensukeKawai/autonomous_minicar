
#include "common.h"
#include "mtcnt.h"

/***************グローバル変数定義***************/
volatile unsigned char u1g_rspicnt_idmoderq;
volatile signed long s4g_rspicnt_nm1tgt;
volatile signed long s4g_rspicnt_nm2tgt;


//受信有無判定、受信していたら目標回転数と制御モード更新
void vdg_rspicnt_recget()
{
    /**********テンポラリ変数定義**********/
    volatile unsigned long u4t_rspicnt_spdrrec;
    volatile unsigned long u4t_rspicnt_spdrrectmp;


    u4t_rspicnt_spdrrec = RSPI0.SPDR.LONG;      //Long Wordで受信データ取得し処理

    /*****割りつけられたbitに対応する判断、データ処理*****/
    //制御モード要求取得
    if(u4t_rspicnt_spdrrec & BITMASK_MODE_NORMAL)
    {
        u1g_rspicnt_idmoderq = ID_MODE_NORMAL;
    }
    else if(u4t_rspicnt_spdrrec & BITMASK_MODE_MTORIGIN)
    {
        u1g_rspicnt_idmoderq = ID_MODE_MTORIGIN;
    }
    else if(u4t_rspicnt_spdrrec & BITMASK_MODE_STOP)
    {
        u1g_rspicnt_idmoderq = ID_MODE_STOP;
    }
    else//全て要求がなかった場合は停車とする
    {
        u1g_rspicnt_idmoderq = ID_MODE_STOP;
    }

    //目標回転数取得
    //Motor1
    if(u4t_rspicnt_spdrrec & BITMASK_ROTDIR)
    {
        s4g_rspicnt_nm1tgt = -1 * (signed long)(u4t_rspicnt_spdrrec & BITMASK_NMTGT);
    }
    else
    {
        s4g_rspicnt_nm1tgt = (signed long)(u4t_rspicnt_spdrrec & BITMASK_NMTGT);
    }
    //Motor2
    //Motor2目標回転数をLSBスタートにするためにビットシフト
    u4t_rspicnt_spdrrectmp = u4t_rspicnt_spdrrec>>14;
    if(u4t_rspicnt_spdrrectmp & BITMASK_ROTDIR)
    {
        s4g_rspicnt_nm2tgt = -1 * (signed long)(u4t_rspicnt_spdrrectmp & BITMASK_NMTGT);
    }
    else
    {
        s4g_rspicnt_nm2tgt = (signed long)(u4t_rspicnt_spdrrectmp & BITMASK_NMTGT);
    }

    // RSPI0.SPDR.LONG = u4t_rspicnt_spdrrec;  // Tmp
}

void vdg_rspicnt_sendset(unsigned char mode)
{
    volatile unsigned long u4t_rspicnt_nmsm1;
    volatile unsigned long u4t_rspicnt_nmsm2;
    volatile unsigned long u4t_rspicnt_spdrset;

    //モータコントローラ内のモータ制御状態に応じてJetsonに状態と回転数を通知
    switch(mode)
    {
        case ID_MODE_NORMAL:
            //実モータ回転数の符号判定処理
            if(s4g_mtcnt_nmsm1 >= 0)
            {
                u4t_rspicnt_nmsm1 = (unsigned long)(s4g_mtcnt_nmsm1);
            }
            else
            {
                u4t_rspicnt_nmsm1 = (unsigned long)(abs(s4g_mtcnt_nmsm1));
                u4t_rspicnt_nmsm1 = u4t_rspicnt_nmsm1 | BITMASK_ROTDIR;
            }
            if(s4g_mtcnt_nmsm2 >= 0)
            {
                u4t_rspicnt_nmsm2 = (unsigned long)(s4g_mtcnt_nmsm2);
            }
            else
            {
                u4t_rspicnt_nmsm2 = (unsigned long)(abs(s4g_mtcnt_nmsm2));
                u4t_rspicnt_nmsm2 = u4t_rspicnt_nmsm2 | BITMASK_ROTDIR;
            }
            u4t_rspicnt_spdrset = u4t_rspicnt_nmsm1 | (u4t_rspicnt_nmsm2<<BITSHIFT_NM2);
            u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_NORMAL;
        break;

        case ID_MODE_MTORIGIN:
            u4t_rspicnt_spdrset = 0;
            u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_MTORIGIN;
        break;
        
        case ID_MODE_STOP:
            u4t_rspicnt_spdrset = 0;
            u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_STOP;
        break;
    }

    // if( u1g_mtcnt_xnormal == 1){u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_NORMAL;}
    // else if(u1g_mtcnt_xmtorigin == 1){u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_MTORIGIN;}
    // else if(u1g_mtcnt_xstop == 1){u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_STOP;}

    RSPI0.SPDR.LONG = u4t_rspicnt_spdrset;
}