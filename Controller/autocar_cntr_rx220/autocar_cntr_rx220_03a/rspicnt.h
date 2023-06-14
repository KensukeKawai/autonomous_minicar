
#ifndef _RSPICNT_H_
#define _RSPICNT_H_

/***************関数extern**************/
extern void vdg_rspicnt_init();
extern void vdg_rspicnt_recget();
extern void vdg_rspicnt_sendset(unsigned char mode);

/***************グローバル変数extern**************/
extern volatile unsigned char u1g_rspicnt_idmoderq;
extern volatile signed long s4g_rspicnt_nm1tgt;
extern volatile signed long s4g_rspicnt_nm2tgt;

/***************マクロ定義***************/
#define ID_MODE_STP 1                                                   //停車
#define ID_MODE_RUN 2                                                   //通常走行
#define ID_MODE_ORG 3                                                   //原点学習
#define ID_MODE_RUNTOSTP 4                                              //通常走行→停車遷移中
#define ID_MODE_RECONLY 15                                              //Jetsonに状態のみ送信するモード

#define BITMASK_MODE ((unsigned long)(0xF0000000))                      //MSBから4bit分状態判定用マスク
#define BITMASK_MODE_STP ((unsigned long)(ID_MODE_STP<<28))
#define BITMASK_MODE_RUN ((unsigned long)(ID_MODE_RUN<<28))             //通常走行モード要求"1"マスク用
#define BITMASK_MODE_ORG ((unsigned long)(ID_MODE_ORG<<28))             //原点学習要求"2"マスク用
#define BITMASK_MODE_RUNTOSTP ((unsigned long)(ID_MODE_RUNTOSTP<<28))   //走行中→停車遷移中"3"マスク用
#define BITMASK_ROTDIR ((unsigned long)(1<<13))                         //Motorの回転方法判定用ビットマスク
#define BITMASK_NMTGT ((unsigned long)(0x1FFF))                         //モータ目標回転数用ビットマスク。13bit
#define BITSHIFT_MODE ((unsigned long)(28))                             //状態判定用上位4bit用ビットシフト回数
#define BITSHIFT_NM2 ((unsigned long)(14))                              //Motor2回転数用ビットシフト回数

#endif