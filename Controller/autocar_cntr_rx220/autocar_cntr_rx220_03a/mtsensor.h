
#ifndef _MTSENSOR_H_
#define _MTSENSOR_H_

/***************�֐�extern**************/
extern void vdg_mtsensor_encoder();

/***************�O���[�o���ϐ�extern**************/
extern volatile unsigned char u1g_mtsensor_rotdirm1;
extern volatile unsigned char u1g_mtsensor_rotdirm2;
extern volatile float f4g_mtsensor_manglem1;
extern volatile float f4g_mtsensor_manglem2;
extern volatile float f4g_mtsensor_eanglem1;
extern volatile float f4g_mtsensor_eanglem2;
extern volatile signed long s4g_mtsensor_nm1;
extern volatile signed long s4g_mtsensor_nm2;

/***************�}�N����`***************/
#define MTORIGIN_EANGLE ((float)(60))                   //���_�w�K�������d�C�p�BPhase1�z�[���h�����ꍇ�A�ǂ��ŗ��������̂��낤�E�E�E
#define MTORIGIN_MANGLE ((float)(MTORIGIN_EANGLE)/(float)(POLE_PAIRS))

#define KPLS2MANGLE ((float)(0.3))                      //���[�^���G���R�[�_�p���X�J�E���g�ˋ@�B�p�ϊ��W��
#define KPLS2EANGLE ((float)(2.1))                      //���[�^���G���R�[�_�p���X�J�E���g�˓d�C�p�ϊ��W��

#define TCNT_ENC_MID ((unsigned short)(32768))          //�O�]��]�ǂ���ɍs���Ă��@�B�p����ŃI�[�o�[/�A���_�[�t���[���Ȃ��悤�ɒ��_�ŃJ�E���g�X�^�[�g
#define TCNT_ENC_DEFAULT ((unsigned short)(TCNT_ENC_MID+(unsigned short)(MTORIGIN_EANGLE/KPLS2EANGLE))) //�J�E���^���_�{���_�w�K��p�x���J�E���g
#define TCNT_ENC_360M ((unsigned short)(360/KPLS2MANGLE))
#define TCNT_ENC_HIGH (TCNT_ENC_MID+TCNT_ENC_360M)      //�J�E���g����B����"�ȏ�"�ɂȂ�����TCNT��MID�ɖ߂�
#define TCNT_ENC_LOW (TCNT_ENC_MID-TCNT_ENC_360M)       //�J�E���g�����B����"�ȉ�"�ɂȂ�����TCNT��MID�ɖ߂�

#define NM_MAX_RANGE ((signed long)(8192))
#define NM_MIN_RANGE ((signed long)(-8192))



#endif