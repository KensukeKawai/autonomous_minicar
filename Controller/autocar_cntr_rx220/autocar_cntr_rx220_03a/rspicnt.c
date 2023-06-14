
#include "common.h"
// #include "mtcnt.h"


/***************�O���[�o���ϐ���`***************/
volatile unsigned char u1g_rspicnt_idmoderq;
volatile signed long s4g_rspicnt_nm1tgt;
volatile signed long s4g_rspicnt_nm2tgt;

void vdg_rspicnt_init()
{
    /*****�O���[�o���ϐ�������*****/
    u1g_rspicnt_idmoderq = ID_MODE_STP;
}

//��M�L������A��M���Ă�����ڕW��]���Ɛ��䃂�[�h�X�V
void vdg_rspicnt_recget()
{
    /**********�e���|�����ϐ���`**********/
    volatile unsigned long u4t_rspicnt_spdrrec;
    volatile unsigned long u4t_rspicnt_spdrrecmd;
    volatile unsigned long u4t_rspicnt_spdrrectmp2;

    u4t_rspicnt_spdrrec = RSPI0.SPDR.LONG;      //Long Word�Ŏ�M�f�[�^�擾������

    /*****�������ꂽbit�ɑΉ����锻�f�A�f�[�^����*****/
    //���䃂�[�h�v���擾
    // u4t_rspicnt_spdrrecmd = u4t_rspicnt_spdrrec & BITMASK_MODE;
    u4t_rspicnt_spdrrecmd = u4t_rspicnt_spdrrec >> BITSHIFT_MODE;

    switch (u4t_rspicnt_spdrrecmd)
    {
        case ID_MODE_STP:
            u1g_rspicnt_idmoderq = ID_MODE_STP;
            s4g_rspicnt_nm1tgt = 0;
            s4g_rspicnt_nm2tgt = 0;
        break;

        case ID_MODE_RUN:
            u1g_rspicnt_idmoderq = ID_MODE_RUN;
            //Motor1
            if(u4t_rspicnt_spdrrec & BITMASK_ROTDIR)
            {
                s4g_rspicnt_nm1tgt = -1 * (signed long)(u4t_rspicnt_spdrrec & BITMASK_NMTGT);
            }
            else
            {
                s4g_rspicnt_nm1tgt = (signed long)(u4t_rspicnt_spdrrec & BITMASK_NMTGT);
            }
            //Motor2
            //Motor2�ڕW��]����LSB�X�^�[�g�ɂ��邽�߂Ƀr�b�g�V�t�g
            u4t_rspicnt_spdrrectmp2 = u4t_rspicnt_spdrrec>>14;
            if(u4t_rspicnt_spdrrectmp2 & BITMASK_ROTDIR)
            {
                s4g_rspicnt_nm2tgt = -1 * (signed long)(u4t_rspicnt_spdrrectmp2 & BITMASK_NMTGT);
            }
            else
            {
                s4g_rspicnt_nm2tgt = (signed long)(u4t_rspicnt_spdrrectmp2 & BITMASK_NMTGT);
            }
        break;   

        case ID_MODE_ORG:
            u1g_rspicnt_idmoderq = ID_MODE_ORG;
            s4g_rspicnt_nm1tgt = 0;
            s4g_rspicnt_nm2tgt = 0;
        break;

        case ID_MODE_RECONLY:
            //Jetson�ւ̏�ԑ��M�݂̂̏ꍇ�͏�ԍX�V�ANm�r�b�g�����s�v
        break;

        default://�S�ėv�����Ȃ������ꍇ�͒�ԂƂ���
            u1g_rspicnt_idmoderq = ID_MODE_STP;
        break;
    }

    // RSPI0.SPDR.LONG = u4t_rspicnt_spdrrec;  // Tmp
}

void vdg_rspicnt_sendset(unsigned char mode)
{
    volatile unsigned long u4t_rspicnt_nmsm1;
    volatile unsigned long u4t_rspicnt_nmsm2;
    volatile unsigned long u4t_rspicnt_spdrset;

    //���[�^�R���g���[�����̃��[�^�����Ԃɉ�����Jetson�ɏ�ԂƉ�]����ʒm
    switch(mode)
    {
        case ID_MODE_RUN:
            //�����[�^��]���̕������菈��
            if(s4g_mtcnt_nmsm1 >= 0)
            {
                u4t_rspicnt_nmsm1 = (unsigned long)(s4g_mtcnt_nmsm1);
            }
            else
            {
                u4t_rspicnt_nmsm1 = (unsigned long)(abs(s4g_mtcnt_nmsm1));
                u4t_rspicnt_nmsm1 = u4t_rspicnt_nmsm1 | BITMASK_ROTDIR;
            }
            if(s4g_mtcnt_nmsm2 >= 0)
            {
                u4t_rspicnt_nmsm2 = (unsigned long)(s4g_mtcnt_nmsm2);
            }
            else
            {
                u4t_rspicnt_nmsm2 = (unsigned long)(abs(s4g_mtcnt_nmsm2));
                u4t_rspicnt_nmsm2 = u4t_rspicnt_nmsm2 | BITMASK_ROTDIR;
            }
            u4t_rspicnt_spdrset = u4t_rspicnt_nmsm1 | (u4t_rspicnt_nmsm2<<BITSHIFT_NM2);
            u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_RUN;
        break;

        case ID_MODE_ORG:
            u4t_rspicnt_spdrset = 0;
            u4t_rspicnt_spdrset = u4t_rspicnt_spdrset | BITMASK_MODE_ORG;
        break;
        
        case ID_MODE_STP:
            u4t_rspicnt_spdrset = BITMASK_MODE_STP;
        break;

        case ID_MODE_RUNTOSTP:
            u4t_rspicnt_spdrset = BITMASK_MODE_RUNTOSTP;
        break;
    }

    RSPI0.SPDR.LONG = u4t_rspicnt_spdrset;
}