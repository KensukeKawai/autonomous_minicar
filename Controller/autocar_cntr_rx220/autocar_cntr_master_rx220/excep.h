
#ifndef _EXCEP_H_
#define _EXCEP_H_

extern void Excep_CMT0_CMI0();

extern void Excep_SCI1_RXI1();
extern void Excep_SCI1_TXI1();
extern void Excep_SCI5_RXI5();
extern void Excep_SCI5_TXI5();
extern void Excep_SCI6_RXI6();
extern void Excep_SCI6_TXI6();
extern void Excep_SCI9_RXI9();
extern void Excep_SCI9_TXI9();
extern void Excep_SCI12_RXI12();
extern void Excep_SCI12_TXI12();

extern void vdg_sci1_send(unsigned short senddata);
extern void vdg_sci5_send(unsigned short senddata);
extern void vdg_sci6_send(unsigned short senddata);
extern void vdg_sci9_send(unsigned short senddata);
extern void vdg_sci12_send(unsigned short senddata);

/***ÉVÉäÉAÉãí êMä÷åW***/
extern volatile unsigned char u1g_rxi1_xmtorgrq;
extern volatile unsigned char u1g_rxi1_xrcnmt;
extern volatile signed short s2g_rxi1_nmtgtfl;
extern volatile signed short s2g_rxi1_nmtgtfr;
extern volatile signed short s2g_rxi1_nmtgtrl;
extern volatile signed short s2g_rxi1_nmtgtrr;

//////////////
extern volatile unsigned char u1g_rxi5_xflmtorgfin;
extern volatile unsigned char u1g_rxi6_xfrmtorgfin;
extern volatile unsigned char u1g_rxi9_xrlmtorgfin;
extern volatile unsigned char u1g_rxi12_xrrmtorgfin;

extern volatile unsigned char u1g_cmt0_xperiod;
extern volatile unsigned char u1g_nmtset_xnmtset1;
extern volatile unsigned char u1g_nmtset_xnmtset2;
extern volatile unsigned char u1g_nmtset_xnmtset3;
extern volatile unsigned char u1g_nmtset_xnmtset4;

extern volatile unsigned short u2g_nmtset_nmtary1[4];
extern volatile unsigned short u2g_nmtset_nmtary2[4];
extern volatile unsigned short u2g_nmtset_nmtary3[4];
extern volatile unsigned short u2g_nmtset_nmtary4[4];

#endif