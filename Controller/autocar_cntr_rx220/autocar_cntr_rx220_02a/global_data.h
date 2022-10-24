
#ifndef GLOBAL_DATA_H_
#define GLOBAL_DATA_H_


/***�|�[�g��`***/
#define TRG1 PORT3.PODR.BIT.B2
#define TRG2 PORT3.PODR.BIT.B1
/***�|�[�g��`***/

/***����***/
#define PCLK1  32000000
#define PCLK1024 31250	//PCLK1/1024

/***MTU2a***/
#define CARRIER_FREQ 5000
#define CARRIER_T 1/5000
#define CNT_HALF_CARR 3200	//�@32MHz/5kHz/2
/***MTU2a***/

/***ADC***/
#define ADC_CNV_COUNT 3			//ADC�A���ϊ���
#define ADC_CNT2V 5/4096/(float)ADC_CNV_COUNT	//V = 5/4096 * (ADC���Z�� / ADC_CNV_COUNT)
#define ADC_AVE_NUM 5			//���񕪂̕��ς���邩�ݒ�B�����l2
#define ACS724_VMED 2.5			//0A���̓d��
#define ACS724_IRATE 5			//1V������̓d���ω���
/***ADC***/

/***TGIB2***/
#define NM_AVE_NUM 10			//Nm�̕��ω�
/***TGIB2***/


/***�n�[�h����***/
#define ENC_NTEETH 54			//�C���^���v�^����
#define POLE_PAIRS 7			//�ɑΐ�
#define V_PW 18				//�d���d��

/***���[�^����***/
#define CNT_DT 160
#define CNT_MIN_ON 128			//
//#define CNT_MAX 3136			//�n�[�t�L�����A�J�E���g-�ŏ�ON�J�E���g/2
//#define CNT_MIN 224			//�f�b�h�^�C���J�E���g+�ŏ�ON�J�E���g/2
#define CNT_MAX 1800			//�n�[�t�L�����A�J�E���g-�ŏ�ON�J�E���g/2
#define CNT_MIN 224			//�f�b�h�^�C���J�E���g+�ŏ�ON�J�E���g/2
#define DUTY_MAX 0.93
#define DUTY_MIN 0.02
#define V_MAX 16.74	//(float)DUTY_MAX*V_PW	//�w�ߓd�����
#define V_MIN -16.74	//-1*V_MAX

#define MAX_I 4				//�d������������d���l
#define CNT_MAX_RG CNT_HALF_CARR
#define CNT_MIN_RG CNT_DT

#define KP 0.0001			//����5kHz�A100rpm�΍���0.5s�Ԃ�0��18V�ɓ��B����Q�C��(100�~PGAIN�~5kHz = 18V)
#define KP_I_LIM 0.0003			//�d�������l�������ꍇ�̒��ߕ΍���FB�␳�p�Q�C��
#define KP_RG -0.00008			//�񐶃u���[�L����FB�Q�C��

#define STAGE1 203			//U_H,V_L�@0�`60��
#define STAGE2 205			//U_H,W_L  60�`120��
#define STAGE3 214			//V_H,W_L  120���`180��
#define STAGE4 211			//V_H,U_L  180���`240��
#define STAGE5 229			//W_H,U_L  240���`300��
#define STAGE6 230			//W_H,V_L  300���`360��

#define CNT_MTORG 1000
#define ADV_ELANGLE 0

#define ID_ALLSET 0
#define ID_HIGHOFF 1
#define ID_LOWOFF 2
#define ID_ALLOFF 3

//���_�w�K���ݒ�
#define CNT_ZLRN CNT_MIN

//�I�[�v�����[�v����p//
#define KP_OL 0.0005		//DUTY_MIN_OL+KP_OL�~nmtgt ��ڕWDuty
#define DUTY_MIN_OL 0.15		//�I�[�v�����[�v���䎞�̃f���[�e�B�����B�܂��Anmtgt=0�̎��̎ԗ��z�[���hDuty
#define KP_ILIM_OL 0.1		//1A���߂ŗ��Ƃ�Duty
#define DUTY_MIN_ILIM_OL 0.1	//�ߓd�����̉���Duty�B�ߓd����Ԃł�����ȉ��ɂ͂��Ȃ�
/***���[�^����***/

/***�ėp�W��***/
//#define K1CARCNT2NM (( 60 * (float)CARRIER_FREQ ) / (float)ENC_NTEETH)	//(1�L�����A�J�E���g��/����)*Fc*60 [rpm]
#define K1PLST2RPM 30/(float)(ENC_NTEETH)	//�@60/(1�p���X���ԁ~�����~2�j�́u60/(�����~2)�v
#define KPLSN2MANGLE 180/(float)(ENC_NTEETH)	//�@�p���X���~360/(�����~2)�́u360/(�����~2)�v
#define PI 3.14159265
#define KDEG2RAD PI / 180


#endif
