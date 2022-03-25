/**
 *******************************************************************************
 * @file        a31t21x_led.h
 * @brief       led driver
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
 * @defgroup    driver_led LED
 * @{
 * @brief       led driver
 */

#ifndef _A31T21X_LED_H_
#define _A31T21X_LED_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"


enum led_clk_sel
{
    LED_CLK_MCCLK			= 0,							/**< miscellaneous clock */
    LED_CLK_PCLK			= 1,							/**< Peripheral clock */
};

enum led_mcclk_sel
{
    LED_MCCLK_LSI = 0,                    /**< LSI clock */
    LED_MCCLK_LSE = 3,                    /**< LSE clock */
    LED_MCCLK_MCK = 4,                    /**< MCK clock */
    LED_MCCLK_HSI = 5,                    /**< HSI clock */
    LED_MCCLK_HSE = 6,                    /**< HSE clock */
    LED_MCCLK_PLL = 7,                    /**< PLL clock */
};

enum led_mode
{
		LED_AUTO_MODE					= 0,
		LED_STOPCOUNT_MODE		= 2,
		LED_ALONE_MODE				= 4,
};

enum led_event
{
		LED_EVENT_DONE					=	(1 << 0),
		LED_EVENT_ERROR					=	(1 << 1),
};

/**
 *******************************************************************************
 * @brief       LED interrupt callback function type
 * @param[in]   event event type ::lcd_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*led_irq_handler_t)(uint32_t event, void *context);

/**
 * LED configuration
 */
struct led_cfg
{
		uint32_t					tPRESD;			/**< LED_CLK = fLED / (PRESD+1) */
		uint32_t					tCOMPWID;		/**< COM Width = LED_CLK / (COMPWID+1) */
		uint32_t					tLEDPD;			/**< LED Period = LED_CLK * LEDPD // ## LED Period > COM Width * count of COMER ## */
		uint32_t					tOVERLAP;		/**< 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = LED_CLK / 2 */
		uint32_t					tSR_INTE;		/**< LED Interrupt Enable */
		enum led_mode			tMODE;			/**< LED Mode Selection */
		enum led_clk_sel	clk_sel;		/**< clock selection */
		uint8_t						irq_prio;   /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize LED
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LED_Init(struct led_cfg *cfg, led_irq_handler_t handler, void *context);

/**********************************************************************
 * @brief				LED Display On
 * @param[in]		None
 * @return			None
 **********************************************************************/
int32_t HAL_LED_DISP_ON(void);

/**********************************************************************
 * @brief				LED Display Off
 * @param[in]		None
 * @return			None
 **********************************************************************/
int32_t HAL_LED_DISP_OFF(void);

/**
 *******************************************************************************
 * @brief       Set LED clock
 *              LED can be configure to use source clock other than MCLK.
 * @param[in]   clk_sel clock selection
 * @param[in]   mcclk_sel miscellaneous clock selection
 * @param[in]   mcclk_div miscellaneous clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LED_SetClock(enum led_clk_sel clk_sel, enum led_mcclk_sel mcclk_sel, uint8_t mcclk_div);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_LED_H_ */

/** @} */
/** @} */

