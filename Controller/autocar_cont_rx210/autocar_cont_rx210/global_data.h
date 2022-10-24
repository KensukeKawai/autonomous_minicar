
#define MOTOR1_UH_OUT PORTC.PODR.BIT.B0
#define MOTOR1_UL_OUT PORTC.PODR.BIT.B1
#define MOTOR1_VH_OUT PORTC.PODR.BIT.B2
#define MOTOR1_VL_OUT PORTC.PODR.BIT.B3
#define MOTOR1_WH_OUT PORTC.PODR.BIT.B4
#define MOTOR1_WL_OUT PORTC.PODR.BIT.B5

#define TRICNT_TRG_UP PORTC.PODR.BIT.B6
#define SET_TRG PORT5.PODR.BIT.B0

#define MOTOR1_IRQ PORTH.PDR.BIT.B1

#define CAREER_CNT 50	//キャリア周期カウント数。三角波トップ。
#define CNTDOWN 0
#define CNTUP 1


signed short s2g_tricntr;
signed short s2g_motor1_UH_cnt;
signed short s2g_motor1_UL_cnt;
signed short s2g_motor1_VH_cnt;
signed short s2g_motor1_VL_cnt;
signed short s2g_motor1_WH_cnt;
signed short s2g_motor1_WL_cnt;

unsigned char u1g_tricntr_dir;
unsigned char u1g_tmr_trg;