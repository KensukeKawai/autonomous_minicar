
#include "common.h"

/***************�O���[�o���ϐ���`***************/
volatile unsigned char u1g_mtsensor_rotdirm1;
volatile unsigned char u1g_mtsensor_rotdirm2;
volatile float f4g_mtsensor_manglem1;
volatile float f4g_mtsensor_manglem2;
volatile float f4g_mtsensor_eanglem1;
volatile float f4g_mtsensor_eanglem2;
volatile signed long s4g_mtsensor_nm1;
volatile signed long s4g_mtsensor_nm2;

void vdg_mtsensor_encoder()
{
    /**********static�ϐ���`**********/
    volatile static unsigned short u2s_mtsensor_enccntm1;
    volatile static unsigned short u2s_mtsensor_enccntm2;
    volatile static unsigned short u2s_mtsensor_enccntm1z = 0;
    volatile static unsigned short u2s_mtsensor_enccntm2z = 0;

    /**********�e���|�����ϐ���`**********/
    volatile signed long s4t_mtsensor_denccntm1;
    volatile signed long s4t_mtsensor_denccntm2;
    volatile unsigned short u2t_mtsensor_cnte360;
    volatile float f4t_mtsensor_eanglepre;
    volatile signed long s4t_mtsensor_nm1pre;
    volatile signed long s4t_mtsensor_nm2pre;

    //MTU2�FMotor1�AMTU1�FMotor2
    //MTU�ƃ��[�^�ԍ����t�ł��邱�Ƃɒ���

    //���[�^��]�����̌��o
    // u1g_mtsensor_rotdirm1 = MTU2.TSR.BIT.TCFD;  //���]�F0�A�t�]�F1
    // u1g_mtsensor_rotdirm2 = MTU1.TSR.BIT.TCFD;  //���]�F0�A�t�]�F1

    //Motor1
    /*****��]�������o�A�J�E���g�O��l�����Z�o*****/
    u2s_mtsensor_enccntm1 = MTU_M1_ENCTCNT;
    s4t_mtsensor_denccntm1 = (signed long)(u2s_mtsensor_enccntm1)-(signed long)(u2s_mtsensor_enccntm1z);
    if(s4t_mtsensor_denccntm1>=0){u1g_mtsensor_rotdirm1=0;}
    else{u1g_mtsensor_rotdirm1=1;}

    /*****�G���R�[�_�p���X�J�E���g�����`�@�B�p�Z�o*****/
    //�J�E���g�㉺������O��Ă���ꍇ�͒��_�ɖ߂��B�J�E���g�l�擾�㑦���Ɏ��{�K�v�Ȃ��߂����Ŏ��{
    //HIGH���J�E���g
    if(u2s_mtsensor_enccntm1>=TCNT_ENC_HIGH)
    {
        u2s_mtsensor_enccntm1 = u2s_mtsensor_enccntm1 - TCNT_ENC_360M;
        MTU_M1_ENCTCNT = u2s_mtsensor_enccntm1;
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //�J�E���g��LOW
    else if(u2s_mtsensor_enccntm1<=TCNT_ENC_LOW)
    {
        u2s_mtsensor_enccntm1 = u2s_mtsensor_enccntm1 + TCNT_ENC_360M;
        MTU_M1_ENCTCNT = u2s_mtsensor_enccntm1;
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }
    //MID���J�E���g��HIGH
    else if(u2s_mtsensor_enccntm1>=TCNT_ENC_MID && u2s_mtsensor_enccntm1 < TCNT_ENC_HIGH)
    {
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //LOW���J�E���g��MID
    else if(u2s_mtsensor_enccntm1<TCNT_ENC_MID && u2s_mtsensor_enccntm1 > TCNT_ENC_LOW)
    {
        f4g_mtsensor_manglem1 = (float)(u2s_mtsensor_enccntm1-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }

    //Motor2
    /*****��]�������o�A�J�E���g�O��l�����Z�o*****/
    u2s_mtsensor_enccntm2 = MTU_M2_ENCTCNT;
    s4t_mtsensor_denccntm2 = (signed long)(u2s_mtsensor_enccntm2)-(signed long)(u2s_mtsensor_enccntm2z);
    if(s4t_mtsensor_denccntm2>=0){u1g_mtsensor_rotdirm2=0;}
    else{u1g_mtsensor_rotdirm2=1;}

    /*****�G���R�[�_�p���X�J�E���g�����`�@�B�p�Z�o*****/
    //�J�E���g�㉺������O��Ă���ꍇ�͒��_�ɖ߂��B�J�E���g�l�擾�㑦���Ɏ��{�K�v�Ȃ��߂����Ŏ��{
    //HIGH���J�E���g
    if(u2s_mtsensor_enccntm2>=TCNT_ENC_HIGH)
    {
        u2s_mtsensor_enccntm2 = u2s_mtsensor_enccntm2 - TCNT_ENC_360M;
        MTU_M2_ENCTCNT = u2s_mtsensor_enccntm2;
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //�J�E���g��LOW
    else if(u2s_mtsensor_enccntm2<=TCNT_ENC_LOW)
    {
        u2s_mtsensor_enccntm2 = u2s_mtsensor_enccntm2 + TCNT_ENC_360M;
        MTU_M2_ENCTCNT = u2s_mtsensor_enccntm2;
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }
    //MID���J�E���g��HIGH
    else if(u2s_mtsensor_enccntm2>=TCNT_ENC_MID && u2s_mtsensor_enccntm2 < TCNT_ENC_HIGH)
    {
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_MID) * (float)KPLS2MANGLE;
    }
    //LOW���J�E���g��MID
    else if(u2s_mtsensor_enccntm2<TCNT_ENC_MID && u2s_mtsensor_enccntm2 > TCNT_ENC_LOW)
    {
        f4g_mtsensor_manglem2 = (float)(u2s_mtsensor_enccntm2-TCNT_ENC_LOW) * (float)KPLS2MANGLE;
    }

    /*****�J�E���g�l�̑O��l���b�`����*****/
    u2s_mtsensor_enccntm1z = u2s_mtsensor_enccntm1;
    u2s_mtsensor_enccntm2z = u2s_mtsensor_enccntm2;

    /*****��]���Z�o*****/
    //���@�B�p(1����������)���J�E���g�~�J�E���gto�@�B�p�W���~�L�����A���g��/360�~60
    // s4t_mtsensor_nm1pre = (signed long)((float)s4t_mtsensor_denccntm1*KPLS2MANGLE*HZ_CARRIER/6);
    s4t_mtsensor_nm1pre = (signed long)((float)(s4t_mtsensor_denccntm1)*KPLS2MANGLE*(float)(HZ_CARRIER)/(float)(6));
    s4t_mtsensor_nm2pre = (signed long)((float)(s4t_mtsensor_denccntm2)*KPLS2MANGLE*(float)(HZ_CARRIER)/(float)(6));

    //�����[�^��]���ɏ㉺�������BJetson�ւ̑��Mbit���s���ケ���Ń����W���Ɏ��߂�B
    if (s4t_mtsensor_nm1pre > NM_MAX_RANGE) { s4g_mtsensor_nm1 = NM_MAX_RANGE; }
    else if(s4t_mtsensor_nm1pre < NM_MIN_RANGE) { s4g_mtsensor_nm1 = NM_MIN_RANGE; }
    else { s4g_mtsensor_nm1 = s4t_mtsensor_nm1pre; }
    if (s4t_mtsensor_nm2pre > NM_MAX_RANGE) { s4g_mtsensor_nm2 = NM_MAX_RANGE; }
    else if(s4t_mtsensor_nm2pre < NM_MIN_RANGE) { s4g_mtsensor_nm2 = NM_MIN_RANGE; }
    else { s4g_mtsensor_nm2 = s4t_mtsensor_nm2pre; }

    /*****�d�C�p�Z�o*****/
    //mangle����d�C�p�Z�o
    //Motor1
    f4t_mtsensor_eanglepre = f4g_mtsensor_manglem1*(float)POLE_PAIRS;
    u2t_mtsensor_cnte360 = (unsigned short)(f4t_mtsensor_eanglepre/360);
    f4g_mtsensor_eanglem1 = f4t_mtsensor_eanglepre - (float)u2t_mtsensor_cnte360*360;
    //Motor2
    f4t_mtsensor_eanglepre = f4g_mtsensor_manglem2*(float)POLE_PAIRS;
    u2t_mtsensor_cnte360 = (unsigned short)(f4t_mtsensor_eanglepre/360);
    f4g_mtsensor_eanglem2 = f4t_mtsensor_eanglepre - (float)u2t_mtsensor_cnte360*360;
}