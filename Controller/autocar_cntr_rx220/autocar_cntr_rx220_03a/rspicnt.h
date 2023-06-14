
#ifndef _RSPICNT_H_
#define _RSPICNT_H_

/***************�֐�extern**************/
extern void vdg_rspicnt_init();
extern void vdg_rspicnt_recget();
extern void vdg_rspicnt_sendset(unsigned char mode);

/***************�O���[�o���ϐ�extern**************/
extern volatile unsigned char u1g_rspicnt_idmoderq;
extern volatile signed long s4g_rspicnt_nm1tgt;
extern volatile signed long s4g_rspicnt_nm2tgt;

/***************�}�N����`***************/
#define ID_MODE_STP 1                                                   //���
#define ID_MODE_RUN 2                                                   //�ʏ푖�s
#define ID_MODE_ORG 3                                                   //���_�w�K
#define ID_MODE_RUNTOSTP 4                                              //�ʏ푖�s����ԑJ�ڒ�
#define ID_MODE_RECONLY 15                                              //Jetson�ɏ�Ԃ̂ݑ��M���郂�[�h

#define BITMASK_MODE ((unsigned long)(0xF0000000))                      //MSB����4bit����Ԕ���p�}�X�N
#define BITMASK_MODE_STP ((unsigned long)(ID_MODE_STP<<28))
#define BITMASK_MODE_RUN ((unsigned long)(ID_MODE_RUN<<28))             //�ʏ푖�s���[�h�v��"1"�}�X�N�p
#define BITMASK_MODE_ORG ((unsigned long)(ID_MODE_ORG<<28))             //���_�w�K�v��"2"�}�X�N�p
#define BITMASK_MODE_RUNTOSTP ((unsigned long)(ID_MODE_RUNTOSTP<<28))   //���s������ԑJ�ڒ�"3"�}�X�N�p
#define BITMASK_ROTDIR ((unsigned long)(1<<13))                         //Motor�̉�]���@����p�r�b�g�}�X�N
#define BITMASK_NMTGT ((unsigned long)(0x1FFF))                         //���[�^�ڕW��]���p�r�b�g�}�X�N�B13bit
#define BITSHIFT_MODE ((unsigned long)(28))                             //��Ԕ���p���4bit�p�r�b�g�V�t�g��
#define BITSHIFT_NM2 ((unsigned long)(14))                              //Motor2��]���p�r�b�g�V�t�g��

#endif