/**********************************************************************//**
* @file				A31T21x_hal_scu.h
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			26. JUNE. 2020
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements, 
*modifications, and improvements to ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and 
*ABOV assumes no liability for application assistance or the design of purchasers�� products. No license, 
*express or implied, to any intellectual property rights is granted by ABOV herein. 
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications. 
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners. 
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
**********************************************************************/


#ifndef A31T21X_HAL_SCU_H
#define A31T21X_HAL_SCU_H

/* Includes ------------------------------------------------------------ */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
/* Low speed oscillator control */
#define LSIOSC_STOP					0x0
#define LSIOSC_EN							0x8
#define LSIOSC_EN_DIV2			0x9
#define LSIOSC_EN_DIV4			0xA
/* Internal high speed oscillator control */
#define HSIOSC_STOP				0x0
#define HSIOSC_EN							0x8
#define HSIOSC_EN_DIV2			0x9
#define HSIOSC_EN_DIV4			0xA
#define HSIOSC_EN_DIV8			0xB
#define HSIOSC_EN_DIV16		0xC
#define HSIOSC_EN_DIV32		0xD
/* External sub crystal oscillator control */
#define EXSOSC_STOP				0x0
#define EXSOSC_EN						0x8
#define EXSOSC_EN_DIV2		0x9
#define EXSOSC_EN_DIV4		0xA
/* External crystal oscillator control */
#define EXOSC_STOP					0x0
#define EXOSC_EN							0x8
#define EXOSC_EN_DIV2			0x9
#define EXOSC_EN_DIV4			0xA

/* PLLCON[BYPASS] */
#define PLLCON_BYPASS_FIN		0
#define PLLCON_BYPASS_PLL	1
/* PLLCON[PREDIV] */
#define PLLCON_FIN_DIV_1		0
#define PLLCON_FIN_DIV_2		1
/* PLLCON[FBCTRL] */
#define PLLCON_M_6			0
#define PLLCON_M_8			1
#define PLLCON_M_10		2
#define PLLCON_M_12		3
#define PLLCON_M_16		4
#define PLLCON_M_18		5
#define PLLCON_M_20		6
#define PLLCON_M_26		7
#define PLLCON_M_32		8
#define PLLCON_M_36		9
#define PLLCON_M_40		10
#define PLLCON_M_64		11
/* PLLCON[POSTDIV] */
#define PLLCON_N_1			0
#define PLLCON_N_2			1
#define PLLCON_N_3			2
#define PLLCON_N_4			3
#define PLLCON_N_6			4
#define PLLCON_N_8			5
#define PLLCON_N_16		7

/* External sub crystal oscillator stable timeout */
#define SOSC_STARTUP_TIMEOUT		0xFFUL
/* External crystal oscillator stable timeout */
#define MOSC_STARTUP_TIMEOUT		0xFFUL
/* PLL stable timeout */
#define PLL_STARTUP_TIMEOUT				0xFFUL

/* System clock control setting */
#define SC_LSIOSC					0x0
#define SC_EXSOSC				0x1
#define SC_HSIOSC					0x2
#define SC_HSIOSCPLL		0x3
#define SC_EXOSC					0x6
#define SC_EXOSCPLL		0x7

/* Clear clock source setting */
#define SCU_CLEAR_HSE								(0)
#define SCU_CLEAR_HSI									(4)
#define SCU_CLEAR_LSI									(8)
#define SCU_CLEAR_LSE									(12)

/* Reset source setting */
#define SCU_RSER_PINRST							(1<<6)
#define SCU_RSER_CPURST						(1<<5)
#define SCU_RSER_SWRST							(1<<4)
#define SCU_RSER_WDTRST						(1<<3)
#define SCU_RSER_MCKFRST					(1<<2)
#define SCU_RSER_MOFRST						(1<<1)
#define SCU_RSER_LVDRST						(1<<0)

/* WakeUp source setting */
#define SCU_WUER_LVDWUE					(1<<0)
#define SCU_WUER_WDTWUE					(1<<1)
#define SCU_WUER_WTWUE						(1<<2)
#define SCU_WUER_USART10WUE	(1<<3)
#define SCU_WUER_USART11WUE	(1<<4)
#define SCU_WUER_GPIOAWUE			(1<<8)
#define SCU_WUER_GPIOBWUE			(1<<9)
#define SCU_WUER_GPIOCWUE			(1<<10)
#define SCU_WUER_GPIODWUE			(1<<11)
#define SCU_WUER_GPIOEWUE			(1<<12)
#define SCU_WUER_GPIOFWUE				(1<<13)
#define SCU_WUER_I2C0WUE					(1<<14)
#define SCU_WUER_I2C1WUE					(1<<15)


//----------------- T1N SCU Clock Constant Definition -----------------
#define TIMER1nCLK_MCCR1						(0)
#define TIMER1nCLK_PCLK							(1)
//----------------- T20 SCU Clock Constant Definition -----------------
#define TIMER20CLK_MCCR2						(0)
#define TIMER20CLK_PCLK							(1)
//----------------- T30 SCU Clock Constant Definition -----------------
#define TIMER30CLK_MCCR2						(0)
#define TIMER30CLK_PCLK							(1)
//----------------- LED SCU Clock Constant Definition -----------------
#define LEDCLK_MCCR5									(0)
#define LEDCLK_PCLK											(1)
//--------------- Watch Timer Clock Constant Definition ---------------
#define WTCLK_MCCR3										(0)
#define WTCLK_SOSC											(1)
#define WTCLK_WDTRC										(2)
//------------- Watch-Dog Timer Clock Constant Definition -------------
#define WDTCLK_WDTRC									(0)
#define WDTCLK_MCCR3									(1)


/* Public Functions ----------------------------------------------------------- */
void HAL_SCU_LSI_ClockConfig(uint32_t lsicon);
void HAL_SCU_HSI_ClockConfig(uint32_t hsicon);
void HAL_SCU_LSE_ClockConfig(uint32_t lsecon);
void HAL_SCU_HSE_ClockConfig(uint32_t hsecon);
SUCCESS_FAIL_Type HAL_SCU_LSE_ClockMonitoring(void);
SUCCESS_FAIL_Type HAL_SCU_HSE_ClockMonitoring(void);
void HAL_SCU_ClearClockSource(uint8_t clock_source);
void HAL_SCU_SystemClockChange(uint32_t sysclkcon);
void HAL_SCU_ClockOutConfig(uint8_t clkodiv, EN_DIS_Type clkoen);
SUCCESS_FAIL_Type HAL_SCU_PLL_ClockConfig(EN_DIS_Type PLLEN, uint8_t BYPASSB, uint8_t PLLMODE, uint8_t PREDIV, uint8_t POSTDIV1, uint8_t POSTDIV2, uint8_t OUTDIV);
void HAL_SCU_ResetSourceConfig(uint32_t reset_source, EN_DIS_Type reset_en);
void HAL_SCU_WakeUpSourceConfig(uint32_t wakeup_source, EN_DIS_Type wakeup_en);
void HAL_SCU_Timer1n_ClockConfig(uint32_t t1nclk);
void HAL_SCU_Timer20_ClockConfig(uint32_t t20clk);
void HAL_SCU_Timer30_ClockConfig(uint32_t t30clk);
void HAL_SCU_LED_ClockConfig(uint32_t ledclk);
void HAL_SCU_WT_ClockConfig(uint32_t wtclk);
void HAL_SCU_WDT_ClockConfig(uint32_t wdtclk);


#endif		/* A31T21X_HAL_SCU_H */

/* --------------------------------- End Of File ------------------------------ */
