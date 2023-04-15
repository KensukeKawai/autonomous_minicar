
#ifndef _RSPICNT_H_
#define _RSPICNT_H_

/***************�֐�extern**************/
extern void vdg_rspicnt_recget();

/***************�O���[�o���ϐ�extern**************/
extern volatile unsigned char u1g_rspicnt_idmoderq;
extern volatile signed long s4g_rspicnt_nm1tgt;
extern volatile signed long s4g_rspicnt_nm2tgt;

/***************�}�N����`***************/
#define BITMASK_MODE_NORMAL ((unsigned long)(1<<28))             //�ʏ푖�s���[�h�v��28�r�b�g�ڃ}�X�N�p
#define BITMASK_MODE_MTORIGIN ((unsigned long)(1<<29))           //���_�w�K�v���r�b�g29�r�b�g�ڃ}�X�N�p
#define BITMASK_MODE_STOP ((unsigned long)(1<<30))               //��ԗv���r�b�g30�r�b�g�}�X�N�p
#define BITMASK_ROTDIR ((unsigned long)(1<<13))                  //Motor�̉�]���@����p�r�b�g�}�X�N
#define BITMASK_NMTGT ((unsigned long)(0x1FFF))                  //���[�^�ڕW��]���p�r�b�g�}�X�N�B13bit
#define BITSHIFT_NM2 ((unsigned long)(14))                       //Motor2��]���p�r�b�g�V�t�g��
#define ID_MODE_STOP 0                                           //Jetson����̌��_�w�K���[�h�v������p
#define ID_MODE_NORMAL 1                                         //Jetson����̒ʏ탂�[�h�v������p
#define ID_MODE_MTORIGIN 2                                       //Jetson����̌��_�w�K���[�h�v������p

#endif