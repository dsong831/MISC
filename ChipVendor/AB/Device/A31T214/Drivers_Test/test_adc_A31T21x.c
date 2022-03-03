/**********************************************************************
* @file		test_adc_A31T21x.c
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
static ADC_CFG_Type AD_config;
static TIMER1n_PERIODICCFG_Type TIMER1n_Periodic_Config;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_adc_main(void);
static uint16_t adcaverage(uint16_t *val);
static void ADC_TRIGGER_TEST(void);
static void ADC_MODE_TEST(void);
/* Private variables ---------------------------------------------------------*/
volatile uint32_t fflag;
static uint16_t adcval[8];


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_adc_main(void)
{

}

/**********************************************************************
 * @brief		adcaverage
 * @param[in]	None
 * @return		None
 **********************************************************************/
static uint16_t adcaverage(uint16_t *val)
{
	uint32_t i;
	uint32_t adcavg;
	
	adcavg=0;			
	for (i=0;i<8;i++){	
		adcavg+=val[i];
	}
	adcavg>>=3;
	
	return	(adcavg);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void ADC_TRIGGER_TEST(void)
{
	uint32_t adcavg;
	
   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);
	
	/* Test Pin setting PA0 */
	HAL_GPIO_ModeConfig(PA, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 0, ALTERN_FUNC4);
	HAL_GPIO_PUPDConfig(PA, 0, PUPD_DISABLE);
	
	/* ADC Configure */
  AD_config.RefSel = ADC_REF_VDD;								// 0:ADC_REF_VDD ,1:ADC_REF_AVREF
#if 0	// SW Trigger
	AD_config.TrgSel = ADC_TRIGGER_DISABLE;
#endif
#if 1	// TIMER30 Trigger
	AD_config.TrgSel = ADC_TRIGGER_TIMER30;
#endif
#if 0	// TIMER1n Trigger
	AD_config.TrgSel = ADC_TRIGGER_TIMER12;
#endif
	
	AD_config.InClkDiv = 14;  
	HAL_ADC_Init(ADC, &AD_config);

	HAL_ADC_ConfigInterrupt(ADC, ENABLE);
	NVIC_SetPriority(ADC_IRQn, 3);
	NVIC_EnableIRQ(ADC_IRQn);
	__enable_irq();

	HAL_ADC_ChannelSel(ADC, 0); // select ch
	fflag=0;			
	HAL_ADC_ClearStatus(ADC);

#if 0	// SW Trigger
	HAL_ADC_Start(ADC); // start
#endif
#if 1	// TIMER30 Trigger
  HAL_SCU_Timer30_ClockConfig(TIMER30CLK_PCLK);
	HAL_TIMER3n_Init(TIMER30);
	HAL_TIMER3n_ClockPrescaler(TIMER30, 31);
	HAL_TIMER3n_SetPeriod(TIMER30, 1000);
	HAL_TIMER3n_SetADuty(TIMER30, 300);
	HAL_TIMER3n_SetBDuty(TIMER30, 500);
	HAL_TIMER3n_SetCDuty(TIMER30, 700);
	HAL_TIMER3n_OutputCtrl(TIMER30,ENABLE,TIMER30O_APOLOW,TIMER30O_BPOHIGH);
	HAL_TIMER3n_ConfigInterrupt(TIMER30,ENABLE,TIMER30INT_PMATEN);
	HAL_TIMER3n_ClearStatus_IT(TIMER30, 0x7F);
	HAL_TIMER3n_SetADCTrigger(TIMER30, TIMER30ADT_PMTGEN, 0);
	HAL_TIMER3n_MPWMCmd(TIMER30, TIMER30_CLKINT, TIMER30_INVM, TIMER30_6CHMOD, TIMER30_UPMATCH, TIMER30_E1PERIOD);
	HAL_TIMER3n_Start(TIMER30,TIMER30_ENABLE);
#endif
#if 0	// TIMER1n Trigger
	HAL_SCU_Timer1n_ClockConfig(TIMER1nCLK_PCLK);
	TIMER1n_Periodic_Config.CkSel = TIMER1n_MCCR1PCLK;
	TIMER1n_Periodic_Config.Prescaler = 32;
	TIMER1n_Periodic_Config.ADR = (1000);
	TIMER1n_Periodic_Config.StartLevel = TIMER1n_START_LOW;
	HAL_TIMER1n_Init(TIMER12, TIMER1n_PERIODIC_MODE, &TIMER1n_Periodic_Config);
	HAL_TIMER1n_Cmd(TIMER12, ENABLE);
#endif

	while(fflag==0){}
	adcval[0]=HAL_ADC_GetData(ADC);
	adcavg = adcaverage(adcval);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void ADC_MODE_TEST(void)
{
	uint32_t adcavg;
	
	/* Test Pin setting PAn */
	HAL_GPIO_ModeConfig(PA, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 0, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 1, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 2, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 3, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 4, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 6, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PB, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 6, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PB, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 7, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PA, 11, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 11, ALTERN_FUNC4);
	
	/* ADC Configure */
  AD_config.RefSel = ADC_REF_VDD;								// 0:ADC_REF_VDD ,1:ADC_REF_AVREF
	AD_config.TrgSel = ADC_TRIGGER_DISABLE;	// 
	AD_config.InClkDiv = 14;  
	HAL_ADC_Init(ADC, &AD_config);
	
	HAL_ADC_ConfigInterrupt(ADC, ENABLE);
	NVIC_SetPriority(ADC_IRQn, 3);
	NVIC_EnableIRQ(ADC_IRQn);
	__enable_irq();
	
	/* ADC RUN */
	HAL_ADC_ChannelSel(ADC, 14); // select ch
	fflag=0;			
	HAL_ADC_ClearStatus(ADC);
	HAL_ADC_Start(ADC); // start
	while(fflag==0){}
	adcval[0]=HAL_ADC_GetData(ADC);
	adcavg = adcaverage(adcval);
}
