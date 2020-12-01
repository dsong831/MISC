/**********************************************************************
* @file		test_wdt_A31T21x.c
* @brief	Contains all macro definitions and function prototypes
* 			support for A31T21x firmware library
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/
#include "main_conf.h"


/* Private typedef -----------------------------------------------------------*/
static WDT_CFG_Type wdtCfg;
/* Private define ------------------------------------------------------------*/
#define	WDT_WDTRC	// 31250Hz (LSI / 16)
//#define	WDT_MCCR3		// LSI, LSE, MCLK, HSI, HSE, PLL
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t msec;
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_wdt_main(void);
static void WDT_Window_Test(void);
static void WDT_ClockSource_Test(void);
static void SysTick_Configure(void);
static void WDT_RESET_RUN(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_wdt_main(void)
{
	SysTick_Configure();
	WDT_Window_Test();
}

/**********************************************************************
 * @brief		SysTick_Configure
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void SysTick_Configure(void)
{
	/*1msec interrupt */
	SysTick_Config(SystemCoreClock/1000);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void WDT_Window_Test(void)
{
	/*WDT clock source from WDTRC. WDTRC must set LSI clock enable */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);

	/* Set WDTRC clock source */
	SCU->PPCLKSR &= ~(0x00000001); //	WDT clock source = WDTRC (31250 Hz)

	/*	Set WDT Config Struct */
	wdtCfg.CLKDIV = WDT_CLKDIV_4; // fwdt = 7812Hz = WDTRC / 4 = 31250 / 4
	wdtCfg.DR = 7812 * 6;		// Set period 4s
	wdtCfg.WINDR = 7812 * 3;
	wdtCfg.UNFIEN = ENABLE;
	wdtCfg.WINMIEN = ENABLE;
	wdtCfg.RSTEN = DISABLE;

	HAL_WDT_Init(wdtCfg);
	HAL_WDT_ConfigInterrupt(wdtCfg, ENABLE);
	HAL_WDT_CounterStart(ENABLE);
	HAL_WDT_ReloadCounter();

	NVIC_SetPriority(WDT_IRQn, 3);
	NVIC_EnableIRQ(WDT_IRQn);
	__enable_irq();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void WDT_ClockSource_Test(void)
{
	/* Enable All Clock Source */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	HAL_SCU_HSI_ClockConfig(HSIOSC_EN);
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	HAL_SCU_HSE_ClockConfig(EXOSC_EN);

	/* Set WDT clock source */
#ifdef WDT_WDTRC
	SCU->PPCLKSR &= ~(0x00000001); //	WDT clock source = WDTRC (31250 Hz)
#endif
#ifdef WDT_MCCR3
	SCU->PPCLKSR |= (0x00000001); 	//		WDT clock source = MCCR3
#endif

	/*	Set WDT Config Struct */
#ifdef WDT_WDTRC
	wdtCfg.CLKDIV = WDT_CLKDIV_4; // fwdt = 7812Hz = WDTRC / 4 = 31250 / 4
	wdtCfg.DR = 7812 * 4;		// Set period 4s
	wdtCfg.WINDR = 7812 * 4;
#endif
#ifdef WDT_MCCR3
#if 0	// LSI
	SCU->MCCR3 |= (0<<8) | (1<<0);	// 0: LSI, 3: LSE, 4: MCLK, 5: HSI, 6: HSE, 7: PLL
	wdtCfg.CLKDIV = WDT_CLKDIV_16;
	wdtCfg.DR = 31250 * 4;		// Set period 4s
	wdtCfg.WINDR = 31250 * 4;
#endif
#if 0	// LSE
	SCU->MCCR3 |= (3<<8) | (1<<0);	// 0: LSI, 3: LSE, 4: MCLK, 5: HSI, 6: HSE, 7: PLL
	wdtCfg.CLKDIV = WDT_CLKDIV_16;
	wdtCfg.DR = 1985 * 4;		// Set period 4s
	wdtCfg.WINDR = 1985 * 4;
#endif
#if 0	// MCLK
	SCU->MCCR3 |= (4<<8) | (1<<0);	// 0: LSI, 3: LSE, 4: MCLK, 5: HSI, 6: HSE, 7: PLL
	wdtCfg.CLKDIV = WDT_CLKDIV_256;
	wdtCfg.DR = 187500 * 4;		// Set period 4s
	wdtCfg.WINDR = 187500 * 4;
#endif
#if 0	// HSI
	SCU->MCCR3 |= (5<<8) | (1<<0);	// 0: LSI, 3: LSE, 4: MCLK, 5: HSI, 6: HSE, 7: PLL
	wdtCfg.CLKDIV = WDT_CLKDIV_256;
	wdtCfg.DR = 125000 * 4;		// Set period 4s
	wdtCfg.WINDR = 125000 * 4;
#endif
#if 0	// HSE
	SCU->MCCR3 |= (6<<8) | (1<<0);	// 0: LSI, 3: LSE, 4: MCLK, 5: HSI, 6: HSE, 7: PLL
	wdtCfg.CLKDIV = WDT_CLKDIV_64;
	wdtCfg.DR = 125000 * 4;		// Set period 4s
	wdtCfg.WINDR = 125000 * 4;
#endif
#if 1	// PLL
	SCU->MCCR3 |= (7<<8) | (1<<0);	// 0: LSI, 3: LSE, 4: MCLK, 5: HSI, 6: HSE, 7: PLL
	wdtCfg.CLKDIV = WDT_CLKDIV_256;
	wdtCfg.DR = 187500 * 4;		// Set period 4s
	wdtCfg.WINDR = 187500 * 4;
#endif
#endif
	wdtCfg.UNFIEN = DISABLE;
	wdtCfg.WINMIEN = DISABLE;
	wdtCfg.RSTEN = ENABLE;
	
	HAL_WDT_Init(wdtCfg);
	HAL_WDT_ConfigInterrupt(wdtCfg, DISABLE);
	HAL_WDT_CounterStart(ENABLE);
	HAL_WDT_ReloadCounter();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void WDT_RESET_RUN(void)
{
	/*WDT clock source from WDTRC. WDTRC must set LSI clock enable */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);

	/* Set WDTRC clock source */
	SCU->PPCLKSR &= ~(0x00000001); //	WDT clock source = WDTRC (31250 Hz)

	/*	Set WDT Config Struct */
	wdtCfg.CLKDIV = WDT_CLKDIV_4; // fwdt = 7812Hz = WDTRC / 4 = 31250 / 4
	wdtCfg.UNFIEN = ENABLE;
	wdtCfg.WINMIEN = DISABLE;
	wdtCfg.RSTEN = ENABLE;
	wdtCfg.DR = 7812 * 4;		// Set period 4s
	wdtCfg.WINDR = 7812 * 4;

	HAL_WDT_Init(wdtCfg);
	HAL_WDT_ConfigInterrupt(wdtCfg, ENABLE);
	HAL_WDT_CounterStart(ENABLE);
	HAL_WDT_ReloadCounter();

#if 0	// For NMI Test
	SCU->NMIR |= (0xA32CUL<<16)|(1<<2);
#endif

	NVIC_SetPriority(WDT_IRQn, 3);
	NVIC_EnableIRQ(WDT_IRQn);
	__enable_irq();
}

