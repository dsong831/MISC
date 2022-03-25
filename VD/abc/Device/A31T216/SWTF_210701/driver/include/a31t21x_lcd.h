/**
 *******************************************************************************
 * @file        a31t21x_lcd.h
 * @brief       lcd driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

/**
 * @addtogroup  driver
 * @{
 * @defgroup    driver_lcd LCD
 * @{
 * @brief       lcd driver
 */

#ifndef _A31T21X_LCD_H_
#define _A31T21X_LCD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"


enum lcd_clk_sel
{
    LCD_CLK_MCCLK			= 0,								/**< miscellaneous clock */
    LCD_CLK_LSE				= 1,								/**< low speed external clock */
		LCD_CLK_WDTRC			= 2,								/**< wdtrc 31.250kHz */
};

enum lcd_mcclk_sel
{
    LCD_MCCLK_LSI = 0,                    /**< LSI clock */
    LCD_MCCLK_LSE = 3,                    /**< LSE clock */
    LCD_MCCLK_MCK = 4,                    /**< MCK clock */
    LCD_MCCLK_HSI = 5,                    /**< HSI clock */
    LCD_MCCLK_HSE = 6,                    /**< HSE clock */
    LCD_MCCLK_PLL = 7,                    /**< PLL clock */
};

enum lcd_bias_sel
{
		LCD_CR_INTBIAS					=	(0 << 8),
		LCD_CR_EXTBIAS					= (1 << 8),
};

enum lcd_resistor_sel
{
		LCD_CR_RLCD3						=	(0 << 6),
		LCD_CR_RLCD1						= (1 << 6),
		LCD_CR_RLCD2						= (2 << 6),
		LCD_CR_RLCD4						= (3 << 6),
};

enum lcd_dutybias_sel
{
		LCD_CR_Duty_div8_Bias_div4		=	(0 << 3),
		LCD_CR_Duty_div6_Bias_div4		= (1 << 3),
		LCD_CR_Duty_div5_Bias_div3		= (2 << 3),
		LCD_CR_Duty_div4_Bias_div3		= (3 << 3),
		LCD_CR_Duty_div3_Bias_div3		= (4 << 3),
		LCD_CR_Duty_div3_Bias_div2		= (5 << 3),
};

enum lcd_clock_sel	// When fLCD = 32.768kHz
{
		LCD_CR_LCLK_128HZ				=	(0 << 1),
		LCD_CR_LCLK_256HZ				= (1 << 1),
		LCD_CR_LCLK_512HZ				= (2 << 1),
		LCD_CR_LCLK_1024HZ			= (3 << 1),
};

enum lcd_autobias_sel
{
		LCD_BCCR_AUTOBIAS_OFF		=	(0 << 12),
		LCD_BCCR_AUTOBIAS_ON		= (1 << 12),
};

enum lcd_autobias_time_sel
{
		LCD_BCCR_BMSEL_1CLK			=	(0 << 8),
		LCD_BCCR_BMSEL_2CLK			= (1 << 8),
		LCD_BCCR_BMSEL_3CLK			= (2 << 8),
		LCD_BCCR_BMSEL_4CLK			= (3 << 8),
		LCD_BCCR_BMSEL_5CLK			= (4 << 8),
		LCD_BCCR_BMSEL_6CLK			= (5 << 8),
		LCD_BCCR_BMSEL_7CLK			= (6 << 8),
		LCD_BCCR_BMSEL_8CLK			= (7 << 8),
};

enum lcd_contrast_sel
{
		LCD_BCCR_CONTRAST_OFF		= (0 << 5),
		LCD_BCCR_CONTRAST_ON		= (1 << 5),
};

enum lcd_vlcd_sel
{
		LCD_BCCR_VLCD_31STEP		=	(0 << 0),
		LCD_BCCR_VLCD_30STEP		= (1 << 0),
		LCD_BCCR_VLCD_29STEP		= (2 << 0),
		LCD_BCCR_VLCD_28STEP		= (3 << 0),
		LCD_BCCR_VLCD_27STEP		= (4 << 0),
		LCD_BCCR_VLCD_26STEP		= (5 << 0),
		LCD_BCCR_VLCD_25STEP		= (6 << 0),
		LCD_BCCR_VLCD_24STEP		= (7 << 0),
		LCD_BCCR_VLCD_23STEP		= (8 << 0),
		LCD_BCCR_VLCD_22STEP		= (9 << 0),
		LCD_BCCR_VLCD_21STEP		= (10 << 0),
		LCD_BCCR_VLCD_20STEP		= (11 << 0),
		LCD_BCCR_VLCD_19STEP		= (12 << 0),
		LCD_BCCR_VLCD_18STEP		= (13 << 0),
		LCD_BCCR_VLCD_17STEP		= (14 << 0),
		LCD_BCCR_VLCD_16STEP		= (15 << 0),
};

enum lcd_event
{
		LCD_EVENT_DONE					=	(1 << 0),
		LCD_EVENT_ERROR					=	(1 << 1),
};

/**
 *******************************************************************************
 * @brief       LCD interrupt callback function type
 * @param[in]   event event type ::lcd_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*lcd_irq_handler_t)(uint32_t event, void *context);

/**
 * LCD configuration
 */
struct lcd_cfg
{
		enum lcd_bias_sel						lcddr;
		enum lcd_resistor_sel 			irsel;
		enum lcd_dutybias_sel 			dbs;
		enum lcd_clock_sel 					lclk;
		enum lcd_autobias_sel 			lcdabc;
		enum lcd_autobias_time_sel	bmsel;
		enum lcd_contrast_sel 			lcten;
		enum lcd_vlcd_sel 					vlcd;
		enum lcd_clk_sel						clk_sel;
		uint8_t											irq_prio;   /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize LCD
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LCD_Init(struct lcd_cfg *cfg, lcd_irq_handler_t handler, void *context);

/**********************************************************************
 * @brief				LCD Display On
 * @param[in]		None
 * @return			None
 **********************************************************************/
int32_t HAL_LCD_DISP_ON(void);

/**********************************************************************
 * @brief				LCD Display Off
 * @param[in]		None
 * @return			None
 **********************************************************************/
int32_t HAL_LCD_DISP_OFF(void);

/**
 *******************************************************************************
 * @brief       Set LCD clock
 *              LCD can be configure to use source clock other than MCLK.
 * @param[in]   clk_sel clock selection
 * @param[in]   mcclk_sel miscellaneous clock selection
 * @param[in]   mcclk_div miscellaneous clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LCD_SetClock(enum lcd_clk_sel clk_sel, enum lcd_mcclk_sel mcclk_sel, uint8_t mcclk_div);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_LCD_H_ */

/** @} */
/** @} */

