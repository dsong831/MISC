/**********************************************************************
* @file		test_wt_A31T21x.c
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
static WT_CFG_Type wtCfg;
/* Private define ------------------------------------------------------------*/
#define WT_MCCR3
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t msec;
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_wt_main(void);
static void SysTick_Configure(void);
static void WT_ClockSource_Test(void);
static void WT_TEST_RUN(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_wt_main(void)
{
	SysTick_Configure();
	WT_ClockSource_Test();

	while(1)
	{
#if 0	// For Polling Test
		if(WT->CR & (1<<1))
			while(1);
#endif
#if 1
		msec = 2000;
		while(msec);
//		WT->CR |= (3<<0);
#endif
	}
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
static void WT_ClockSource_Test(void)
{
	/* Enable All Clock Source */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	HAL_SCU_HSI_ClockConfig(HSIOSC_EN);
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	HAL_SCU_HSE_ClockConfig(EXOSC_EN);

	/* Set WDT clock source */
#ifdef WT_WDTRC
	SCU->PPCLKSR |= (2<<3); 		//	WT clock source = WDTRC (31250 Hz)
#endif
#ifdef WT_LSE
	SCU->PPCLKSR |= (1<<3);			// WT clock source = LSE
#endif
#ifdef WT_MCCR3
	SCU->PPCLKSR &= ~(3<<3); 	//	WT clock source = MCCR3
#endif

	/*	Set WDT Config Struct */
#ifdef WT_WDTRC
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = 1.9Hz = WDTRC / 16384 = 31250 / 16384
	wtCfg.WTDR = 2 * 4;		// Set period 4s
	HAL_WT_Init(wtCfg, WT_ENABLE, WT_INT_ENABLE);
#endif
#ifdef WT_LSE
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = 1.9Hz = LSE / 16384 = 32768 / 16384
	wtCfg.WTDR = 2 * 4;		// Set period 4s
	HAL_WT_Init(wtCfg, WT_ENABLE, WT_INT_ENABLE);
#endif
#ifdef WT_MCCR3
#if 0	// LSI
	SCU->MCCR3 &= ~(7<<24);	// Clock Source Setting
	SCU->MCCR3 |= (1<<16);			// Clock Divider = 1
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = ClockSource / 16384
	wtCfg.WTDR = 30 * 4;							// Set period 4s
	HAL_WT_Init(wtCfg, WT_ENABLE, WT_INT_ENABLE);
#endif
#if 0	// LSE
	SCU->MCCR3 |= (3<<24);	// Clock Source Setting
	SCU->MCCR3 |= (1<<16);			// Clock Divider = 1
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = ClockSource / 16384
	wtCfg.WTDR = 2 * 4;							// Set period 4s
	HAL_WT_Init(wtCfg, WT_ENABLE, WT_INT_ENABLE);
#endif
#if 0	// MCLK
	SCU->MCCR3 |= (4<<24);	// Clock Source Setting
	SCU->MCCR3 |= (1<<16);			// Clock Divider = 1
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = ClockSource / 16384
	wtCfg.WTDR = 488 * 4;							// Set period 4s
	HAL_WT_Init(wtCfg, WT_ENABLE, WT_INT_ENABLE);
#endif
#if 0	// HSI 32MHz
	SCU->MCCR3 |= (5<<24);	// Clock Source Setting
	SCU->MCCR3 |= (4<<16);			// Clock Divider = 4
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = ClockSource / 16384
	wtCfg.WTDR = 488 * 4;							// Set period 4s
	HAL_WT_Init(wtCfg, WT_ENABLE, WT_INT_ENABLE);
#endif
#if 0	// HSE
	SCU->MCCR3 |= (6<<24);	// Clock Source Setting
	SCU->MCCR3 |= (1<<16);			// Clock Divider = 1
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = ClockSource / 16384
	wtCfg.WTDR = 488 * 4;							// Set period 4s
	HAL_WT_Init(wtCfg, WT_ENABLE, WT_INT_ENABLE);
#endif
#if 1	// PLL
	SCU->MCCR3 |= (7<<24);	// Clock Source Setting
	SCU->MCCR3 |= (6<<16);			// Clock Divider = 6
	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = ClockSource / 16384
	wtCfg.WTDR = 488 * 4;							// Set period 4s
	HAL_WT_Init(wtCfg);
	HAL_WT_ConfigInterrupt(ENABLE);
#endif
#endif

	NVIC_SetPriority(WT_IRQn, 3);
	NVIC_EnableIRQ(WT_IRQn);
	__enable_irq();

	HAL_WT_TimerStart(ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void WT_TEST_RUN(void)
{
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	SCU->PPCLKSR |= (2<<3);		// WT clock source = WDTRC

	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = 1.9Hz = WDTRC / 16384 = 31250 / 16384
	wtCfg.WTDR = 2 * 10;		// Set period 5s
	HAL_WT_Init(wtCfg);
	HAL_WT_ConfigInterrupt(ENABLE);

	NVIC_SetPriority(WT_IRQn, 3);
	NVIC_EnableIRQ(WT_IRQn);
	__enable_irq();

	HAL_WT_TimerStart(ENABLE);
}

