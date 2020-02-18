//==============================================================================
//      version 0.0     2015-02-03
//==============================================================================
#ifndef _MC97FG316_
#define _MC97FG316_
//==============================================================================
//      PORT Control Register
//==============================================================================
sfr         P0          = 0x80;                                     // 
  sbit      P00 	    = 0x80;
  sbit      P01	  	    = 0x81;
  sbit      P02	  	    = 0x82;
  sbit      P03 	    = 0x83;
  sbit      P04	  	    = 0x84;
  sbit      P05	  	    = 0x85;
  sbit      P06	  	    = 0x86;
  sbit      P07	  	    = 0x87;

sfr         P0IO        = 0x89;                                     // 
#define     P0PU        *(volatile unsigned char xdata *) 0x2F00    // 
#define     P0OD        *(volatile unsigned char xdata *) 0x2F04    // 
#define     P0DB        *(volatile unsigned char xdata *) 0x2F08    // 

sfr         P1          = 0x88;                                     // 
  sbit      P10         = 0x88;
  sbit      P11         = 0x89;
  sbit      P12         = 0x8A;
  sbit      P13         = 0x8B;
  sbit      P14         = 0x8C;
  sbit      P15         = 0x8D;
  sbit      P16         = 0x8E;
//sbit      P17         = 0x8F;

sfr         P1IO        = 0x91;                                     // 
#define     P1PU        *(volatile unsigned char xdata *) 0x2F01    // 
#define     P1OD        *(volatile unsigned char xdata *) 0x2F05    // 
#define     P1DB        *(volatile unsigned char xdata *) 0x2F09    // 

sfr         P2          = 0x90;                                     // 
  sbit      P20         = 0x90;
  sbit      P21         = 0x91;
  sbit      P22         = 0x92;
  sbit      P23         = 0x93;
  sbit      P24         = 0x94;
  sbit      P25         = 0x95;
  sbit      P26         = 0x96;
//sbit      P27         = 0x97;

sfr         P2IO        = 0x99;                                     // 
#define     P2PU        *(volatile unsigned char xdata *) 0x2F02    // 
#define     P2OD        *(volatile unsigned char xdata *) 0x2F06    // 
#define     P2DB        *(volatile unsigned char xdata *) 0x2F0A    // 

sfr         P3          = 0x98;                                     // 
  sbit      P30         = 0x98;
  sbit      P31         = 0x99;
  sbit      P32         = 0x9A;
  sbit      P33         = 0x9B;
  sbit      P34         = 0x9C;
  sbit      P35         = 0x9D;
  sbit      P36         = 0x9E;
  sbit      P37         = 0x9F;

sfr         P3IO        = 0xA1;                                     // 
#define     P3PU        *(volatile unsigned char xdata *) 0x2F03    // 
#define     P3OD        *(volatile unsigned char xdata *) 0x2F07    // 
#define     P3DB        *(volatile unsigned char xdata *) 0x2F0B    // 
//==============================================================================
//      USART
//==============================================================================
sfr         UCTRL1      = 0xE2;                                     // 
sfr         UCTRL2      = 0xE3;                                     // 
sfr         UCTRL3      = 0xE4;                                     // 
sfr         USTAT       = 0xE5;                                     // 
sfr         UBAUD       = 0xE6;                                     // 
sfr         UDATA       = 0xE7;                                     // 

sfr         UCTRL11     = 0xFA;                                     // 
sfr         UCTRL12     = 0xFB;                                     // 
sfr         UCTRL13     = 0xFC;                                     // 
sfr         USTAT1      = 0xFD;                                     // 
sfr         UBAUD1      = 0xFE;                                     // 
sfr         UDATA1      = 0xFF;                                     // 
//==============================================================================
//      SYSCON 
//==============================================================================
sfr         BODR        = 0x86;                                     // 
sfr         PCON        = 0x87;                                     // 
sfr         SCCR        = 0x8A;                                     // 
sfr         BCCR        = 0x8B;                                     // 
sfr         BITR        = 0x8C;                                     // 
sfr         PLLCR       = 0x94;                                     // 
//==============================================================================
//      CPU
//==============================================================================
sfr         SP          = 0x81;                                     // 

sfr16       DPTR0       = 0x82;                                     // 
sfr         DPL         = 0x82;                                     // 
sfr         DPH         = 0x83;                                     // 
sfr16       DPTR1       = 0x84;                                     // 
sfr         DPL1        = 0x84;                                     // 
sfr         DPH1        = 0x85;                                     // 
sfr         XBANK       = 0xA0;                                     // 

//==============================================================================
//      Watchdog Timer 
//==============================================================================
sfr         WDTCR       = 0x8D;                                     // 
sfr         WDTIDR      = 0x8E;                                     // 
#define     WDTC        *(volatile unsigned char xdata *) 0x2F14    // 
#define     WDTSR       *(volatile unsigned char xdata *) 0x2F15    // 
#define     WDTCNTH     *(volatile unsigned char xdata *) 0x2F16    // 
#define     WDTCNTL     *(volatile unsigned char xdata *) 0x2F17    // 
#define     WDTTEST0    *(volatile unsigned char xdata *) 0x2F1E    // 
#define     WDTTEST1    *(volatile unsigned char xdata *) 0x2F1F    // 

//==============================================================================
//      Watch Timer
//==============================================================================
sfr         WTMR        = 0x95;                                     // 
sfr         WTR         = 0x96;                                     // 
sfr         WTCR        = 0x96;                                     // 
//==============================================================================
//      BUZZER 
//==============================================================================
sfr         BUZDR       = 0x8F;                                     // 
sfr         BUZCR       = 0x97;                                     // 
//==============================================================================
//      Interrupt Control
//==============================================================================
sfr         EIENAB      = 0xA3;                                     // 
sfr         EIFLAG      = 0xA4;                                     // 
sfr         EIEDGE      = 0xA5;                                     // 
sfr         EIPOLA      = 0xA6;                                     // 
sfr	        EIBEDGE	    = 0xA7;                                     // 
sfr	        EIBOTH	    = 0xA7;                                     // 

sfr         IE          = 0xA8;                                     // 
    sbit    EA          = 0xAF;                                     // 
sfr         IE1         = 0xA9;                                     // 
sfr         IE2         = 0xAA;                                     // 
sfr         IE3         = 0xAB;                                     // 

sfr         IP          = 0x92;                                     // 
sfr         IPH         = 0x93;                                     // 
sfr         IP1         = 0x9A;                                     // 
sfr         IP1H        = 0x9B;                                     // 
sfr         IP2         = 0x9C;                                     // 
sfr         IP2H        = 0x9D;                                     // 
sfr         IP3         = 0x9E;                                     // 
sfr         IP3H        = 0x9F;                                     // 

sfr         PCI0        = 0xAE;                                     // 
//==============================================================================
//      TIMER  
//==============================================================================
sfr         TMISR       = 0xAF;                                     // 

sfr         T0CR        = 0xB2;                                     // 
sfr         T0          = 0xB3;                                     // 
sfr         T0DR        = 0xB3;                                     // 
sfr         CDR0        = 0xB3;                                     // 

sfr         T1CR        = 0xB4;                                     // 
sfr         T1DR        = 0xB5;                                     // 
sfr16       T1PPR       = 0xB5;                                     // 
sfr         T1PPRL      = 0xB5;                                     // 
sfr         T1PPRH      = 0xB6;                                     // 
sfr         T1PCR       = 0xB7;                                     // 
sfr         T1PCR2      = 0xBD;                                     // 
sfr         T1PCR3      = 0xBE;                                     // 

sfr         T1          = 0xB0;                                     // 
sfr         CDR1        = 0xB0;                                     // 
sfr16       T1ADR       = 0xB0;                                     // 
sfr         T1ADRL      = 0xB0;                                     // 
sfr         T1ADHL      = 0xB1;                                     // 
sfr16       T1BDR       = 0xB9;                                     // 
sfr         T1BDRL      = 0xB9;                                     // 
sfr         T1BDRH      = 0xBA;                                     // 
sfr16       T1CDR       = 0xBB;                                     // 
sfr         T1CDRL      = 0xBB;                                     // 
sfr         T1CDRH      = 0xBC;                                     // 

sfr         T1DLYA      = 0xBF;                                     // 
sfr         T1DLYB      = 0xC2;                                     // 
sfr         T1DLYC      = 0xC3;                                     // 
sfr         T1ISR       = 0xC4;                                     // 
sfr         T1IMSK      = 0xC5;                                     // 

sfr         T2CR        = 0xC6;                                     //  
sfr         T2          = 0xC7;                                     // 
sfr         T2DR        = 0xC7;                                     // 
sfr         CDR2        = 0xC7;                                     // 

sfr         T3CR        = 0xCA;                                     // 
sfr         T3DR        = 0xCB;                                     // 
sfr         PWM3PR      = 0xCB;                                     // 
sfr         T3          = 0xCC;                                     // 
sfr         PWM3DR      = 0xCC;                                     // 
sfr         CDR3        = 0xCC;                                     // 
sfr         PWM3HR      = 0xCD;                                     // 

sfr         T4CR        = 0xCE;                                     // 
sfr         T4L         = 0xCF;                                     // 
sfr         T4H         = 0xD5;                                     // 
sfr         T4LDR       = 0xCF;                                     // 
sfr         T4HDR       = 0xD5;                                     // 
//==============================================================================
//      SPI
//==============================================================================
sfr         SPICR       = 0xD2;                                     // 
sfr         SPIDR       = 0xD3;                                     // 
sfr         SPISR       = 0xD4;                                     // 
//==============================================================================
//      I2C
//==============================================================================
sfr         I2CMR       = 0xDA;                                     // 
sfr         I2CCR       = 0xDA;                                     // 
sfr         I2CSR       = 0xDB;                                     // 
sfr         I2CSCLLR    = 0xDC;                                     // 
sfr         I2CSCLHR    = 0xDD;                                     // 
sfr         I2CSDAHR    = 0xDE;                                     // 
sfr         I2CDR       = 0xDF;                                     // 
sfr         I2CSAR1     = 0xD6;                                     // 
sfr         I2CSAR      = 0xD7;                                     // 
//==============================================================================
//      Analog Comparator
//==============================================================================
sfr         ACCSR       = 0xF9;                                     // 
//==============================================================================
//      ???
//==============================================================================
sfr         TCRCDATAL   = 0xD8;                                     // 
sfr         TCRCDATAH   = 0xD9;                                     // 
sfr         TCRCC       = 0xE1;                                     // 
sfr         UCRCL       = 0xE8;                                     // 
sfr         UCRCH       = 0xE9;                                     // 
sfr         UCRCDIN     = 0xF1;                                     // 
sfr         UCRCC       = 0xF8;                                     // 
//==============================================================================
//      A/D Converter
//==============================================================================
sfr         ADCM        = 0xC0;                                     // 
sfr         ADCM2       = 0xC1;                                     // 
sfr16       ADCR        = 0xC8;                                     // 
sfr         ADCRL       = 0xB8;                                     // 
sfr         ADCRH       = 0xC9;                                     // 
//==============================================================================
//      PSR
//==============================================================================
#define     PSR0		*(volatile unsigned char xdata *) 0x2F10    // 
#define     PSR1		*(volatile unsigned char xdata *) 0x2F11    // 
#define     PSR2		*(volatile unsigned char xdata *) 0x2F12    // 
//==============================================================================
//      Flash and EEPROM Memory REGISTER 
//==============================================================================
sfr         FEMR        = 0xEA;                                     // 
sfr         FECR        = 0xEB;                                     // 
sfr         FESR        = 0xEC;                                     // 
sfr         FETCR       = 0xED;                                     // 
sfr         FEARL       = 0xF2;                                     // 
sfr         FEARM       = 0xF3;                                     // 
sfr         FEARH       = 0xF4;                                     // 
sfr         FEDR        = 0xF5;                                     // 
sfr         FETR        = 0xF6;                                     // 
//==============================================================================
//      TEST MODE REGISTER 
//==============================================================================
#define     TESTTCRC0	*(volatile unsigned char xdata *) 0x2F0E    // 
#define     TESTTCRC1	*(volatile unsigned char xdata *) 0x2F0F    // 

#define     FUSE_CONF	*(volatile unsigned char xdata *) 0x2F50    // 
#define     FUSE_TEMP	*(volatile unsigned char xdata *) 0x2F51    // 
#define     FUSE_FREQ	*(volatile unsigned char xdata *) 0x2F52    // 
#define     FUSE_RING	*(volatile unsigned char xdata *) 0x2F53    // 
#define     FUSE_BGR	*(volatile unsigned char xdata *) 0x2F54    // 
#define     FUSE_FLAS	*(volatile unsigned char xdata *) 0x2F55    // 
#define     FUSE_XTAKL	*(volatile unsigned char xdata *) 0x2F56    // 
#define     TEST_REGB	*(volatile unsigned char xdata *) 0x2F5E    // 
#define     TEST_REGA	*(volatile unsigned char xdata *) 0x2F5F    // 
//==============================================================================

// Interrupt vectors of 97FG316
#define EINT0VECT	0		// IE.0
#define EINT1VECT	1		// IE.1
#define EINT2VECT	2		// IE.2
#define EINT3VECT	3		// IE.3
#define EINT4VECT	4		// IE.4
#define EINT5VECT 	5		// IE.5

#define RXDVECT		6		// IE1.0
#define TXDVECT		7		// IE1.1
#define SPIVECT		8		// IE1.2
#define I2CVECT		9		// IE1.3
#define RXD1VECT	10		// IE1.4
#define TXD1VECT	11		// IE1.5

#define T0VECT		12		// IE2.0
#define T1VECT		13		// IE2.1
#define T2VECT		14		// IE2.2
#define T3VECT		15		// IE2.3
#define T4VECT		16		// IE2.4
#define EINT6VECT	17		// IE2.5	

#define ADCVECT		18		// IE3.0	
#define CMPVECT		19		// IE3.1
#define WTVECT		20		// IE3.2
#define WDTVECT		21		// IE3.3
#define BITVECT		22		// IE3.4
#define PCIVECT		22		// IE3.4


// Interrupt en/dis control
#define eint0_en()		reg_set(IE, (1<<BIT0));
#define eint1_en()		reg_set(IE, (1<<BIT1));
#define eint2_en()		reg_set(IE, (1<<BIT2));
#define eint3_en()		reg_set(IE, (1<<BIT3));
#define eint4_en()		reg_set(IE, (1<<BIT4));
#define eint5_en()		reg_set(IE, (1<<BIT5));

#define rxdint_en()		reg_set(IE1, (1<<BIT0));
#define txdint_en()		reg_set(IE1, (1<<BIT1));
#define spiint_en()		reg_set(IE1, (1<<BIT2));
#define i2cint_en()		reg_set(IE1, (1<<BIT3));
#define rxd1int_en()	reg_set(IE1, (1<<BIT4));
#define txd1int_en()	reg_set(IE1, (1<<BIT5));

#define t0int_en()		reg_set(IE2, (1<<BIT0));
#define t1int_en()		reg_set(IE2, (1<<BIT1));
#define t2int_en()		reg_set(IE2, (1<<BIT2));
#define t3int_en()		reg_set(IE2, (1<<BIT3));
#define t4int_en()		reg_set(IE2, (1<<BIT4));
#define eint6_en()		reg_set(IE2, (1<<BIT5));

#define adcint_en()		reg_set(IE3, (1<<BIT0));
#define cmpint_en()		reg_set(IE3, (1<<BIT1));
#define wtint_en()		reg_set(IE3, (1<<BIT2));
#define wdtint_en()		reg_set(IE3, (1<<BIT3));
#define bitint_en()		reg_set(IE3, (1<<BIT4));
#define pciint_en()		reg_set(IE3, (1<<BIT5));

#define eint0_dis()		reg_clr(IE, (1<<BIT0));
#define eint1_dis()		reg_clr(IE, (1<<BIT1));
#define eint2_dis()		reg_clr(IE, (1<<BIT2));
#define eint3_dis()		reg_clr(IE, (1<<BIT3));
#define eint4_dis()		reg_clr(IE, (1<<BIT4));
#define eint5_dis()		reg_clr(IE, (1<<BIT5));

#define rxdint_dis()	reg_clr(IE1, (1<<BIT0));
#define txdint_dis()	reg_clr(IE1, (1<<BIT1));
#define spiint_dis()	reg_clr(IE1, (1<<BIT2));
#define i2cint_dis()	reg_clr(IE1, (1<<BIT3));
#define rxd1int_dis()	reg_clr(IE1, (1<<BIT4));
#define txd1int_dis()	reg_clr(IE1, (1<<BIT5));

#define t0int_dis()		reg_clr(IE2, (1<<BIT0));
#define t1int_dis()		reg_clr(IE2, (1<<BIT1));
#define t2int_dis()		reg_clr(IE2, (1<<BIT2));
#define t3int_dis()		reg_clr(IE2, (1<<BIT3));
#define t4int_dis()		reg_clr(IE2, (1<<BIT4));
#define eint6_dis()		reg_clr(IE2, (1<<BIT5));

#define adcint_dis()	reg_clr(IE3, (1<<BIT0));
#define cmpint_dis()	reg_clr(IE3, (1<<BIT1));
#define wtint_dis()		reg_clr(IE3, (1<<BIT2));
#define wdtint_dis()	reg_clr(IE3, (1<<BIT3));
#define bitint_dis()	reg_clr(IE3, (1<<BIT4));
#define pciint_dis()	reg_clr(IE3, (1<<BIT5));

#endif	//_MC97FG316_ 
