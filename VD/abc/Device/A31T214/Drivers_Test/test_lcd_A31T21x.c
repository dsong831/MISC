/**********************************************************************
* @file		test_lcd_A31T21x.c
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
static LCD_CFG_Type	LCD_Config;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_lcd_main(void);
static void LCD_ExternalBias_Test(void);
static void LCD_InternalBias_Test(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_lcd_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LCD_ExternalBias_Test(void)
{
	/* VLC0 to VLC3 port setting */
	HAL_GPIO_ModeConfig(PE, 8, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 8, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PE, 9, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 9, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PE, 10, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 10, ALTERN_FUNC4);
	HAL_GPIO_ModeConfig(PE, 11, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 11, ALTERN_FUNC4);
	
	/* COM port setting - COM0 to COM3*/
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC0);

#if 1
	/* SEG port segging - SEG0 to SEG6 */
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 11, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 11, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 10, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 10, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 9, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 9, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 8, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 8, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 7, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 6, ALTERN_FUNC0);
#endif
#if 1
	/* SEG port segging - SEG9 to SEG16 */
	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 6, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 4, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 3, ALTERN_FUNC0);
#endif

	/* LCD Initialization */
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	HAL_SCU_LCD_ClockConfig(LCDCLK_SOSC);

	LCD_Config.tLCDDR = LCD_CR_EXTBIAS;
	LCD_Config.tIRSEL = LCD_CR_RLCD3;
	LCD_Config.tDBS = LCD_CR_Duty_div4_Bias_div3;
	LCD_Config.tLCLK = LCD_CR_LCLK_1024HZ;

	LCD_Config.tLCDABC = LCD_BCCR_AUTOBIAS_OFF;
	LCD_Config.tBMSEL = LCD_BCCR_BMSEL_4CLK;
	LCD_Config.tLCTEN = LCD_BCCR_CONTRAST_OFF;
	LCD_Config.tVLCD = LCD_BCCR_VLCD_18STEP;
	
	HAL_LCD_Init(LCD, &LCD_Config);


	/* Application Initialization */
	LCD_BUFFER_INIT(0x00);
	GDC8310_LCD_SET(1, 0, 1);

	/* LCD RUN */
	HAL_LCD_DISP_ON();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LCD_InternalBias_Test(void)
{
	/* COM port setting - COM0 to COM3*/
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC0);
#if 0	// COM4 to COM7
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 10, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 10, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 9, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 9, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 8, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 8, ALTERN_FUNC0);
#endif

#if 0
	/* SEG port segging - SEG0 to SEG6 */
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 11, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 11, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 10, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 10, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 9, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 9, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 8, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 8, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 7, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 6, ALTERN_FUNC0);
#endif
#if 1
	/* SEG port segging - SEG9 to SEG16 */
	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 7, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 6, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 5, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 4, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 3, ALTERN_FUNC0);
#endif
#if 1
	/* SEG port segging - SEG17 to SEG31 */
	HAL_GPIO_ModeConfig(PA, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 2, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 1, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 0, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 11, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 11, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 10, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 10, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PA, 9, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 9, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PF, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 4, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PF, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 5, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PF, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 6, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PF, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 7, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 8, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 8, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 9, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 9, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 10, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 10, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 11, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 11, ALTERN_FUNC0);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC0);
#endif

	/* LCD Initialization */
	HAL_SCU_LSE_ClockConfig(EXSOSC_EN);
	HAL_SCU_LCD_ClockConfig(LCDCLK_SOSC);

	LCD_Config.tLCDDR = LCD_CR_INTBIAS;
	LCD_Config.tIRSEL = LCD_CR_RLCD3;
//	LCD_Config.tDBS = LCD_CR_Duty_div4_Bias_div3;
	LCD_Config.tDBS = LCD_CR_Duty_div3_Bias_div3;
	LCD_Config.tLCLK = LCD_CR_LCLK_512HZ; // 128/256/512/1024

	LCD_Config.tLCDABC = LCD_BCCR_AUTOBIAS_OFF;
	LCD_Config.tBMSEL = LCD_BCCR_BMSEL_4CLK;
	LCD_Config.tLCTEN = LCD_BCCR_CONTRAST_OFF;
	LCD_Config.tVLCD = LCD_BCCR_VLCD_18STEP;
	
	HAL_LCD_Init(LCD, &LCD_Config);


	/* Application Initialization */
	LCD_BUFFER_INIT(0x5A);
	GDC8310_LCD_SET(5, 12, 13);

	/* LCD RUN */
	HAL_LCD_DISP_ON();
}
