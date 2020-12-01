/**********************************************************************
* @file		test_core_A31T21x.c
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
static TIMER1n_PERIODICCFG_Type TIMER1n_Periodic_Config;
static TIMER2n_PERIODICCFG_Type TIMER2n_Periodic_Config;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_core_main(void);
static void OVER_CLOCK_TEST(void);
static void INT_PRIORITY_TEST(void);
static void SRAM_VECTOR_TEST(void);
static void SysTick_MCCR1_TEST(void);
static void SysTick_Configure(void);
static void PC5_Toggle_RUN(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_core_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
void OVER_CLOCK_TEST(void)
{
	/* Set Flach Access Time */
	FMC->MR = 0x81;
	FMC->MR = 0x28;
	FMC->CFG = (0x7858 << 16) | (1 << 8);		// Flash Access in 4 cycles (3-wait)
	FMC->MR = 0;
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
void INT_PRIORITY_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);

#if 1	// TIMER1n START
	/* Timer1n pin config set if need - Timer10/11/12/13 */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC1);

  /* Timer1n clock source */
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;

	/*TIMER1n 1msec	*/
	TIMER1n_Periodic_Config.ADR = (1000);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);

	/*TIMER1n ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_MIE, ENABLE);

  /* Enable Interrupt for TIMERn channel */
	NVIC_SetPriority(TIMER10_IRQn, 2);
	NVIC_EnableIRQ(TIMER10_IRQn);

	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
#endif

#if 1	// TIMER2n START
	/* Timer2n pin config set if need - Timer20/21 */
	HAL_GPIO_ModeConfig(PC, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PC, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 1, ALTERN_FUNC1);
	
  /*Timer2n clock source */
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_PCLK);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 32;    /* 32Mhz / 32 = 1Mhz ->1us*/
	TIMER2n_Periodic_Config.ADR = (1000);
	TIMER2n_Periodic_Config.StartLevel = TIMER2n_START_LOW;   
 
	HAL_TIMER2n_Init(TIMER20, TIMER2n_PERIODIC_MODE, &TIMER2n_Periodic_Config);
	HAL_TIMER2n_ConfigInterrupt(TIMER20, TIMER2n_CR_MATCH_INTR, ENABLE);

	/* Enable Interrupt for TIMERx channel */
	NVIC_SetPriority(TIMER20_IRQn, 1);
	NVIC_EnableIRQ(TIMER20_IRQn); 

	/*  timer start*/
	HAL_TIMER2n_Cmd(TIMER20, ENABLE);
#endif

	__enable_irq();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
void SRAM_VECTOR_TEST(void)
{
	SCB->VTOR = 0x20000000;
	__enable_irq();
}

/**********************************************************************
 * @brief		SysTick_Configure
 * @param[in]	None
 * @return 	None
 **********************************************************************/
void SysTick_MCCR1_TEST(void)
{
	SysTick_Config(SystemCoreClock/1000);
	SysTick->CTRL &= ~(1<<2);

	SCU->MCCR1 |= (6<<8) | (1<<0);		// 8[bit] 0: LSI / 3: LSE / 4: MCLK / 5: HSI / 6: HSE / 7: PLL
	
	__enable_irq();
}

/**********************************************************************
 * @brief		SysTick_Configure
 * @param[in]	None
 * @return 	None
 **********************************************************************/
void SysTick_Configure(void)
{
	/*1msec interrupt */
	SysTick_Config(SystemCoreClock/1000);
	__enable_irq();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
void PC5_Toggle_RUN(void)
{
	unsigned int i;

   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);

	while(1)
	{
		for(i=0; i<0x5FFFF; i++);
		HAL_GPIO_SetPin(PC, 5);
		for(i=0; i<0x5FFFF; i++);
		HAL_GPIO_ClearPin(PC, 5);
	}
}
