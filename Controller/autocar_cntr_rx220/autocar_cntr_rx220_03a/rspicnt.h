
#ifndef _RSPICNT_H_
#define _RSPICNT_H_

/***************関数extern**************/
extern void vdg_rspicnt_recget();

/***************グローバル変数extern**************/
extern volatile unsigned char u1g_rspicnt_idmoderq;
extern volatile signed long s4g_rspicnt_nm1tgt;
extern volatile signed long s4g_rspicnt_nm2tgt;

/***************マクロ定義***************/
#define BITMASK_MODE_NORMAL ((unsigned long)(1<<28))             //通常走行モード要求28ビット目マスク用
#define BITMASK_MODE_MTORIGIN ((unsigned long)(1<<29))           //原点学習要求ビット29ビット目マスク用
#define BITMASK_MODE_STOP ((unsigned long)(1<<30))               //停車要求ビット30ビットマスク用
#define BITMASK_ROTDIR ((unsigned long)(1<<13))                  //Motorの回転方法判定用ビットマスク
#define BITMASK_NMTGT ((unsigned long)(0x1FFF))                  //モータ目標回転数用ビットマスク。13bit
#define BITSHIFT_NM2 ((unsigned long)(14))                       //Motor2回転数用ビットシフト回数
#define ID_MODE_STOP 0                                           //Jetsonからの原点学習モード要求判定用
#define ID_MODE_NORMAL 1                                         //Jetsonからの通常モード要求判定用
#define ID_MODE_MTORIGIN 2                                       //Jetsonからの原点学習モード要求判定用

#endif