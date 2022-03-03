/**********************************************************************
* @file		test_timer_A31T21x.c
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
static TIMER1n_CAPTURECFG_Type TIMER1n_Capture_Config;
static TIMER1n_ONESHOTCFG_Type TIMER1n_OneShot_Config;
static TIMER1n_PWMCFG_Type					TIMER1n_PWM_Config;

static TIMER2n_PERIODICCFG_Type TIMER2n_Periodic_Config;
static TIMER2n_CAPTURECFG_Type TIMER2n_Capture_Config;
static TIMER2n_ONESHOTCFG_Type	TIMER2n_OneShot_Config;
static TIMER2n_PWMCFG_Type					TIMER2n_PWM_Config;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_timer_main(void);
static void TIMER30_ADCTRIG_TEST(void);
static void TIMER30_CAPTURE_TEST(void);
static void TIMER30_MATCH_TEST(void);
static void TIMER30_EC_TEST(void);
static void TIMER30_CLOCK_TEST(void);
static void TIMER2n_PWMMode_TEST(void);
static void TIMER2n_OneShotMode_TEST(void);
static void TIMER2n_CaptureMode_TEST(void);
static void TIMER2n_MatchMode_TEST(void);
static void TIMER2n_CLOCK_TEST(void);
static void TIMER1n_PWMMode_TEST(void);
static void TIMER1n_OneShotMode_TEST(void);
static void TIMER1n_CaptureMode_TEST(void);
static void TIMER1n_MatchMode_TEST(void);
static void TIMER1n_CLOCK_TEST(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_timer_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER30_ADCTRIG_TEST(void)
{
	ADC_CFG_Type AD_config;
	
	/* ADC Configure */
	AD_config.RefSel = ADC_REF_VDD;   
	AD_config.TrgSel = ADC_TRIGGER_TIMER30;	
	AD_config.InClkDiv = 12;      // 32M/17 =2.7M 
	HAL_ADC_Init(ADC, &AD_config);
	HAL_ADC_ConfigInterrupt(ADC, ENABLE);
	NVIC_SetPriority(ADC_IRQn, 3);
	NVIC_EnableIRQ(ADC_IRQn);	

	/* Timer30 pin config set if need - Timer30 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC1);

  /*Timer30 clock source */
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_PCLK);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 31);

	/* Output signal configure */
	HAL_TIMER3n_SetPeriod(TIMER30, 1000);
	HAL_TIMER3n_SetADuty(TIMER30, 300);
	HAL_TIMER3n_SetBDuty(TIMER30, 500);
	HAL_TIMER3n_SetCDuty(TIMER30, 700);
	HAL_TIMER3n_OutputCtrl(TIMER30,ENABLE,TIMER30O_APOLOW,TIMER30O_BPOHIGH);

#if 1
	HAL_TIMER3n_ConfigInterrupt(TIMER30,ENABLE,TIMER30INT_BOTEN);
	HAL_TIMER3n_ClearStatus_IT(TIMER30, 0x7F);
	HAL_TIMER3n_SetADCTrigger(TIMER30, TIMER30ADT_BTTGEN, 0);
	NVIC_EnableIRQ(TIMER30_IRQn);
	NVIC_SetPriority(TIMER30_IRQn, 3);
	__enable_irq();
#endif

	HAL_TIMER3n_MPWMCmd(TIMER30, TIMER30_CLKINT, TIMER30_BTOB, TIMER30_6CHMOD, TIMER30_UPMATCH, TIMER30_E1PERIOD);
	HAL_TIMER3n_Start(TIMER30,TIMER30_ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER30_CAPTURE_TEST(void)
{
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#if 1	// Input Source - Timer10(PE6)
  /* Timer 1n Period Start */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	/* Timer1n clock source */
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
	/*TIMER1n 1msec	*/
	TIMER1n_Periodic_Config.ADR = (99);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);
	/*TIMER1n ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_MIE, DISABLE);
	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
#endif
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
	
	/* Timer30 pin config set if need - Timer30 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC1);

	/* Timer30 Capture pin set */
	HAL_GPIO_ModeConfig(PF, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 7, ALTERN_FUNC1);

  /*Timer30 clock source */
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_PCLK);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 31);

	/* Period / Duty Setting */
	HAL_TIMER3n_SetPeriod(TIMER30, 1000);
	HAL_TIMER3n_SetADuty(TIMER30, 300);
	HAL_TIMER3n_SetBDuty(TIMER30, 500);
	HAL_TIMER3n_SetCDuty(TIMER30, 700);
	HAL_TIMER3n_OutputCtrl(TIMER30,ENABLE,TIMER30O_APOLOW,TIMER30O_BPOHIGH);

#if 1
	HAL_TIMER3n_ConfigInterrupt(TIMER30,ENABLE,TIMER30INT_CAPEN);
	HAL_TIMER3n_ClearStatus_IT(TIMER30, 0x7F);
	NVIC_EnableIRQ(TIMER30_IRQn);
	NVIC_SetPriority(TIMER30_IRQn, 3);
	__enable_irq();
#endif

	HAL_TIMER3n_MPWMCmd(TIMER30, TIMER30_CLKINT, TIMER30_CAPM, TIMER30_6CHMOD, TIMER30_UPMATCH, TIMER30_E1PERIOD);
	HAL_TIMER3n_Start(TIMER30,TIMER30_ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER30_MATCH_TEST(void)
{
	/* Timer30 pin config set if need - Timer30 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC1);

  /*Timer30 clock source */
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_PCLK);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 31);

	HAL_TIMER3n_SetPeriod(TIMER30, 1000);
	HAL_TIMER3n_SetADuty(TIMER30, 300); 
	HAL_TIMER3n_SetBDuty(TIMER30, 500);
	HAL_TIMER3n_SetCDuty(TIMER30, 700);
	HAL_TIMER3n_OutputCtrl(TIMER30,ENABLE,TIMER30O_APOLOW,TIMER30O_BPOHIGH);

#if 1	// Interrupt Setting
	HAL_TIMER3n_ConfigInterrupt(TIMER30,ENABLE,TIMER30INT_PMATEN);
	HAL_TIMER3n_ClearStatus_IT(TIMER30, 0x7F);
	NVIC_EnableIRQ(TIMER30_IRQn);
	NVIC_SetPriority(TIMER30_IRQn, 3);
	__enable_irq();
#endif

#if 0	// BLNK Setting
	HAL_GPIO_ModeConfig(PF, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 5, ALTERN_FUNC1);

	TIMER30->HIZCR |= (1<<7) | (1<<2);
#endif

	HAL_TIMER3n_MPWMCmd(TIMER30, TIMER30_CLKINT, TIMER30_INVM, TIMER30_6CHMOD, TIMER30_UPMATCH, TIMER30_E1PERIOD);
	HAL_TIMER3n_Start(TIMER30,TIMER30_ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER30_EC_TEST(void)
{
	/* Timer30 pin config set if need - Timer30 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC1);

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#if 1	// Input Source - Timer10(PE6)
  /* Timer 1n Period Start */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	/* Timer1n clock source */
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
	/*TIMER1n 1msec	*/
	TIMER1n_Periodic_Config.ADR = (10);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);
	/*TIMER1n ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_MIE, DISABLE);
	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
#endif
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_PCLK);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 1);

	/* Set External Clock */
	HAL_GPIO_ModeConfig(PF, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 6, ALTERN_FUNC1);
	
	HAL_TIMER3n_SetPeriod(TIMER30, 2000);
	HAL_TIMER3n_SetADuty(TIMER30,(TIMER30->PDR>>1)); 
	HAL_TIMER3n_SetBDuty(TIMER30,(TIMER30->PDR>>1));
	HAL_TIMER3n_SetCDuty(TIMER30,(TIMER30->PDR>>1));
	HAL_TIMER3n_OutputCtrl(TIMER30,ENABLE,TIMER30O_APOLOW,TIMER30O_BPOHIGH);

	HAL_TIMER3n_MPWMCmd(TIMER30, TIMER30_CLKINT, TIMER30_INVM, TIMER30_6CHMOD, TIMER30_UPMATCH, TIMER30_E1PERIOD);
	HAL_TIMER3n_Start(TIMER30,TIMER30_ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER30_CLOCK_TEST(void)
{
	/* Timer30 pin config set if need - Timer30 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC1);

	/* Set External Clock */
	HAL_GPIO_ModeConfig(PF, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 6, ALTERN_FUNC1);

	/* Enable All Clock Source */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	HAL_SCU_HSI_ClockConfig(HSIOSC_EN);
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	HAL_SCU_HSE_ClockConfig(EXOSC_EN);
	
  /*Timer30 clock source */
#if 0	// PCLK
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_PCLK);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 31);
#endif
#if 0	// MCCR2 - LSI 500kHz
	SCU->MCCR2 &= ~(7<<24);	// Clock Source Setting
	SCU->MCCR2 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_MCCR2);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 4);
#endif
#if 0	// MCCR2 - LSE 32kHz
	SCU->MCCR2 |= (3<<24);	// Clock Source Setting
	SCU->MCCR2 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_MCCR2);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 31);
#endif
#if 0	// MCCR2 - MCLK
	SCU->MCCR2 |= (4<<24);	// Clock Source Setting
	SCU->MCCR2 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_MCCR2);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 47);
#endif
#if 0	// MCCR2 - HSI
	SCU->MCCR2 |= (5<<24);	// Clock Source Setting
	SCU->MCCR2 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_MCCR2);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 31);
#endif
#if 0	// MCCR2 - HSE
	SCU->MCCR2 |= (6<<24);	// Clock Source Setting
	SCU->MCCR2 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_MCCR2);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 7);
#endif
#if 0	// MCCR2 - PLL
	SCU->MCCR2 |= (7<<24);	// Clock Source Setting
	SCU->MCCR2 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_MCCR2);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 47);
#endif

#if 0
	HAL_TIMER3n_ConfigInterrupt(TIMER30,ENABLE,TIMER30INT_PMATEN);
	HAL_TIMER3n_ClearStatus_IT(TIMER30, 0x7F);
	HAL_TIMER3n_SetADCTrigger(TIMER30,TIMER30ADT_BTTGEN,0);
	HAL_TIMER3n_SetDelayTime(TIMER30,TIMER30_DLYINSEN,TIMER30_INSFRONT, 63);	// (63 + 1) / 32MHz = 2us
	NVIC_EnableIRQ(TIMER30_IRQn);
	NVIC_SetPriority(TIMER30_IRQn, 3);
	__enable_irq();
#endif

	HAL_TIMER3n_SetPeriod(TIMER30, 1000);
	HAL_TIMER3n_SetADuty(TIMER30,(TIMER30->PDR>>1)); 
	HAL_TIMER3n_SetBDuty(TIMER30,(TIMER30->PDR>>1));
	HAL_TIMER3n_SetCDuty(TIMER30,(TIMER30->PDR>>1));
	HAL_TIMER3n_OutputCtrl(TIMER30,ENABLE,TIMER30O_APOLOW,TIMER30O_BPOHIGH);

	HAL_TIMER3n_MPWMCmd(TIMER30, TIMER30_CLKINT, TIMER30_INVM, TIMER30_6CHMOD, TIMER30_UPMATCH, TIMER30_E1PERIOD);
	HAL_TIMER3n_Start(TIMER30,TIMER30_ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER2n_PWMMode_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);

	/* Timer2n pin config set if need - Timer20/21 */
	HAL_GPIO_ModeConfig(PC, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PC, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 1, ALTERN_FUNC1);
	
  /*Timer2n clock source from PCLK*/
	HAL_SCU_Timer20_ClockConfig(TIMER20CLK_PCLK);
	TIMER2n_PWM_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_PWM_Config.Prescaler = 32;    // 32Mhz / 32 = 1Mhz ->1us  
  
	/*TIMER2n 1msec	*/
	TIMER2n_PWM_Config.ADR = (2000); 						// 2msec PERIOD	= ADATA
	TIMER2n_PWM_Config.BDR = (500); 						// 0.5msec DUTY = BDATA
	TIMER2n_PWM_Config.StartLevel = TIMER2n_START_LOW;   // START_LOW, START_HIGH
	
	HAL_TIMER2n_Init(TIMER21, TIMER2n_PWM_MODE, &TIMER2n_PWM_Config);

	/*TIMER20 ConfigInterrup*/
	HAL_TIMER2n_ConfigInterrupt(TIMER21, TIMER2n_CR_MATCH_INTR, ENABLE);

  /*timer start & clear*/
	HAL_TIMER2n_Cmd(TIMER21, ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER2n_OneShotMode_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);

	/* Timer2n pin config set if need - Timer20/21 */
	HAL_GPIO_ModeConfig(PC, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PC, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 1, ALTERN_FUNC1);
	
  /*Timer2n clock source from PCLK*/
	HAL_SCU_Timer20_ClockConfig(TIMER20CLK_PCLK);
	TIMER2n_OneShot_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_OneShot_Config.Prescaler = 32;    // 32Mhz / 32 = 1Mhz ->1us      
  
	/*TIMER2n 1msec	*/
  TIMER2n_OneShot_Config.ADR = (2000); 						// 2msec PERIOD	= ADATA
	TIMER2n_OneShot_Config.BDR = (300); 						// 0.5msec DUTY = BDATA
	TIMER2n_OneShot_Config.StartLevel = TIMER2n_START_HIGH;        
	
	HAL_TIMER2n_Init(TIMER20, TIMER2n_ONESHOT_MODE, &TIMER2n_OneShot_Config);

  /*timer start & clear*/
	HAL_TIMER2n_Cmd(TIMER20, ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER2n_CaptureMode_TEST(void)
{
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#if 1	// Input Source - Timer10(PE6)
  /* Timer 1n Period Start */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	/* Timer1n clock source */
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
	/*TIMER1n 1msec	*/
	TIMER1n_Periodic_Config.ADR = (1000);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);
	/*TIMER1n ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_MIE, DISABLE);
	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
#endif
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#if 1	// Capture setting
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);

	/* Timer2n pin config set if need - Timer20/21 */
	HAL_GPIO_ModeConfig(PC, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 0, ALTERN_FUNC2);
	HAL_GPIO_ModeConfig(PC, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 1, ALTERN_FUNC2);
	
  /*Timer2n clock source from PCLK*/
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_PCLK);
	TIMER2n_Capture_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Capture_Config.Prescaler = 32;    /* 32Mhz / 32 = 1Mhz ->1us*/
	TIMER2n_Capture_Config.ADR = (0xFFFF); 
	TIMER2n_Capture_Config.ClrMode=TIMER2n_RISING_EGDE;   
	TIMER2n_Capture_Config.CAPCkSel=TIMER2n_CAP_EXTERNAL_CLK;     
	
	HAL_TIMER2n_Init(TIMER21, TIMER2n_CAPTURE_MODE, &TIMER2n_Capture_Config);
	HAL_TIMER2n_ConfigInterrupt(TIMER21, TIMER2n_CR_CAPTURE_INTR, ENABLE);
	HAL_TIMER2n_ClearCaptureInterrupt(TIMER21);

#if 1
	/* Enable Interrupt for TIMERx channel */
	NVIC_SetPriority(TIMER21_IRQn, 3);
	NVIC_EnableIRQ(TIMER21_IRQn); 
	__enable_irq();
#endif

	HAL_TIMER2n_ClearCounter(TIMER21);
	/* timer start & clear*/
	HAL_TIMER2n_Cmd(TIMER21, ENABLE); 
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER2n_MatchMode_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);

	/* Timer2n pin config set if need - Timer20/21 */
	HAL_GPIO_ModeConfig(PC, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PC, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 1, ALTERN_FUNC1);
	
  /*Timer2n clock source */
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_PCLK);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 32;    /* 32Mhz / 32 = 1Mhz ->1us*/
	TIMER2n_Periodic_Config.ADR = (500);
	TIMER2n_Periodic_Config.StartLevel = TIMER2n_START_LOW;   
 
	HAL_TIMER2n_Init(TIMER21, TIMER2n_PERIODIC_MODE, &TIMER2n_Periodic_Config);
	HAL_TIMER2n_ConfigInterrupt(TIMER21, TIMER2n_CR_MATCH_INTR, ENABLE);

#if 1
	/* Enable Interrupt for TIMERx channel */
	NVIC_SetPriority(TIMER21_IRQn, 3);
	NVIC_EnableIRQ(TIMER21_IRQn); 
	__enable_irq();
#endif
	/*  timer start*/
	HAL_TIMER2n_Cmd(TIMER21, ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER2n_CLOCK_TEST(void)
{
	/* Timer1n pin config set if need - Timer20/21 */
	HAL_GPIO_ModeConfig(PC, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PC, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 1, ALTERN_FUNC1);

	/* Enable All Clock Source */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	HAL_SCU_HSI_ClockConfig(HSIOSC_EN);
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	HAL_SCU_HSE_ClockConfig(EXOSC_EN);
	
  /*Timer2n clock source */
#if 0	// PCLK
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_PCLK);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 32;    /* 32Mhz / 32 = 1Mhz ->1us*/
#endif
#if 0	// MCCR2 - LSI 500kHz
	SCU->MCCR2 &= ~(7<<8);	// Clock Source Setting
	SCU->MCCR2 |= (1<<0);			// Clock Divider = 1
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_MCCR2);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 5;
#endif
#if 0	// MCCR2 - LSE 32kHz
	SCU->MCCR2 |= (3<<8);	// Clock Source Setting
	SCU->MCCR2 |= (1<<0);			// Clock Divider = 1
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_MCCR2);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 32;
#endif
#if 0	// MCCR2 - MCLK
	SCU->MCCR2 |= (4<<8);	// Clock Source Setting
	SCU->MCCR2 |= (1<<0);			// Clock Divider = 1
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_MCCR2);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 48;
#endif
#if 0	// MCCR2 - HSI
	SCU->MCCR2 |= (5<<8);	// Clock Source Setting
	SCU->MCCR2 |= (1<<0);			// Clock Divider = 1
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_MCCR2);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 32;
#endif
#if 0	// MCCR2 - HSE
	SCU->MCCR2 |= (6<<8);	// Clock Source Setting
	SCU->MCCR2 |= (1<<0);			// Clock Divider = 1
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_MCCR2);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 8;
#endif
#if 1	// MCCR2 - PLL
	SCU->MCCR2 |= (7<<8);	// Clock Source Setting
	SCU->MCCR2 |= (1<<0);			// Clock Divider = 1
  HAL_SCU_Timer20_ClockConfig(TIMER20CLK_MCCR2);
	TIMER2n_Periodic_Config.CkSel = TIMER2n_MCCR2PCLK;    
	TIMER2n_Periodic_Config.Prescaler = 48;
#endif

	TIMER2n_Periodic_Config.ADR = (1000); 
	TIMER2n_Periodic_Config.StartLevel = TIMER2n_START_LOW;   
 
	HAL_TIMER2n_Init(TIMER20, TIMER2n_PERIODIC_MODE, &TIMER2n_Periodic_Config);
	HAL_TIMER2n_ConfigInterrupt(TIMER20, TIMER2n_CR_MATCH_INTR, ENABLE);

#if 0
	/* Enable Interrupt for TIMERx channel */
	NVIC_SetPriority(TIMER20_IRQn, 3);
	NVIC_EnableIRQ(TIMER20_IRQn); 
	__enable_irq();
#endif
	/*  timer start*/
	HAL_TIMER2n_Cmd(TIMER20, ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER1n_PWMMode_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);
	
	/* Timer1n pin config set if need - Timer10/11/12/13 */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC1);
	
  /*Timer1n clock source from PCLK*/
	HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_PWM_Config.CkSel = TIMER1n_MCCR1PCLK;    
	TIMER1n_PWM_Config.Prescaler = 32;       
  
	/*TIMER1n 1msec	*/
	TIMER1n_PWM_Config.ADR = (1000);
	TIMER1n_PWM_Config.BDR = (300);  // 300usec
	TIMER1n_PWM_Config.StartLevel = TIMER1n_START_HIGH;

	HAL_TIMER1n_Init(TIMER13, TIMER1n_PWM_MODE, &TIMER1n_PWM_Config);

  /*timer start & clear*/
	HAL_TIMER1n_Cmd(TIMER13, ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER1n_OneShotMode_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);
	
	/* Timer1n pin config set if need - Timer10/11/12/13 */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC1);
	
  /*Timer1n clock source from PCLK*/
	HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_OneShot_Config.CkSel = TIMER1n_MCCR1PCLK;    
	TIMER1n_OneShot_Config.Prescaler = 32;       
  
	/*TIMER1n 1msec	*/
	TIMER1n_OneShot_Config.ADR = (1000);
	TIMER1n_OneShot_Config.BDR = (500);  // 500usec
	TIMER1n_OneShot_Config.StartLevel = TIMER1n_START_HIGH;

	HAL_TIMER1n_Init(TIMER13, TIMER1n_ONESHOT_MODE, &TIMER1n_OneShot_Config);

  /*timer start & clear*/
	HAL_TIMER1n_Cmd(TIMER13, ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER1n_CaptureMode_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);
	
	/* Timer1n pin config set if need - Timer10/11/12/13 */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC2);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC2);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC2);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC2);

#if 0	// Input Source - Timer10(PE6)
  /* Timer 1n Period Start */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	/* Timer1n clock source */
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
	/*TIMER1n 1msec	*/
	TIMER1n_Periodic_Config.ADR = (500);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);
	/*TIMER1n ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_MIE, DISABLE);
	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
#else	// Input Source - Timer13(PA7)
  /* Timer 1n Period Start */
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC1);
	/* Timer1n clock source */
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
	/*TIMER1n 1msec	*/
	TIMER1n_Periodic_Config.ADR = (1000);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	HAL_TIMER1n_Init(TIMER13, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);
	/*TIMER1n ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER13, TIMER1n_INTCFG_MIE, DISABLE);
	HAL_TIMER1n_Cmd(TIMER13, ENABLE);
#endif
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#if 1	// Capture Signal Setting
  /*Timer1n clock source from PCLK*/
	HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Capture_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Capture_Config.Prescaler = 32;    // 32Mhz / 32 = 1Mhz ->1us
	TIMER1n_Capture_Config.ClrMode = TIMER1n_FALLING_EGDE;   //	FALLING_EGDE,RISING_EGDE,BOTH_EGDE,
	
	HAL_TIMER1n_Init(TIMER10, TIMER1n_CAPTURE_MODE, &TIMER1n_Capture_Config);
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_CIE, ENABLE);
	HAL_TIMER1n_ClearCaptureInterrupt(TIMER10);

	/* Enable Interrupt for TIMERx channel */
	NVIC_SetPriority(TIMER10_IRQn, 3);
	NVIC_EnableIRQ(TIMER10_IRQn);
	__enable_irq();

	HAL_TIMER1n_ClearCounter(TIMER10);
	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER1n_MatchMode_TEST(void)
{
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);
	
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
	HAL_TIMER1n_Init(TIMER13, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);

	/*TIMER1n ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER13, TIMER1n_INTCFG_MIE, ENABLE);

#if 1
  /* Enable Interrupt for TIMERn channel */
	NVIC_SetPriority(TIMER13_IRQn, 3);
	NVIC_EnableIRQ(TIMER13_IRQn);
	__enable_irq();
#endif

	HAL_TIMER1n_Cmd(TIMER13, ENABLE);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void TIMER1n_CLOCK_TEST(void)
{
	/* Timer1n pin config set if need - Timer10/11/12/13 */
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC1);

	/* Enable All Clock Source */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	HAL_SCU_HSI_ClockConfig(HSIOSC_EN);
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	HAL_SCU_HSE_ClockConfig(EXOSC_EN);
	
  /* Timer1n clock source */
#if 0	// PCLK
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
#endif
#if 0	// MCCR1 - LSI 500kHz
	SCU->MCCR1 &= ~(7<<24);	// Clock Source Setting
	SCU->MCCR1 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_MCCR1);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 5;
#endif
#if 0	// MCCR1 - LSE 32kHz
	SCU->MCCR1 |= (3<<24);			// Clock Source Setting
	SCU->MCCR1 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_MCCR1);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
#endif
#if 0	// MCCR1 - MCLK
	SCU->MCCR1 |= (4<<24);			// Clock Source Setting
	SCU->MCCR1 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_MCCR1);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 48;
#endif
#if 0	// MCCR1 - HSI 32MHz
	SCU->MCCR1 |= (5<<24);			// Clock Source Setting
	SCU->MCCR1 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_MCCR1);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
#endif
#if 0	// MCCR1 - HSE 8MHz
	SCU->MCCR1 |= (6<<24);			// Clock Source Setting
	SCU->MCCR1 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_MCCR1);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 8;
#endif
#if 0	// MCCR1 - PLL 48MHz
	SCU->MCCR1 |= (7<<24);			// Clock Source Setting
	SCU->MCCR1 |= (1<<16);			// Clock Divider = 1
  HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_MCCR1);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 48;
#endif

	/*TIMER1n 1msec	*/
	TIMER1n_Periodic_Config.ADR = (1000);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	
	HAL_TIMER1n_Init(TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);

	/*TIMER10 ConfigInterrup*/
	HAL_TIMER1n_ConfigInterrupt(TIMER10, TIMER1n_INTCFG_MIE, ENABLE);

#if 0
  /* Enable Interrupt for TIMERn channel */
	NVIC_SetPriority(TIMER10_IRQn, 3);
	NVIC_EnableIRQ(TIMER10_IRQn);
	__enable_irq();
#endif

	HAL_TIMER1n_Cmd(TIMER10, ENABLE);
}

