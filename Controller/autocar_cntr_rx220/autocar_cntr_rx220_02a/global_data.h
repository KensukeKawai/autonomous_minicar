
#ifndef GLOBAL_DATA_H_
#define GLOBAL_DATA_H_


/***ポート定義***/
#define TRG1 PORT3.PODR.BIT.B2
#define TRG2 PORT3.PODR.BIT.B1
/***ポート定義***/

/***共通***/
#define PCLK1  32000000
#define PCLK1024 31250	//PCLK1/1024

/***MTU2a***/
#define CARRIER_FREQ 5000
#define CARRIER_T 1/5000
#define CNT_HALF_CARR 3200	//　32MHz/5kHz/2
/***MTU2a***/

/***ADC***/
#define ADC_CNV_COUNT 3			//ADC連続変換回数
#define ADC_CNT2V 5/4096/(float)ADC_CNV_COUNT	//V = 5/4096 * (ADC加算後 / ADC_CNV_COUNT)
#define ADC_AVE_NUM 5			//何回分の平均を取るか設定。下限値2
#define ACS724_VMED 2.5			//0A時の電圧
#define ACS724_IRATE 5			//1Vあたりの電流変化量
/***ADC***/

/***TGIB2***/
#define NM_AVE_NUM 10			//Nmの平均回数
/***TGIB2***/


/***ハード緒元***/
#define ENC_NTEETH 54			//インタラプタ歯数
#define POLE_PAIRS 7			//極対数
#define V_PW 18				//電源電圧

/***モータ制御***/
#define CNT_DT 160
#define CNT_MIN_ON 128			//
//#define CNT_MAX 3136			//ハーフキャリアカウント-最小ONカウント/2
//#define CNT_MIN 224			//デッドタイムカウント+最小ONカウント/2
#define CNT_MAX 1800			//ハーフキャリアカウント-最小ONカウント/2
#define CNT_MIN 224			//デッドタイムカウント+最小ONカウント/2
#define DUTY_MAX 0.93
#define DUTY_MIN 0.02
#define V_MAX 16.74	//(float)DUTY_MAX*V_PW	//指令電圧上限
#define V_MIN -16.74	//-1*V_MAX

#define MAX_I 4				//電圧制限かける電流値
#define CNT_MAX_RG CNT_HALF_CARR
#define CNT_MIN_RG CNT_DT

#define KP 0.0001			//周期5kHz、100rpm偏差で0.5s間に0→18Vに到達するゲイン(100×PGAIN×5kHz = 18V)
#define KP_I_LIM 0.0003			//電流制限値超えた場合の超過偏差分FB補正用ゲイン
#define KP_RG -0.00008			//回生ブレーキ時のFBゲイン

#define STAGE1 203			//U_H,V_L　0～60°
#define STAGE2 205			//U_H,W_L  60～120°
#define STAGE3 214			//V_H,W_L  120°～180°
#define STAGE4 211			//V_H,U_L  180°～240°
#define STAGE5 229			//W_H,U_L  240°～300°
#define STAGE6 230			//W_H,V_L  300°～360°

#define CNT_MTORG 1000
#define ADV_ELANGLE 0

#define ID_ALLSET 0
#define ID_HIGHOFF 1
#define ID_LOWOFF 2
#define ID_ALLOFF 3

//原点学習時設定
#define CNT_ZLRN CNT_MIN

//オープンループ制御用//
#define KP_OL 0.0005		//DUTY_MIN_OL+KP_OL×nmtgt を目標Duty
#define DUTY_MIN_OL 0.15		//オープンループ制御時のデューティ下限。また、nmtgt=0の時の車両ホールドDuty
#define KP_ILIM_OL 0.1		//1A超過で落とすDuty
#define DUTY_MIN_ILIM_OL 0.1	//過電流時の下限Duty。過電流状態でもこれ以下にはしない
/***モータ制御***/

/***汎用係数***/
//#define K1CARCNT2NM (( 60 * (float)CARRIER_FREQ ) / (float)ENC_NTEETH)	//(1キャリアカウント数/歯数)*Fc*60 [rpm]
#define K1PLST2RPM 30/(float)(ENC_NTEETH)	//　60/(1パルス時間×歯数×2）の「60/(歯数×2)」
#define KPLSN2MANGLE 180/(float)(ENC_NTEETH)	//　パルス数×360/(歯数×2)の「360/(歯数×2)」
#define PI 3.14159265
#define KDEG2RAD PI / 180


#endif
