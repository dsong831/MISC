/**
 *******************************************************************************
 * @file        a31t21x_wdt.h
 * @brief       WDT driver
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
 * @defgroup    driver_wdt WDT
 * @{
 * @brief       WDT driver
 */


#ifndef _A31T21X_WDT_H_
#define _A31T21X_WDT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"

/**
 * WDT clock configuration
 */
enum wdt_clk_src
{
    WDT_CLK_SRC_WDTRC			= 0,							/**< WDTRC as clock source */
    WDT_CLK_SRC_MCCLK			= 1,  		        /**< miscellaneous clock */
};

/**
 * Miscellaneous Clock
 */
enum wdt_mcclk_sel
{
    WDT_MCCLK_LSI = 0,       		            /**< LSI clock */
    WDT_MCCLK_LSE = 3,          	          /**< LSE clock */
    WDT_MCCLK_MCK = 4,            	        /**< MCK clock */
    WDT_MCCLK_HSI = 5,              	      /**< HSI clock */
    WDT_MCCLK_HSE = 6,                	    /**< HSE clock */
    WDT_MCCLK_PLL = 7,                  	  /**< PLL clock */
};

/**
 * WDT clock divider configuration
 */
enum wdt_clk_div
{
    WDT_CLK_DIV_4		= 0,                    /**< clock divided by 4 */
    WDT_CLK_DIV_16	= 1,                    /**< clock divided by 16 */
    WDT_CLK_DIV_64	= 2,                    /**< clock divided by 64 */
    WDT_CLK_DIV_256	= 3,                    /**< clock divided by 256 */
};

/**
 * WDT mode configuration
 */
enum wdt_mode
{
    WDT_MODE_RESET	= 0,                    /**< reset on expiration */
    WDT_MODE_IRQ		= 1,                    /**< interrupt on expiration */
};

/**
 * WDT driver configuration
 */
struct wdt_cfg
{
    enum wdt_clk_src    clk_src;            /**< clock source */
    enum wdt_clk_div    clk_div;            /**< clock divider */
    enum wdt_mode       mode;               /**< mode */
		uint32_t						window_count;				/**< window data */
    uint8_t             irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       WDT interrupt callback function type
 * @param[in]   stacked_regs stacked register values during exception
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*wdt_irq_handler_t)(unsigned long *stacked_regs, void *context);

/**
 *******************************************************************************
 * @brief       Initialize WDT
 * @param[in]   cfg set of WDT configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Init(struct wdt_cfg *cfg, wdt_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Deinitialize WDT
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Deinit(void);

/**
 *******************************************************************************
 * @brief       Start WDT with initial counter.
 *              counter value is in unit of WDT source clock.
 * @param[in]   counter starting counter, re-used when ::HAL_WDT_Feed is called.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Start(uint32_t counter);

/**
 *******************************************************************************
 * @brief       Stop WDT.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Stop(void);

/**
 *******************************************************************************
 * @brief       Feed the WDT.
 *              It uses the previously installed counter.
 *              When the WDT is started, WDT must be feed periodically to avoid
 *              the interrupt or reset.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Feed(void);

/**
 *******************************************************************************
 * @brief       Get current WDT counter value.
 * @param[out]  counter counter value to get
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_GetCounter(uint32_t *counter);

/**
 *******************************************************************************
 * @brief       Clear current WDT status.
 * @param[out]  int_flag interrupt flag
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_ClearStatus(uint32_t int_flag);

/**
 *******************************************************************************
 * @brief       Set WDT clock
 *              WDT can be configure to use source clock other than MCLK.
 * @param[in]   clk_sel clock selection
 * @param[in]   mcclk_sel miscellaneous clock selection
 * @param[in]   mcclk_div miscellaneous clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_SetClock(enum wdt_clk_src clk_sel, enum wdt_mcclk_sel mcclk_sel, uint8_t mcclk_div);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_WDT_H_ */

/** @} */
/** @} */

