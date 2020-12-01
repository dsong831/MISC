/**********************************************************************
* @file		test_scu_A31T21x.c
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
static LVI_CFG_Type LviCfg;
static WDT_CFG_Type wdtCfg;
static WT_CFG_Type wtCfg;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_scu_main(void);
static void NMI_TEST(void);
static void Clock_Monitoring_TEST(void);
static void Sleep_DeepSleep_TEST(void);
static void RESET_SOURCE_TEST(void);
static void WDT_RESET_RUN(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_scu_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void NMI_TEST(void)
{
#if 0	// NMI LVDEN
	SCU->NMIR |= (0xA32CUL<<16)|(1<<0);

	LviCfg.LVIEN = LVI_ENABLE;
	LviCfg.LVIINTEN = LVI_INTEN;
	LviCfg.LVIVS = LVI_LVL_4_48V;
	HAL_LVI_Init(&LviCfg);
	HAL_SCU_ResetSourceConfig(SCU_RSER_LVDRST, DISABLE);
#endif

#if 0	// NMI MCLKFAILEN
	SCU->NMIR |= (0xA32CUL<<16)|(1<<1);

	HAL_SCU_HSE_ClockConfig(EXOSC_EN);
	SCU->CMR |= (3<<6);	// HSEMNT | HSEIE
	HAL_SCU_ResetSourceConfig(SCU_RSER_MCKFRST, DISABLE);
#endif

#if 0	// NMI WDTINTEN
	SCU->NMIR |= (0xA32CUL<<16)|(1<<2);

	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	SCU->PPCLKSR &= ~(1<<0);		// WDT clock source = WDTRC

	wdtCfg.CLKDIV = WDT_CLKDIV_4; // fwdt = 7812Hz = WDTRC / 4 = 31250 / 4
	wdtCfg.UNFIEN = ENABLE;
	wdtCfg.WINMIEN = DISABLE;
	wdtCfg.RSTEN = ENABLE;
	wdtCfg.DR = 7812 * 4;		// Set period 4s
	wdtCfg.WINDR = 7812 * 4;
	HAL_WDT_Init(wdtCfg);

	HAL_SCU_ResetSourceConfig(SCU_RSER_WDTRST, DISABLE);
	HAL_WDT_CounterStart(ENABLE);
	HAL_WDT_ReloadCounter();
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void Clock_Monitoring_TEST(void)
{
#if 1	// HSE Clock Monitoring
	HAL_SCU_HSE_ClockConfig(EXOSC_EN);
	SCU->CMR |= (3<<2);	// HSEMNT | HSEIE
	HAL_SCU_ResetSourceConfig(SCU_RSER_MOFRST, DISABLE);
#endif
#if 0	// Main Clock Monitoring
	HAL_SCU_HSE_ClockConfig(EXOSC_EN);
	SCU->CMR |= (3<<6);	// HSEMNT | HSEIE
	HAL_SCU_ResetSourceConfig(SCU_RSER_MCKFRST, DISABLE);
#endif
#if 0	// LSE Clock Monitoring
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	SCU->CMR |= (3<<10);	// LSEMNT | LSEIE
#endif
	
	NVIC_SetPriority(SYSCLKFAIL_IRQn, 3);
	NVIC_EnableIRQ(SYSCLKFAIL_IRQn);
	__enable_irq();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void Sleep_DeepSleep_TEST(void)
{
	volatile uint32_t Reg32;
	USART_CFG_Type USARTConfigStruct;

#if 0	// GPIOA WakeUp Source Setting
   // Test Pin setting - PA1 Input mode & External Interrupt
	HAL_GPIO_ModeConfig(PA, 1, INPUT);
	HAL_GPIO_EXTI_Config(PA, 1, IER_EDGE_EINT, ICR_FALLING_EDGE_EINT);

	NVIC_SetPriority(GPIOAB_IRQn, 3);
	NVIC_EnableIRQ(GPIOAB_IRQn);
	__enable_irq();

	HAL_SCU_WakeUpSourceConfig(SCU_WUER_GPIOAWUE, ENABLE);
#endif
#if 0	// GPIOB WakeUp Source Setting
   // Test Pin setting - PB1 Input mode & External Interrupt
	HAL_GPIO_ModeConfig(PB, 1, INPUT);
	HAL_GPIO_EXTI_Config(PB, 1, IER_EDGE_EINT, ICR_FALLING_EDGE_EINT);

	NVIC_SetPriority(GPIOAB_IRQn, 3);
	NVIC_EnableIRQ(GPIOAB_IRQn);
	__enable_irq();

	HAL_SCU_WakeUpSourceConfig(SCU_WUER_GPIOBWUE, ENABLE);
#endif
#if 0	// GPIOC WakeUp Source Setting
   // Test Pin setting - PC1 Input mode & External Interrupt
	HAL_GPIO_ModeConfig(PC, 1, INPUT);
	HAL_GPIO_EXTI_Config(PC, 1, IER_EDGE_EINT, ICR_FALLING_EDGE_EINT);

	NVIC_SetPriority(GPIOCD_IRQn, 3);
	NVIC_EnableIRQ(GPIOCD_IRQn);
	__enable_irq();

	HAL_SCU_WakeUpSourceConfig(SCU_WUER_GPIOCWUE, ENABLE);
#endif
#if 0	// GPIOD WakeUp Source Setting
   // Test Pin setting - PD1 Input mode & External Interrupt
	HAL_GPIO_ModeConfig(PD, 1, INPUT);
	HAL_GPIO_EXTI_Config(PD, 1, IER_EDGE_EINT, ICR_FALLING_EDGE_EINT);

	NVIC_SetPriority(GPIOCD_IRQn, 3);
	NVIC_EnableIRQ(GPIOCD_IRQn);
	__enable_irq();

	HAL_SCU_WakeUpSourceConfig(SCU_WUER_GPIODWUE, ENABLE);
#endif
#if 0	// GPIOE WakeUp Source Setting
   // Test Pin setting - PE1 Input mode & External Interrupt
	HAL_GPIO_ModeConfig(PE, 1, INPUT);
	HAL_GPIO_EXTI_Config(PE, 1, IER_EDGE_EINT, ICR_FALLING_EDGE_EINT);

	NVIC_SetPriority(GPIOE_IRQn, 3);
	NVIC_EnableIRQ(GPIOE_IRQn);
	__enable_irq();

	HAL_SCU_WakeUpSourceConfig(SCU_WUER_GPIOEWUE, ENABLE);
#endif
#if 0	// GPIOF WakeUp Source Setting
   // Test Pin setting - PF1 Input mode & External Interrupt
	HAL_GPIO_ModeConfig(PF, 1, INPUT);
	HAL_GPIO_EXTI_Config(PF, 1, IER_EDGE_EINT, ICR_FALLING_EDGE_EINT);

	NVIC_SetPriority(GPIOF_IRQn, 3);
	NVIC_EnableIRQ(GPIOF_IRQn);
	__enable_irq();

	HAL_SCU_WakeUpSourceConfig(SCU_WUER_GPIOFWUE, ENABLE);
#endif
#if 0	// LVD WakeUp Source Setting
	LviCfg.LVIEN = LVI_ENABLE;
	LviCfg.LVIINTEN = LVI_INTEN;
	LviCfg.LVIVS = LVI_LVL_4_48V;
	HAL_LVI_Init(&LviCfg);
	
	NVIC_SetPriority(LVI_IRQn, 3);
	NVIC_EnableIRQ(LVI_IRQn);
	__enable_irq();

	HAL_SCU_ResetSourceConfig(SCU_RSER_LVDRST, DISABLE);
	HAL_SCU_WakeUpSourceConfig(SCU_WUER_LVDWUE, ENABLE);
#endif
#if 0	// WDT WakeUp Source Setting
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	SCU->PPCLKSR &= ~(1<<0);		// WDT clock source = WDTRC

	wdtCfg.CLKDIV = WDT_CLKDIV_4; // fwdt = 7812Hz = WDTRC / 4 = 31250 / 4
	wdtCfg.UNFIEN = ENABLE;
	wdtCfg.WINMIEN = DISABLE;
	wdtCfg.RSTEN = ENABLE;
	wdtCfg.DR = 7812 * 3;		// Set period 4s
	wdtCfg.WINDR = 7812 * 3;
	HAL_WDT_Init(wdtCfg);

	NVIC_SetPriority(WDT_IRQn, 3);
	NVIC_EnableIRQ(WDT_IRQn);
	__enable_irq();

	HAL_SCU_ResetSourceConfig(SCU_RSER_WDTRST, DISABLE);
	HAL_SCU_WakeUpSourceConfig(SCU_WUER_WDTWUE, ENABLE);
	HAL_WDT_CounterStart(ENABLE);
	HAL_WDT_ReloadCounter();
#endif
#if 0	// WT WakeUp Source Setting
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	SCU->PPCLKSR |= (2<<3);		// WT clock source = WDTRC

	wtCfg.WTCLKDIV = WT_CLKDIV_2_14_MUL_WTDR;	// fwt = 1.9Hz = WDTRC / 16384 = 31250 / 16384
	wtCfg.WTDR = 2 * 4;		// Set period 4s
	HAL_WT_Init(wtCfg);

	NVIC_SetPriority(WT_IRQn, 3);
	NVIC_EnableIRQ(WT_IRQn);
	__enable_irq();
	
	HAL_SCU_WakeUpSourceConfig(SCU_WUER_WTWUE, ENABLE);
	HAL_WT_TimerStart(ENABLE);
#endif
#if 1	// USART WakeUp Source Setting
	/* Config USART11 PIN */
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC); //RXD11
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 3, 1);

	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC); //TXD11
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 2, 1);
	
	/* USART11 Setting */
	USARTConfigStruct.tBaudRate = 9600;
	USARTConfigStruct.tOpMode = USART_UART_MODE;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	HAL_USART_Init(USART11, &USARTConfigStruct);
	
	HAL_USART_ConfigInterrupt(USART11, USART_INT_RXCIE_TXCIE, ENABLE);
	NVIC_SetPriority(USART11_IRQn, 3);
	NVIC_EnableIRQ(USART11_IRQn);
	__enable_irq();

	HAL_SCU_WakeUpSourceConfig(SCU_WUER_USART11WUE, ENABLE);
#endif
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#if 1		// Sleep mode
	HAL_PWR_EnterSleepMode();
#else	// Deep sleep mode
	HAL_PWR_EnterPowerDownMode(0, 1);
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void RESET_SOURCE_TEST(void)
{
	/* Clear All Reset Source */
	SCU->RSSR = 0xFF;

#if 0		// For CPURST Test
	HAL_SCU_ResetSourceConfig(SCU_RSER_CPURST, ENABLE);
	NVIC_SystemReset();
#endif

#if 0		// For SWRST Test
	HAL_SCU_ResetSourceConfig(SCU_RSER_SWRST, ENABLE);
	SCU->SCR = (0x9EB3<<16) | (1<<0);
#endif

#if 0		// For WDTRST Test
	HAL_SCU_ResetSourceConfig(SCU_RSER_WDTRST, ENABLE);
	WDT_RESET_RUN();
#endif

#if 0		// For Main Clock Fail Reset Test
	HAL_SCU_ResetSourceConfig(SCU_RSER_MCKFRST, ENABLE);
#endif
	
#if 0		// For HSE Fail Reset Test
	HAL_SCU_ResetSourceConfig(SCU_RSER_MOFRST, ENABLE);
#endif

#if 0		// For LVR Test
	HAL_SCU_ResetSourceConfig(SCU_RSER_LVDRST, ENABLE);
#endif
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
	HAL_WDT_CounterStart(ENABLE);
	HAL_WDT_ReloadCounter();
}

