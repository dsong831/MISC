/**********************************************************************
* @file		test_pcu_A31T21x.c
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
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_pcu_main(void);
static void GPIO_PATTERN_RUN(void);
static void GPIO_DEBOUNCE_RUN(void);
static void GPIO_INTERRUT_RUN(void);
static void GPIO_INPUT_RUN(void);
static void GPIO_Toggle_RUN(PORT_Type *Pn);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_pcu_main(void)
{
	
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void GPIO_PATTERN_RUN(void)
{
	unsigned char i;

   // Pin mode setting
	PA->MOD = 0x55555555;	// Output
//	PA->MOD = 0;									// Input

#if 0
	PA->OUTDR = 0xFFFFFFFF;
	for(i=0; i<12; i++)
	{
		PA->OUTDR = (PA->OUTDR<<1);
	}
#else
	PA->OUTDR = 1;
	for(i=0; i<12; i++)
	{
		PA->OUTDR = (PA->OUTDR << 1);
	}
#endif

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void GPIO_DEBOUNCE_RUN(void)
{
#if 1	// Timer10 Output Set
	TIMER1n_PERIODICCFG_Type TIMER1n_Config;

	/* T10C pin config set if need*/
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	
  /*Timer1n clock source from PCLK*/
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Config.Prescaler = 32;
	/*TIMER1n 6usec	*/
	TIMER1n_Config.ADR = (6);
	TIMER1n_Config.StartLevel = TIMER1n_START_LOW;
	
	HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Config);
	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
#endif

	SCU->MCCR4 |= (1<<0);
	HAL_GPIO_SetDebouncePin(PA, 0);
	GPIO_INTERRUT_RUN();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void GPIO_INTERRUT_RUN(void)
{
	/* Port Access Enable */
	PORT_ACCESS_EN();
	
	// Input Mode Setting
	PA->MOD = 0;
	// Interrupt Setting (IER 1: Level, 3: Edge) (ICR 1: Low/Falling , 2: High/Rising, 3: Both Edge)
	PA->IER = (1<<0);
	PA->ICR = (1<<0);

	NVIC_SetPriority(GPIOAB_IRQn, 3);
	NVIC_EnableIRQ(GPIOAB_IRQn);
	__enable_irq();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void GPIO_INPUT_RUN(void)
{
   // Test Pin setting Pn
	PA->MOD = 0;
	PB->MOD = 0|(2<<6)|(2<<8)|(2<<10);
	PC->MOD = 0;
	PD->MOD = 0;
	PE->MOD = 0;
	PF->MOD = 0;
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void GPIO_Toggle_RUN(PORT_Type *Pn)
{
//	unsigned int i;
	
   // Test Pin setting Pn
	PA->MOD = 0x55555555;
	PC->MOD = 0x55555555;
	PD->MOD = 0x55555555;
	PE->MOD = 0x55555555;
	PF->MOD = 0x55555555;

	while(1)
	{
//		for(i=0; i<0x5FFFF; i++);
		Pn->OUTDR = 0xFFFFFFFF;
//		for(i=0; i<0x5FFFF; i++);
		Pn->OUTDR = 0x00000000;
	}
}

