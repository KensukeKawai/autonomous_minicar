
#ifndef _MTCNT_H_
#define _MTCNT_H_

/***************関数extern**************/
extern void vdg_mtcnt_nmsmcal();
extern void vdg_mtcnt_stagejdg(unsigned char motor, unsigned char id_direction);
extern void vdg_mtcnt_outset(unsigned char setmotor, unsigned char mode, unsigned short cntduty);
extern void vdg_mtcnt_orthantjdg();
extern void vdg_mtcnt_stagephasejdg();
extern void vdg_mtcnt_tgrpwrcalm1();
extern void vdg_mtcnt_tgrpwrcalm2();
extern void vdg_mtcnt_tgrregcalm1();
extern void vdg_mtcnt_tgrregcalm2();
extern void vdg_mtcnt_mtorigin();

/***************グローバル変数extern**************/
extern volatile signed long s4g_mtcnt_nmsm1;
extern volatile signed long s4g_mtcnt_nmsm2;
extern volatile unsigned char u1g_mtcnt_idstagem1;
extern volatile unsigned char u1g_mtcnt_idstagem2;

extern volatile unsigned char u1g_mtcnt_idorthantm1;
extern volatile unsigned char u1g_mtcnt_idorthantm2;

extern volatile unsigned short u2g_mtcnt_cntm1;
extern volatile unsigned short u2g_mtcnt_cntm2;

extern volatile unsigned char u1g_mtcnt_xnormal;
extern volatile unsigned char u1g_mtcnt_xmtorigin;
extern volatile unsigned char u1g_mtcnt_xstop;


// extern volatile float f4g_mtcnt_nmsm;
// extern volatile float f4g_mtcnt_vtgt;
// extern volatile unsigned char u1g_mtcnt_xrgmode;
// extern volatile float f4g_mtcnt_nmtgt;


/***************マクロ定義**************/
#define LPF_CUTF_NM ((float)(500))    //モータ回転数の1次LPFフィルタカットオフ周波数
#define LPF_TAU_NM ((float)(1/(2*PI*LPF_CUTF_NM)))  //モータ回転数の1次LPFフィルタ時定数
#define LPF_A ((float)(T_CARRIER+2*LPF_TAU_NM))      //離散LPF係数
#define LPF_B ((float)(T_CARRIER-2*LPF_TAU_NM))      //離散LPF係数
#define LPF_K1 ((float)(T_CARRIER/LPF_A))
#define LPF_K2 ((float)(LPF_B/LPF_A))

/***ID定義***/
#define ID_MOTOR1 1
#define ID_MOTOR2 2

#define ID_ALLSET 10
#define ID_LOW_ON 11
#define ID_HIGH_ON 12
#define ID_ALLOFF 13

#define ID_STAGE1 1
#define ID_STAGE2 2
#define ID_STAGE3 3
#define ID_STAGE4 4
#define ID_STAGE5 5
#define ID_STAGE6 6

#define ID_MTRUN_FWDPWR 1
#define ID_MTRUN_FWDREG 2
#define ID_MTRUN_REVPWR 3
#define ID_MTRUN_REVREG 4
// #define ID_MTRUN_PWR 1
// #define ID_MTRUN_REG 2
// #define ID_MTRUN_FWD 1
// #define ID_MTRUN_REV 2

#define ID_MT_ADV 1
#define ID_MT_BACK 2


/***モータ制御***/
#define CNT_DEADTIME ((unsigned short)(160))            //PCLK×最小OFF時間。ON⇒OFF⇒ONでちゃんとスイッチングできる最小カウント。PCLK×デッドタイム
#define CNT_MIN_PRE ((unsigned short)(128))                 //PCLK×最小ON時間
#define CNT_MAX ((unsigned short)(CNT_CARRIER-CNT_MIN_PRE))
// #define CNT_MAX (unsigned short)((CNT_CARRIER-CNT_DEADTIME))
#define CNT_MAX_PRE ((unsigned short)(3000)) 
// #define CNT_MIN ((unsigned short)(CNT_CARRIER-CNT_MAX_PRE))
#define CNT_MIN ((unsigned short)(CNT_CARRIER-CNT_MAX_PRE))

#define CNT_OUTOFF (unsigned short)((CNT_CARRIER+10))

#define DUTY_MIN ((float)(0.02))
#define DUTY_MAX ((float)(0.4))        //0.98
#define DUTY_MINFF ((float)(0.02))
#define DUTY_MAXFF ((float)(0.3))
#define DUTY_MAXFB ((float)(0.2))
#define DUTY_MINILIM ((float)(-0.5))

#define DUTY_MINREG ((float)(0.02))
#define DUTY_MAXREG ((float)(0.2))
#define DUTY_MAXFFREG ((float)(0.3))
#define DUTY_MAXFBREG ((float)(0.3))
#define DUTY_MINILIMREG ((float)(-0.5))

#define KP_FB ((float)(0.002))
#define KI_FB ((float)(0.00001))
#define KP_ILIM ((float)(0.2))

#define I_MAX ((float)(2.5))
#define I_MIN ((float)(-2))

/***原点学習***/
#define CNT_ORIGINDUTY ((unsigned short)(6100))

#endif