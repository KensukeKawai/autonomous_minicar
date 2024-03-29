/************************************************************************
*
* Device     : RX/RX200/RX220
*
* File Name  : vect.h
*
* Abstract   : Definition of Vector.
*
* History    : 1.00  (2012-11-30)  [Hardware Manual Revision : 1.00]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2012 Renesas Electronics Corporation and
* Renesas Solutions Corp. All rights reserved.
*
********************************************************************+++*/

// Exception(Supervisor Instruction)
#pragma interrupt (Excep_SuperVisorInst)
void Excep_SuperVisorInst(void);

// Exception(Undefined Instruction)
#pragma interrupt (Excep_UndefinedInst)
void Excep_UndefinedInst(void);

// NMI
#pragma interrupt (NonMaskableInterrupt)
void NonMaskableInterrupt(void);

// Dummy
#pragma interrupt (Dummy)
void Dummy(void);

// BRK
#pragma interrupt (Excep_BRK(vect=0))
void Excep_BRK(void);

// BSC BUSERR
#pragma interrupt (Excep_BSC_BUSERR(vect=16))
void Excep_BSC_BUSERR(void);

// FCU FIFERR
#pragma interrupt (Excep_FCU_FIFERR(vect=21))
void Excep_FCU_FIFERR(void);

// FCU FRDYI
#pragma interrupt (Excep_FCU_FRDYI(vect=23))
void Excep_FCU_FRDYI(void);

// ICU SWINT
#pragma interrupt (Excep_ICU_SWINT(vect=27))
void Excep_ICU_SWINT(void);

// CMT0 CMI0
#pragma interrupt (Excep_CMT0_CMI0(vect=28))
void Excep_CMT0_CMI0(void);

// CMT1 CMI1
#pragma interrupt (Excep_CMT1_CMI1(vect=29))
void Excep_CMT1_CMI1(void);

// CMT2 CMI2
#pragma interrupt (Excep_CMT2_CMI2(vect=30))
void Excep_CMT2_CMI2(void);

// CMT3 CMI3
#pragma interrupt (Excep_CMT3_CMI3(vect=31))
void Excep_CMT3_CMI3(void);

// CAC FERRF
#pragma interrupt (Excep_CAC_FERRF(vect=32))
void Excep_CAC_FERRF(void);

// CAC MENDF
#pragma interrupt (Excep_CAC_MENDF(vect=33))
void Excep_CAC_MENDF(void);

// CAC OVFF
#pragma interrupt (Excep_CAC_OVFF(vect=34))
void Excep_CAC_OVFF(void);

// RSPI0 SPEI0
#pragma interrupt (Excep_RSPI0_SPEI0(vect=44))
void Excep_RSPI0_SPEI0(void);

// RSPI0 SPRI0
#pragma interrupt (Excep_RSPI0_SPRI0(vect=45))
void Excep_RSPI0_SPRI0(void);

// RSPI0 SPTI0
#pragma interrupt (Excep_RSPI0_SPTI0(vect=46))
void Excep_RSPI0_SPTI0(void);

// RSPI0 SPII0
#pragma interrupt (Excep_RSPI0_SPII0(vect=47))
void Excep_RSPI0_SPII0(void);

// DOC DOPCF
#pragma interrupt (Excep_DOC_DOPCF(vect=57))
void Excep_DOC_DOPCF(void);

// RTC CUP
#pragma interrupt (Excep_RTC_CUP(vect=63))
void Excep_RTC_CUP(void);

// ICU IRQ0
#pragma interrupt (Excep_ICU_IRQ0(vect=64))
void Excep_ICU_IRQ0(void);

// ICU IRQ1
#pragma interrupt (Excep_ICU_IRQ1(vect=65))
void Excep_ICU_IRQ1(void);

// ICU IRQ2
#pragma interrupt (Excep_ICU_IRQ2(vect=66))
void Excep_ICU_IRQ2(void);

// ICU IRQ3
#pragma interrupt (Excep_ICU_IRQ3(vect=67))
void Excep_ICU_IRQ3(void);

// ICU IRQ4
#pragma interrupt (Excep_ICU_IRQ4(vect=68))
void Excep_ICU_IRQ4(void);

// ICU IRQ5
#pragma interrupt (Excep_ICU_IRQ5(vect=69))
void Excep_ICU_IRQ5(void);

// ICU IRQ6
#pragma interrupt (Excep_ICU_IRQ6(vect=70))
void Excep_ICU_IRQ6(void);

// ICU IRQ7
#pragma interrupt (Excep_ICU_IRQ7(vect=71))
void Excep_ICU_IRQ7(void);

// LVD LVD1　→CMPA1と併用(vect=88で同じ)のため、こちらはコメントアウト
//#pragma interrupt (Excep_LVD_LVD1(vect=88))
//void Excep_LVD_LVD1(void);

// CMPA CMPA1
#pragma interrupt (Excep_CMPA_CMPA1(vect=88))
void Excep_CMPA_CMPA1(void);

// LVD LVD2
#pragma interrupt (Excep_LVD_LVD2(vect=89))
void Excep_LVD_LVD2(void);

// CMPA CMPA2
//#pragma interrupt (Excep_CMPA_CMPA2(vect=89))
//void Excep_CMPA_CMPA2(void);

// RTC ALM
#pragma interrupt (Excep_RTC_ALM(vect=92))
void Excep_RTC_ALM(void);

// RTC PRD
#pragma interrupt (Excep_RTC_PRD(vect=93))
void Excep_RTC_PRD(void);

// S12AD S12ADI0
#pragma interrupt (Excep_S12AD_S12ADI0(enable,vect=102))
void Excep_S12AD_S12ADI0(void);

// S12AD GBADI
#pragma interrupt (Excep_S12AD_GBADI(vect=103))
void Excep_S12AD_GBADI(void);

// ELC ELSR18I
#pragma interrupt (Excep_ELC_ELSR18I(vect=106))
void Excep_ELC_ELSR18I(void);

// MTU0 TGIA0
#pragma interrupt (Excep_MTU0_TGIA0(vect=114))
void Excep_MTU0_TGIA0(void);

// MTU0 TGIB0
#pragma interrupt (Excep_MTU0_TGIB0(vect=115))
void Excep_MTU0_TGIB0(void);

// MTU0 TGIC0
#pragma interrupt (Excep_MTU0_TGIC0(vect=116))
void Excep_MTU0_TGIC0(void);

// MTU0 TGID0
#pragma interrupt (Excep_MTU0_TGID0(vect=117))
void Excep_MTU0_TGID0(void);

// MTU0 TCIV0
#pragma interrupt (Excep_MTU0_TCIV0(vect=118))
void Excep_MTU0_TCIV0(void);

// MTU0 TGIE0
#pragma interrupt (Excep_MTU0_TGIE0(vect=119))
void Excep_MTU0_TGIE0(void);

// MTU0 TGIF0
#pragma interrupt (Excep_MTU0_TGIF0(vect=120))
void Excep_MTU0_TGIF0(void);

// MTU1 TGIA1
#pragma interrupt (Excep_MTU1_TGIA1(vect=121))
void Excep_MTU1_TGIA1(void);

// MTU1 TGIB1
#pragma interrupt (Excep_MTU1_TGIB1(vect=122))
void Excep_MTU1_TGIB1(void);

// MTU1 TCIV1
#pragma interrupt (Excep_MTU1_TCIV1(vect=123))
void Excep_MTU1_TCIV1(void);

// MTU1 TCIU1
#pragma interrupt (Excep_MTU1_TCIU1(vect=124))
void Excep_MTU1_TCIU1(void);

// MTU2 TGIA2
#pragma interrupt (Excep_MTU2_TGIA2(vect=125))
void Excep_MTU2_TGIA2(void);

// MTU2 TGIB2
#pragma interrupt (Excep_MTU2_TGIB2(enable,vect=126))
void Excep_MTU2_TGIB2(void);

// MTU2 TCIV2
#pragma interrupt (Excep_MTU2_TCIV2(enable,vect=127))
void Excep_MTU2_TCIV2(void);

// MTU2 TCIU2
#pragma interrupt (Excep_MTU2_TCIU2(vect=128))
void Excep_MTU2_TCIU2(void);

// MTU3 TGIA3
#pragma interrupt (Excep_MTU3_TGIA3(vect=129))
void Excep_MTU3_TGIA3(void);

// MTU3 TGIB3
#pragma interrupt (Excep_MTU3_TGIB3(vect=130))
void Excep_MTU3_TGIB3(void);

// MTU3 TGIC3
#pragma interrupt (Excep_MTU3_TGIC3(vect=131))
void Excep_MTU3_TGIC3(void);

// MTU3 TGID3
#pragma interrupt (Excep_MTU3_TGID3(vect=132))
void Excep_MTU3_TGID3(void);

// MTU3 TCIV3
#pragma interrupt (Excep_MTU3_TCIV3(vect=133))
void Excep_MTU3_TCIV3(void);

// MTU4 TGIA4
#pragma interrupt (Excep_MTU4_TGIA4(vect=134))
void Excep_MTU4_TGIA4(void);

// MTU4 TGIB4
#pragma interrupt (Excep_MTU4_TGIB4(vect=135))
void Excep_MTU4_TGIB4(void);

// MTU4 TGIC4
#pragma interrupt (Excep_MTU4_TGIC4(vect=136))
void Excep_MTU4_TGIC4(void);

// MTU4 TGID4
#pragma interrupt (Excep_MTU4_TGID4(vect=137))
void Excep_MTU4_TGID4(void);

// MTU4 TCIV4
#pragma interrupt (Excep_MTU4_TCIV4(vect=138))
void Excep_MTU4_TCIV4(void);

// MTU5 TGIU5
#pragma interrupt (Excep_MTU5_TGIU5(vect=139))
void Excep_MTU5_TGIU5(void);

// MTU5 TGIV5
#pragma interrupt (Excep_MTU5_TGIV5(vect=140))
void Excep_MTU5_TGIV5(void);

// MTU5 TGIW5
#pragma interrupt (Excep_MTU5_TGIW5(vect=141))
void Excep_MTU5_TGIW5(void);

// POE OEI1
#pragma interrupt (Excep_POE_OEI1(vect=170))
void Excep_POE_OEI1(void);

// POE OEI2
#pragma interrupt (Excep_POE_OEI2(vect=171))
void Excep_POE_OEI2(void);

// TMR0 CMIA0
#pragma interrupt (Excep_TMR0_CMIA0(vect=174))
void Excep_TMR0_CMIA0(void);

// TMR0 CMIB0
#pragma interrupt (Excep_TMR0_CMIB0(vect=175))
void Excep_TMR0_CMIB0(void);

// TMR0 OVI0
#pragma interrupt (Excep_TMR0_OVI0(vect=176))
void Excep_TMR0_OVI0(void);

// TMR1 CMIA1
#pragma interrupt (Excep_TMR1_CMIA1(vect=177))
void Excep_TMR1_CMIA1(void);

// TMR1 CMIB1
#pragma interrupt (Excep_TMR1_CMIB1(vect=178))
void Excep_TMR1_CMIB1(void);

// TMR1 OVI1
#pragma interrupt (Excep_TMR1_OVI1(vect=179))
void Excep_TMR1_OVI1(void);

// TMR2 CMIA2
#pragma interrupt (Excep_TMR2_CMIA2(vect=180))
void Excep_TMR2_CMIA2(void);

// TMR2 CMIB2
#pragma interrupt (Excep_TMR2_CMIB2(vect=181))
void Excep_TMR2_CMIB2(void);

// TMR2 OVI2
#pragma interrupt (Excep_TMR2_OVI2(vect=182))
void Excep_TMR2_OVI2(void);

// TMR3 CMIA3
#pragma interrupt (Excep_TMR3_CMIA3(vect=183))
void Excep_TMR3_CMIA3(void);

// TMR3 CMIB3
#pragma interrupt (Excep_TMR3_CMIB3(vect=184))
void Excep_TMR3_CMIB3(void);

// TMR3 OVI3
#pragma interrupt (Excep_TMR3_OVI3(vect=185))
void Excep_TMR3_OVI3(void);

// DMAC DMAC0I
#pragma interrupt (Excep_DMAC_DMAC0I(vect=198))
void Excep_DMAC_DMAC0I(void);

// DMAC DMAC1I
#pragma interrupt (Excep_DMAC_DMAC1I(vect=199))
void Excep_DMAC_DMAC1I(void);

// DMAC DMAC2I
#pragma interrupt (Excep_DMAC_DMAC2I(vect=200))
void Excep_DMAC_DMAC2I(void);

// DMAC DMAC3I
#pragma interrupt (Excep_DMAC_DMAC3I(vect=201))
void Excep_DMAC_DMAC3I(void);

// SCI1 ERI1
#pragma interrupt (Excep_SCI1_ERI1(vect=218))
void Excep_SCI1_ERI1(void);

// SCI1 RXI1
#pragma interrupt (Excep_SCI1_RXI1(enable,vect=219))
void Excep_SCI1_RXI1(void);

// SCI1 TXI1
#pragma interrupt (Excep_SCI1_TXI1(enable,vect=220))
void Excep_SCI1_TXI1(void);

// SCI1 TEI1
#pragma interrupt (Excep_SCI1_TEI1(vect=221))
void Excep_SCI1_TEI1(void);

// SCI5 ERI5
#pragma interrupt (Excep_SCI5_ERI5(vect=222))
void Excep_SCI5_ERI5(void);

// SCI5 RXI5
#pragma interrupt (Excep_SCI5_RXI5(vect=223))
void Excep_SCI5_RXI5(void);

// SCI5 TXI5
#pragma interrupt (Excep_SCI5_TXI5(vect=224))
void Excep_SCI5_TXI5(void);

// SCI5 TEI5
#pragma interrupt (Excep_SCI5_TEI5(vect=225))
void Excep_SCI5_TEI5(void);

// SCI6 ERI6
#pragma interrupt (Excep_SCI6_ERI6(vect=226))
void Excep_SCI6_ERI6(void);

// SCI6 RXI6
#pragma interrupt (Excep_SCI6_RXI6(vect=227))
void Excep_SCI6_RXI6(void);

// SCI6 TXI6
#pragma interrupt (Excep_SCI6_TXI6(vect=228))
void Excep_SCI6_TXI6(void);

// SCI6 TEI6
#pragma interrupt (Excep_SCI6_TEI6(vect=229))
void Excep_SCI6_TEI6(void);

// SCI9 ERI9
#pragma interrupt (Excep_SCI9_ERI9(vect=234))
void Excep_SCI9_ERI9(void);

// SCI9 RXI9
#pragma interrupt (Excep_SCI9_RXI9(vect=235))
void Excep_SCI9_RXI9(void);

// SCI9 TXI9
#pragma interrupt (Excep_SCI9_TXI9(vect=236))
void Excep_SCI9_TXI9(void);

// SCI9 TEI9
#pragma interrupt (Excep_SCI9_TEI9(vect=237))
void Excep_SCI9_TEI9(void);

// SCI12 ERI12
#pragma interrupt (Excep_SCI12_ERI12(vect=238))
void Excep_SCI12_ERI12(void);

// SCI12 RXI12
#pragma interrupt (Excep_SCI12_RXI12(enable,vect=239))
void Excep_SCI12_RXI12(void);

// SCI12 TXI12
#pragma interrupt (Excep_SCI12_TXI12(enable,vect=240))
void Excep_SCI12_TXI12(void);

// SCI12 TEI12
#pragma interrupt (Excep_SCI12_TEI12(vect=241))
void Excep_SCI12_TEI12(void);

// SCI12 SCIX0
#pragma interrupt (Excep_SCI12_SCIX0(vect=242))
void Excep_SCI12_SCIX0(void);

// SCI12 SCIX1
#pragma interrupt (Excep_SCI12_SCIX1(vect=243))
void Excep_SCI12_SCIX1(void);

// SCI12 SCIX2
#pragma interrupt (Excep_SCI12_SCIX2(vect=244))
void Excep_SCI12_SCIX2(void);

// SCI12 SCIX3
#pragma interrupt (Excep_SCI12_SCIX3(vect=245))
void Excep_SCI12_SCIX3(void);

// RIIC0 EEI0
#pragma interrupt (Excep_RIIC0_EEI0(vect=246))
void Excep_RIIC0_EEI0(void);

// RIIC0 RXI0
#pragma interrupt (Excep_RIIC0_RXI0(vect=247))
void Excep_RIIC0_RXI0(void);

// RIIC0 TXI0
#pragma interrupt (Excep_RIIC0_TXI0(vect=248))
void Excep_RIIC0_TXI0(void);

// RIIC0 TEI0
#pragma interrupt (Excep_RIIC0_TEI0(vect=249))
void Excep_RIIC0_TEI0(void);


//;<<VECTOR DATA START (POWER ON RESET)>>
//;Power On Reset PC
extern void PowerON_Reset_PC(void);                                                                                                                
//;<<VECTOR DATA END (POWER ON RESET)>>

