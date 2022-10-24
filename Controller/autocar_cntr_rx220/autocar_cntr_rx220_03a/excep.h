
#ifndef _EXCEP_H_
#define _EXCEP_H_

/***************�֐�extern**************/
extern void Excep_S12AD_S12ADI0();
extern void Excep_MTU2_TGIB2();
extern void Excep_MTU2_TCIV2();
extern void Excep_SCI1_RXI1();
extern void Excep_SCI1_TXI1();
extern void Excep_SCI12_RXI12();
extern void Excep_SCI12_TXI12();

/***************�O���[�o���ϐ�extern**************/
/***MTU0.TGIA0�����ݕϐ���`***/
extern volatile unsigned char u1g_extgia0_interrupt;

/***�d���Z���TADC�֌WRAM��`***/
extern volatile unsigned short u2g_exs12adi0_adccntm1ad;
extern volatile unsigned short u2g_exs12adi0_adccntm2ad;
extern volatile unsigned char u1g_exs12adi0_xadcex;

/***SPI�֌W�ϐ���`***/
extern volatile unsigned char u1g_exspri0_xrspirec;




/***��]���֌WRAM��`***/
extern volatile float f4g_tgib2_nmsm;
extern volatile unsigned short u2g_tgib2_citrpt;
extern volatile unsigned char u1g_tgib2_xzero;
extern volatile float f4g_tgib2_elangle;
extern volatile float f4g_tgib2_mcangle;
extern volatile unsigned char u1g_tciv2_xovflw;
extern volatile unsigned char u1g_tciv2_xdnmrst;

/***�V���A���ʐM�֌W***/
extern volatile unsigned char u1g_rxi12_xmtorgrq;
extern volatile unsigned char u1g_rxi12_xrcnmt;
extern volatile signed short s2g_rxi12_nmtgt;
extern volatile unsigned char u1g_rxi12_nmtary[4];


/**********�}�N����`*********/


#endif