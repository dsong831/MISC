/**********************************************************************
* @file		A31G21x_scu.h
* @brief	Contains all macro definitions and function prototypes
* 			support for SCU firmware library on A31G216X
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

#ifndef _SCU_H_
#define _SCU_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"



/* Private macros ------------------------------------------------------------- */
/* External sub crystal oscillator control */
#define EXSOSC_STOP               0x0				
#define EXSOSC_EN                    0x8				
#define EXSOSC_EN_DIV2            0x9		
#define EXSOSC_EN_DIV4            0xa

/* External sub crystal oscillator stable timeout */
#define SOSC_STARTUP_TIMEOUT 0xFFFFFFFFUL

	/* low speed oscillator control */
#define LSIOSC_STOP               0x0				
#define LSIOSC_EN                    0x8				
#define LSIOSC_EN_DIV2            0x9		
#define LSIOSC_EN_DIV4            0xa
	
	/* Internal high speed oscillator control */
#define HSIOSC_STOP               0x0				
#define HSIOSC_EN                    0x8				
#define HSIOSC_EN_DIV2            0x9		
#define HSIOSC_EN_DIV4            0xa					
#define HSIOSC_EN_DIV8            0xb		
#define HSIOSC_EN_DIV16          0xc
#define HSIOSC_EN_DIV32          0xd

/* External crystal oscillator control */
#define EXOSC_STOP               0x0				
#define EXOSC_EN                    0x8				
#define EXOSC_EN_DIV2            0x9		
#define EXOSC_EN_DIV4            0xa

/* External crystal oscillator stable timeout */
#define MOSC_STARTUP_TIMEOUT 0xFFFFFFFFUL

/* PLL stable timeout */
#define PLL_STARTUP_TIMEOUT 0xFFFFFFFFUL

/* PLLCON[BYPASS] */
#define PLLCON_BYPASS_FIN 0
#define PLLCON_BYPASS_PLL 1

// PLL freq = (FIN or FIN/2) * M / N
/* PLLCON[PREDIV] */
#define PLLCON_FIN_DIV_1 0
#define PLLCON_FIN_DIV_2 1

/* PLLCON[FBCTRL] */
#define PLLCON_M_6 0
#define PLLCON_M_8 1
#define PLLCON_M_10 2
#define PLLCON_M_12 3
#define PLLCON_M_16 4
#define PLLCON_M_18 5
#define PLLCON_M_20 6
#define PLLCON_M_26 7
#define PLLCON_M_32 8
#define PLLCON_M_36 9
#define PLLCON_M_40 10
#define PLLCON_M_64 11

/* PLLCON[POSTDIV] */
#define PLLCON_N_1 0
#define PLLCON_N_2 1
#define PLLCON_N_3 2
#define PLLCON_N_4 3
#define PLLCON_N_6 4
#define PLLCON_N_8 5
#define PLLCON_N_16 7


/* system clock control setting */
#define SC_LSIOSC	0x0
#define SC_SOSC	   0x1
#define SC_HSIOSC          0x2
#define SC_HSIOSCPLL     0x3 
#define SC_EXOSC          0x6
#define SC_EXOSCPLL     0x7

enum {
	SYSTICK_TYPE=0,
	TIMER10_TYPE,
	TIMER20_TYPE,
	TIMER30_TYPE,	
	WDT_TYPE,
	WT_TYPE,
	PD0_TYPE,
	PD1_TYPE,
	LED_TYPE,
	LCD_TYPE,
	USB_TYPE
};

enum {
	CLKSRC_LSI=0,
	CLKSRC_SOSC=3,	
	CLKSRC_MCLK=4,
	CLKSRC_HSI=5,	
	CLKSRC_MOSC=6,
	CLKSRC_PLL=7	
};

enum {
	LVD_LEVEL_1_65 = 0,
	LVD_LEVEL_2_5,
	LVD_LEVEL_3_6,
	LVD_LEVEL_4_2
};

//----------------- SCU Reset Source definition -----------------
#define RST_PINRST     (1UL<<6)
#define RST_CPURST     (1UL<<5)
#define RST_SWRST     (1UL<<4)
#define RST_WDTRST     (1UL<<3)
#define RST_MCKFRST        (1UL<<2)
#define RST_MOFRST        (1UL<<1)
#define RST_LVDRST        (1UL<<0)


#define WAKEUP_GPIOF	 (1UL<<13)
#define WAKEUP_GPIOE	 (1UL<<12)
#define WAKEUP_GPIOD     (1UL<<11)
#define WAKEUP_GPIOC     (1UL<<10)
#define WAKEUP_GPIOB     (1UL<<9)
#define WAKEUP_GPIOA     (1UL<<8)
#define WAKEUP_FRT        (1UL<<2)
#define WAKEUP_WDT        (1UL<<1)
#define WAKEUP_LVD        (1UL<<0)


//----------------- T1N SCU Clock Constant Definition -----------------
#define T1NCLK_MCCR1                   0
#define T1NCLK_PCLK                   1

//----------------- T20 SCU Clock Constant Definition -----------------
#define T20CLK_MCCR2                  0
#define T20CLK_PCLK                   1

//----------------- T30 SCU Clock Constant Definition -----------------
#define T30CLK_MCCR2                   0
#define T30CLK_PCLK                   1

//----------------- LED SCU Clock Constant Definition -----------------
#define LEDCLK_MCCR5                   0
#define LEDCLK_PCLK                   1

//----------------- USB SCU Clock Constant Definition -----------------
#define USBCLK_MCCR6                   0
#define USBCLK_PCLK                   1

//---------------- LCD Driver Clock Constant Definition ---------------
#define LCDCLK_MCCR5                   0
#define LCDCLK_SOSC                  1
#define LCDCLK_WDTRC                  2

//--------------- Watch Timer Clock Constant Definition ---------------
#define WTCLK_MCCR3                   0
#define WTCLK_SOSC                   1
#define WTCLK_WDTRC                   2

//------------- Watch-Dog Timer Clock Constant Definition -------------
#define WDTCLK_WDTRC                  0
#define WDTCLK_MCCR3                   1


/* Public Functions ----------------------------------------------------------- */
void SCU_SetSOSC(uint32_t esosccon);
Status SCU_WaitForSOSCStartUp(void);

void SCU_SetLSIOSC(uint32_t ringosc); // in ring osc set 
void SCU_SetHSIOSC(uint32_t hsiosc); // hsi osc set

void SCU_SetMOSC(uint32_t eosccon);	 // exosc set
Status SCU_WaitForMOSCStartUp(void); // check for MOSC stable 


void SCU_ChangeSysClk(uint32_t sysclkcon); // set system clock ==> final MCLK
void SCU_SetCOR(uint8_t divval, FunctionalState endis);
Status SCU_SetPLLandWaitForPLLStartUp(FunctionalState pllsetstate, uint8_t selbypass, uint8_t selfin,  uint8_t PREDIV, uint8_t POSTDIV1, uint8_t POSTDIV2, uint8_t OUTDIV);

void SCU_SetTimer1nClk(uint32_t t1nclk);
void SCU_SetTimer20Clk(uint32_t t20clk);
void SCU_SetTimer30Clk(uint32_t t30clk);
void SCU_SetLedClk(uint32_t ledclk);
void SCU_SetUSBClk(uint32_t usbclk);
void SCU_SetLcdClk(uint32_t lcdclk);
void SCU_SetWtClk(uint32_t wtclk);
void SCU_SetWdtClk(uint32_t wdtclk);

void SCU_SetMCCRx(uint8_t mccrnum, uint8_t type, uint8_t clksrc, uint8_t clkdiv);

uint32_t SCULV_GetLviFlag(void);
void SCULV_ClrLviFlag(void);
	
void SCU_WakeUpSRCCmd(uint32_t WakeUpSrc, FunctionalState NewState);
uint32_t SCU_GetWakeUpSRCStatus(void);
void SCU_ClearWakeUpSRC(uint32_t WakeUpStatus);

void SCU_LVDCmd(FunctionalState NewState);
	
uint32_t SCU_GetNMIStatus(void);
void SCU_SetNMI(uint32_t nmisrc);

void SCU_SetResetSrc(uint32_t RstSrc, FunctionalState NewState);
uint32_t SCU_GetResetSrc(void);
void SCU_ClearResetStatus(uint32_t rststatus);

#ifdef __cplusplus
}
#endif

#endif /* end _SCU_H_ */

/* --------------------------------- End Of File ------------------------------ */
