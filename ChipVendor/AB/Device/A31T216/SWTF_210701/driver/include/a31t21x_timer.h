/**
 *******************************************************************************
 * @file        a31t21x_timer.h
 * @brief       TIMER driver
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
 * @defgroup    driver_timer TIMER
 * @{
 * @brief       TIMER driver
 */

#ifndef _A31T21X_TIMER_H_
#define _A31T21X_TIMER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"

/**
 * Timer id
 */
enum timer_id
{
    TIMER_ID_10				= 0,									/**< TIMER 10 */
    TIMER_ID_11				= 1,									/**< TIMER 11 */
    TIMER_ID_12				= 2,									/**< TIMER 12 */
    TIMER_ID_13				= 3,									/**< TIMER 13 */
    TIMER_ID_20				= 4,									/**< TIMER 20 */
    TIMER_ID_21				= 5,									/**< TIMER 21 */
    TIMER_ID_MAX			= 6,									/**< TIMER max */
};

/**
 * TIMER mode
 */
enum timer_mode
{
    TIMER_MODE_PERIODIC = 0,                /**< periodic mode */
		TIMER_MODE_CAPTURE	= 1,                /**< capture mode */
    TIMER_MODE_ONESHOT  = 2,                /**< oneshot mode */
    TIMER_MODE_PWM      = 3,                /**< pwm mode */
};

/**
 * TIMER clock selection
 */
enum timer_clk_sel
{
    TIMER_CLK_MCCLK			= 0,  		          /**< miscellaneous clock */
    TIMER_CLK_PCLK			= 1,    		        /**< PCLK divided by 1 */
    TIMER_CLK_TNIO			= 2,      		      /**< input to pin */
};

/**
 * Miscellaneous Clock
 */
enum timer_mcclk_sel
{
    TIMER_MCCLK_LSI = 0,                    /**< LSI clock */
    TIMER_MCCLK_LSE = 3,                    /**< LSE clock */
    TIMER_MCCLK_MCK = 4,                    /**< MCK clock */
    TIMER_MCCLK_HSI = 5,                    /**< HSI clock */
    TIMER_MCCLK_HSE = 6,                    /**< HSE clock */
    TIMER_MCCLK_PLL = 7,                    /**< PLL clock */
};

/**
 * TIMER PWM start level
 */
enum timer_pwm_start_level
{
    TIMER_PWM_START_LOW     = 0,            /**< PWM mode start low */
    TIMER_PWM_START_HIGH    = 1,            /**< PWM mode start high */
};

/**
 * TIMER capture clear mode
 */
enum timer_capture_clear
{
    TIMER_CAPTURE_CLEAR_FALLING = 0,  	    /**< capture clear on falling edge */
    TIMER_CAPTURE_CLEAR_RISING  = 1,    	  /**< capture clear on rising edge */
    TIMER_CAPTURE_CLEAR_BOTH    = 2,    	  /**< capture clear on both edges */
    TIMER_CAPTURE_CLEAR_NONE    = 3,				/**< capture clear not used */
};

/**
 * TIMER PWM event flag
 */
enum timer_pwm_event
{
    TIMER_PWM_EVENT_NONE    = 0,            /**< enable no event */
    TIMER_PWM_EVENT_PERIOD  = 1,						/**< enable period event */
};

/**
 * TIMTER capture register
 */
enum timer_capture_reg
{
    TIMER_CAPTURE_REG_CAPDR		= 0,          /**< capture with CAPDR */
    TIMER_CAPTURE_REG_LSE			= 1,          /**< capture with LSE (Only TIMER20) */
    TIMER_CAPTURE_REG_WDTRC		= 2,          /**< capture with WDTRC (Only TIMER20) */
};

/**
 * TIMTER interrupt type
 */
enum timer_irq_type
{
    TIMER_IRQ_COUNT_MATCH		= 0,            /**< counter match */
    TIMER_IRQ_CAPTURE_CAPDR	= 1,            /**< capture with CAPDR */
    TIMER_IRQ_PWM_PERIOD    = 2,            /**< pwm period */
};

/**
 * TIMER event
 */
enum timer_event
{
    TIMER_EVENT_COUNT_MATCH		= BIT(0),     /**< counter match */
    TIMER_EVENT_CAPTURE_CAPDR = BIT(1),     /**< capture with CAPDR */
    TIMER_EVNET_PWM_PERIOD		= BIT(2),     /**< pwm period */
};

/**
 *******************************************************************************
 * @brief       TIMER interrupt callback function type
 * @param[in]   event event type ::timer_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*timer_irq_handler_t)(uint32_t event, void *context);

/**
 * TIMER configuration
 */
struct timer_cfg
{
    enum timer_mode     mode;               /**< mode */
    enum timer_clk_sel  clk_sel;            /**< clock selection */
    uint16_t            prescaler;          /**< prescaler */
    uint8_t             irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize TIMER.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Init(enum timer_id id, struct timer_cfg *cfg,
                       timer_irq_handler_t handler,
                       void *context);

/**
 *******************************************************************************
 * @brief       Deintialize TIMER.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Deinit(enum timer_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER status
 * @param[in]   id instance id
 * @param[out]  status status of the TIMER
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_GetStatus(enum timer_id id, uint8_t *status);

/**
 *******************************************************************************
 * @brief       Start TIMER in either oneshot mode or periodic mode.
 * @param[in]   id instance id
 * @param[in]   count count
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Start(enum timer_id id, uint32_t count);

/**
 *******************************************************************************
 * @brief       Start TIMER in PWM mode.
 * @param[in]   id instance id
 * @param[in]   level PWM start level
 * @param[in]   duty PWM duty
 * @param[in]   period PWM period
 * @param[in]   event PWM event to b enabled, see ::timer_pwm_event
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_StartPWM(enum timer_id id, enum timer_pwm_start_level level,
                           uint32_t duty, uint32_t period,
                           uint8_t event);

/**
 *******************************************************************************
 * @brief       Start TIMER in capture mode.
 * @param[in]   id instance id
 * @param[in]   clear condition for clearing
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_StartCapture(enum timer_id id, enum timer_capture_clear clear);

/**
 *******************************************************************************
 * @brief       Stop TIMER.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Stop(enum timer_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER count
 * @param[in]   id instance id
 * @param[out]  count the current count value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_GetCount(enum timer_id id, uint32_t *count);

/**
 *******************************************************************************
 * @brief       Clear TIMER count
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_ClearCount(enum timer_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER capture count
 * @param[in]   id instance id
 * @param[out]  count the current count value
 * @param[in]   reg which register to use for the capture
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_GetCaptureCount(enum timer_id id, uint32_t *count, enum timer_capture_reg reg);

/**
 *******************************************************************************
 * @brief       Set TIMER clock
 *              Timer can be configure to use source clock other than MCLK.
 * @param[in]   id instance id
 * @param[in]   clk_sel clock selection
 * @param[in]   prescaler pre-scaler
 * @param[in]   mcclk_sel miscellaneous clock selection
 * @param[in]   mcclk_div miscellaneous clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_SetClock(enum timer_id id, enum timer_clk_sel clk_sel, uint16_t prescaler,
                           enum timer_mcclk_sel mcclk_sel, uint8_t mcclk_div);

/**
 *******************************************************************************
 * @brief       Set TIMER interrupt
 * @param[in]   id instance id
 * @param[in]   type interrypt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_SetIRQ(enum timer_id id, enum timer_irq_type type, uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_TIMER_H_ */

/** @} */
/** @} */

