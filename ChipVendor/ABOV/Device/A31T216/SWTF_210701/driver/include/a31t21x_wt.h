/**
 *******************************************************************************
 * @file        a31t21x_wt.h
 * @brief       wt driver
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
 * @defgroup    driver_wt WT
 * @{
 * @brief       wt driver
 */

#ifndef _A31T21X_WT_H_
#define _A31T21X_WT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"


/**
 * wt clock selection
 */
enum wt_clk_sel
{
    WT_CLK_MCCLK			= 0,							/**< miscellaneous clock */
    WT_CLK_LSE				= 1,							/**< low speed external clock */
    WT_CLK_WDTRC			= 2,							/**< wdtrc clock */
};

/**
 * Miscellaneous Clock
 */
enum wt_mcclk_sel
{
    WT_MCCLK_LSI = 0,                    /**< LSI clock */
    WT_MCCLK_LSE = 3,                    /**< LSE clock */
    WT_MCCLK_MCK = 4,                    /**< MCK clock */
    WT_MCCLK_HSI = 5,                    /**< HSI clock */
    WT_MCCLK_HSE = 6,                    /**< HSE clock */
    WT_MCCLK_PLL = 7,                    /**< PLL clock */
};

/**
 * WT interval selection
 */
enum wt_interval_sel
{
		WT_INTERVAL_DIV_2_7				= 0,				/**< fWT/2^7 */
		WT_INTERVAL_DIV_2_13			= 1,				/**< fWT/2^13 */
		WT_INTERVAL_DIV_2_14			= 2,				/**< fWT/2^14 */
		WT_INTERVAL_DIV_2_14_WTDR	= 3,				/**< fWT/(2^14 x (WTDR+1)) */
};

/**
 * WT interrupt type
 */
enum wt_irq_type
{
    WT_IRQ_NONE						= 0,           /**< disable interrupt */
    WT_IRQ_COUNT_MATCH		= 1,           /**< counter match interrupt */
};

/**
 * wt event
 */
enum wt_event
{
    WT_EVENT_COUNT_MATCH	= BIT(0),			/**< counter match */
};

/**
 *******************************************************************************
 * @brief       wt interrupt callback function type
 * @param[in]   event event type ::wt_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*wt_irq_handler_t)(uint32_t event, void *context);

/**
 * wt configuration
 */
struct wt_cfg
{
    enum wt_clk_sel				clk_sel;            /**< clock selection */
		enum wt_interval_sel	interval_sel;				/**< interval selection */
		enum wt_irq_type			irq_type;           /**< interrupt selection */
    uint8_t           	  irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize wt.
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_Init(struct wt_cfg *cfg, wt_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Deintialize wt.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_Deinit(void);

/**
 *******************************************************************************
 * @brief       Get wt status
 * @param[out]  status status of the wt
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_GetStatus(uint8_t *status);

/**
 *******************************************************************************
 * @brief       Start wt in either oneshot mode or periodic mode.
 * @param[in]   count count
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_Start(uint16_t count);

/**
 *******************************************************************************
 * @brief       Stop wt.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_Stop(void);

/**
 *******************************************************************************
 * @brief       Get wt count
 * @param[out]  count the current count value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_GetCount(uint16_t *count);

/**
 *******************************************************************************
 * @brief       Clear wt count
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_ClearCount(void);

/**
 *******************************************************************************
 * @brief       Set wt clock
 *              wt can be configure to use source clock other than MCLK.
 * @param[in]   clk_sel clock selection
 * @param[in]   mcclk_sel miscellaneous clock selection
 * @param[in]   mcclk_div miscellaneous clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_SetClock(enum wt_clk_sel clk_sel, enum wt_mcclk_sel mcclk_sel, uint8_t mcclk_div);

/**
 *******************************************************************************
 * @brief       Set wt interrupt
 * @param[in]   type interrypt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WT_SetIRQ(enum wt_irq_type type, uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_WT_H_ */

/** @} */
/** @} */

