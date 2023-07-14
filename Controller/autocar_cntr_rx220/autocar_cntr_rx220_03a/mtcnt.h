
#ifndef _MTCNT_H_
#define _MTCNT_H_

/***************�֐�extern**************/
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

/***************�O���[�o���ϐ�extern**************/
extern volatile signed long s4g_mtcnt_nmsm1;
extern volatile signed long s4g_mtcnt_nmsm2;
extern volatile unsigned char u1g_mtcnt_idstagem1;
extern volatile unsigned char u1g_mtcnt_idstagem2;

extern volatile unsigned char u1g_mtcnt_idorthantm1;
extern volatile unsigned char u1g_mtcnt_idorthantm2;

extern volatile unsigned char u1g_mtcnt_idmode;

extern volatile unsigned short u2g_mtcnt_cntm1;
extern volatile unsigned short u2g_mtcnt_cntm2;

extern volatile unsigned char u1g_mtcnt_xnormal;
extern volatile unsigned char u1g_mtcnt_xmtorigin;
extern volatile unsigned char u1g_mtcnt_xstop;

/***************�}�N����`**************/
#define LPF_CUTF_NM ((float)(3))    //���[�^��]����1��LPF�t�B���^�J�b�g�I�t���g��
#define LPF_TAU_NM ((float)(1/(2*PI*LPF_CUTF_NM)))  //���[�^��]����1��LPF�t�B���^���萔
#define LPF_A ((float)(T_CARRIER+2*LPF_TAU_NM))      //���ULPF�W��
#define LPF_B ((float)(T_CARRIER-2*LPF_TAU_NM))      //���ULPF�W��
#define LPF_K1 ((float)(T_CARRIER/LPF_A))
#define LPF_K2 ((float)(LPF_B/LPF_A))

/***ID��`***/
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
#define ID_MT_ADV 1
#define ID_MT_BACK 2

/***��]������p�����[�^***/
#define KP_FF ((float)(0.5))                //�uKP_FF�~nmtgt/NMAX�~�iDUTY_MAXFF-DUTY_MINFF)�v
#define KP_FB ((float)(0.0004))              //P�Q�C��
#define KI_FB ((float)(0.00005))            //I�Q�C��

/*****�d������*****/
#define I_MAX ((float)(2))                //�͍s���̓d�������l
#define I_MIN ((float)(-2))                 //�񐶎��̓d�������l
#define KP_ILIM ((float)(0.2))              //�d������FB����P�Q�C��

/*****��Ԕ���*****/
#define NM_STP 15                           //����]��������ȉ����������Ԕ���.����������ƃn���`���O����

/***Duty�ݒ�***/
//�͍s���p�����[�^
#define DUTY_MIN ((float)(0.02))            //�͍s���̉���Duty
#define DUTY_MAX ((float)(0.4))             //�͍s���̏��Duty
//�ԏd�ɂ���ĕς���K�v����//
#define DUTY_MINFF ((float)(DUTY_MIN+0.015))          //FF���̃I�t�Z�b�gDuty�B�ڕW��]���ɂ�炸�Œ���m�ۂ���Duty
#define DUTY_MAXFF ((float)(0.2))           //FF���̍ő�Duty
#define DUTY_MAXFB ((float)(DUTY_MAX-DUTY_MAXFF))           //FB���̍ő�Duty
#define DUTY_MINFB ((float)(-1 * DUTY_MAXFB))           //FB���̍ŏ�Duty
#define DUTY_MAXFBI ((float)(0.05))                 //�ϕ����̏���K�[�h
#define DUTY_MINFBI ((float)(-1 * DUTY_MAXFBI))                 //�ϕ����̉����K�[�h
#define DUTY_MINILIM ((float)(-1 * DUTY_MAX))        //�d������FB���̍ő�Duty�B��{DUTY_MAX�̕������]�B
//�񐶎��p�����[�^
#define DUTY_MINREG ((float)(0.02))         //�񐶎��̉���Duty
#define DUTY_MAXREG ((float)(0.3))          //�񐶎��̏��Duty
#define DUTY_MINFFREG ((float)(0.02))          //FF���̃I�t�Z�b�gDuty�B�ڕW��]���ɂ�炸�Œ���m�ۂ���Duty
#define DUTY_MAXFFREG ((float)(0.1))        //FF���̍ő�Duty
#define DUTY_MAXFBREG ((float)(0.2))        //FB���̍ő�Duty
#define DUTY_MINILIMREG ((float)(-0.3))     //�d������FB���̍ő�Duty

/***�J�E���g�ݒ�***/
#define CNT_OUTOFF (unsigned short)((CNT_CARRIER+10))
#define CNT_MIN_PRE ((unsigned short)(128))                     //PCLK�~�ŏ�ON����
#define CNT_MAX_PRE ((unsigned short)(6240))                    //PCLK�~�ő�ON����(=�L�����A����-�ŏ�OFF����=�f�b�h�^�C��)
#define CNT_MAX ((unsigned short)(CNT_CARRIER-CNT_MIN_PRE))     //�J�E���g���]����
#define CNT_MIN ((unsigned short)(CNT_CARRIER-CNT_MAX_PRE))     //�J�E���g���]����
// #define CNT_DEADTIME ((unsigned short)(160))                 //PCLK�~�ŏ�OFF���ԁBON��OFF��ON�ł����ƃX�C�b�`���O�ł���ŏ��J�E���g�BPCLK�~�f�b�h�^�C��

/***���_�w�K***/
#define CNT_ORIGINDUTY ((unsigned short)(6100))

#endif