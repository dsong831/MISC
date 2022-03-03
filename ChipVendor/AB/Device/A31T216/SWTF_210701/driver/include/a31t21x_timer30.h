/**
 *******************************************************************************
 * @file        a31t21x_timer30.h
 * @brief       TIMER30 driver
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
 * @defgroup    driver_timer TIMER30
 * @{
 * @brief       TIMER30 driver
 */

#ifndef _A31T21X_TIMER30_H_
#define _A31T21X_TIMER30_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"

/**
 * TIMER30 id
 */
enum timer30_id
{
    TIMER30_ID_30				= 0,								/**< TIMER 30 */
    TIMER30_ID_MAX			= 1,								/**< TIMER max */
};

/**
 * TIMER30 mode
 */
enum timer30_mode
{
    TIMER30_MODE_INTERVAL		= 0,						/**< interval mode */
		TIMER30_MODE_CAPTURE		= 1,						/**< capture mode */
    TIMER30_MODE_BACKTOBACK	= 2,						/**< backtoback mode */
};

/**
 * TIMER30 output mode
 */
enum timer30_output_mode
{
    TIMER30_OUTPUT_MODE_6_CH		= 0,					/**< 6-channel mode */
    TIMER30_OUTPUT_MODE_FORCE_A	= 1,					/**< Force A-channel mode */
};

/**
 * TIMER30 clock selection
 */
enum timer30_clk_sel
{
    TIMER30_CLK_MCCLK			= 0,							/**< miscellaneous clock */
    TIMER30_CLK_PCLK			= 1,							/**< PCLK divided by 1 */
    TIMER30_CLK_TNIO			= 2,							/**< input to pin */
};

/**
 * Miscellaneous Clock
 */
enum timer30_mcclk_sel
{
    TIMER30_MCCLK_LSI = 0,                  /**< LSI clock */
    TIMER30_MCCLK_LSE = 3,                  /**< LSE clock */
    TIMER30_MCCLK_MCK = 4,                  /**< MCK clock */
    TIMER30_MCCLK_HSI = 5,                  /**< HSI clock */
    TIMER30_MCCLK_HSE = 6,                  /**< HSE clock */
    TIMER30_MCCLK_PLL = 7,                  /**< PLL clock */
};

/**
 * TIMER30 PWM start level
 */
enum timer30_pwm_start_level
{
    TIMER30_PWM_START_LOW     = 0,          /**< PWM mode start low */
    TIMER30_PWM_START_HIGH    = 1,          /**< PWM mode start high */
};

/**
 * TIMER30 capture clear mode
 */
enum timer30_capture_clear
{
    TIMER30_CAPTURE_CLEAR_FALLING = 0,  	  /**< capture clear on falling edge */
    TIMER30_CAPTURE_CLEAR_RISING  = 1,    	/**< capture clear on rising edge */
    TIMER30_CAPTURE_CLEAR_BOTH    = 2,    	/**< capture clear on both edges */
    TIMER30_CAPTURE_CLEAR_NONE    = 3,			/**< capture clear not used */
};

/**
 * TIMER30 PWM event flag
 */
enum timer30_pwm_event
{
		TIMER30_PWM_EVENT_C_DUTY  = 0,					/**< enable c-duty event */
		TIMER30_PWM_EVENT_B_DUTY  = 1,					/**< enable b-duty event */
		TIMER30_PWM_EVENT_A_DUTY  = 2,					/**< enable a-duty event */
    TIMER30_PWM_EVENT_PERIOD  = 3,					/**< enable period event */
    TIMER30_PWM_EVENT_NONE    = 4,          /**< enable no event */
};

/**
 * TIMTER30 capture register
 */
enum timer30_capture_reg
{
    TIMER30_CAPTURE_REG_CAPDR		= 0,          /**< capture with CAPDR */
};

/**
 * TIMTER30 adc trigger signal
 */
enum timer30_adc_trigger_sig
{
    TIMER30_ADC_TRIG_C_MATCH			= 0,        /**< duty-C match */
    TIMER30_ADC_TRIG_B_MATCH			= 1,        /**< duty-B match */
    TIMER30_ADC_TRIG_A_MATCH			= 2,        /**< duty-A match */
    TIMER30_ADC_TRIG_PERIOD_MATCH	= 3,        /**< period match */
    TIMER30_ADC_TRIG_BOTTOM_MATCH	= 4,        /**< bottom match */
};

/**
 * TIMTER30 delay time insertion position
 */
enum timer30_delay_time_position
{
    TIMER30_DELAY_A_FRONT			= 0,        /**< insert at front of PWM30xA and at back of PWM30xB */
    TIMER30_DELAY_B_FRONT			= 1,        /**< insert at back of PWM30xA and at front of PWM30xB */
};

/**
 * TIMTER30 high impedance edge selection
 */
enum timer30_hi_z_edge_sel
{
    TIMER30_HI_Z_FALLING_EDGE			= 0,        /**< falling edge of the BLNK30 pin */
    TIMER30_HI_Z_RISING_EDGE			= 1,        /**< rising edge of the BLNK30 pin */
};

/**
 * TIMTER30 interrupt type
 */
enum timer30_irq_type
{
    TIMER30_IRQ_C_MATCH				= 0,            /**< duty-C match */
    TIMER30_IRQ_B_MATCH				= 1,            /**< duty-B match */
    TIMER30_IRQ_A_MATCH				= 2,            /**< duty-A match */
    TIMER30_IRQ_PERIOD_MATCH	= 3,            /**< period match */
    TIMER30_IRQ_BOTTOM_MATCH	= 4,            /**< bottom match */
    TIMER30_IRQ_CAPTURE_CAPDR	= 5,            /**< capture with CAPDR */
    TIMER30_IRQ_HI_Z					= 6,						/**< high-impedance */
};

/**
 * TIMER30 event
 */
enum timer30_event
{
    TIMER30_EVENT_C_MATCH				= BIT(0),			/**< duty-C match */
    TIMER30_EVENT_B_MATCH				= BIT(1),			/**< duty-B match */
    TIMER30_EVENT_A_MATCH				= BIT(2),			/**< duty-A match */
    TIMER30_EVENT_PERIOD_MATCH	= BIT(3),			/**< period match */
    TIMER30_EVENT_BOTTOM_MATCH	= BIT(4),			/**< bottom match */
    TIMER30_EVENT_CAPTURE_CAPDR	= BIT(5),			/**< capture with CAPDR */
    TIMER30_EVENT_HI_Z					= BIT(6),			/**< high-impedance */
};

/**
 *******************************************************************************
 * @brief       TIMER interrupt callback function type
 * @param[in]   event event type ::timer_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*timer30_irq_handler_t)(uint32_t event, void *context);

/**
 * TIMER30 configuration
 */
struct timer30_cfg
{
    enum timer30_mode						mode;               /**< mode */
    enum timer30_output_mode		output_mode;				/**< output mode */
    enum timer30_clk_sel				clk_sel;            /**< clock selection */
    uint16_t										prescaler;          /**< prescaler */
    uint8_t											irq_prio;           /**< interrupt priority */
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
int32_t HAL_TIMER30_Init(enum timer30_id id, struct timer30_cfg *cfg,
                       timer30_irq_handler_t handler,
                       void *context);

/**
 *******************************************************************************
 * @brief       Deintialize TIMER.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_Deinit(enum timer30_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER status
 * @param[in]   id instance id
 * @param[out]  status status of the TIMER
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_GetStatus(enum timer30_id id, uint8_t *status);

/**
 *******************************************************************************
 * @brief       Start TIMER in PWM mode.
 * @param[in]   id instance id
 * @param[in]   level PWM start level
 * @param[in]   a_duty PWM A-duty
 * @param[in]   b_duty PWM B-duty
 * @param[in]   c_duty PWM C-duty
 * @param[in]   period PWM period
 * @param[in]   event PWM event to b enabled, see ::timer_pwm_event
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_StartPWM(enum timer30_id id, enum timer30_pwm_start_level level,
                           uint16_t a_duty, uint16_t b_duty, uint16_t c_duty, uint16_t period,
                           uint8_t event);

/**
 *******************************************************************************
 * @brief       Start TIMER in capture mode.
 * @param[in]   id instance id
 * @param[in]   clear condition for clearing
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_StartCapture(enum timer30_id id, enum timer30_capture_clear clear);

/**
 *******************************************************************************
 * @brief       Stop TIMER.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_Stop(enum timer30_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER count
 * @param[in]   id instance id
 * @param[out]  count the current count value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_GetCount(enum timer30_id id, uint16_t *count);

/**
 *******************************************************************************
 * @brief       Clear TIMER count
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_ClearCount(enum timer30_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER capture count
 * @param[in]   id instance id
 * @param[out]  count the current count value
 * @param[in]   reg which register to use for the capture
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_GetCaptureCount(enum timer30_id id, uint16_t *count, enum timer30_capture_reg reg);

/**
 *******************************************************************************
 * @brief       Set TIMER adc trigger
 * @param[in]   id instance id
 * @param[in]   sig adc trigger signal type
 * @param[in]   counter adc count timer value
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_SetAdcTrigger(enum timer30_id id, enum timer30_adc_trigger_sig sig, uint16_t counter, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Set TIMER delay time
 * @param[in]   id instance id
 * @param[in]   delay_position delay time position type
 * @param[in]   delay_data delay data
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_SetDelayTime(enum timer30_id id, enum timer30_delay_time_position delay_position, uint16_t delay_data, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Set TIMER high impedance
 * @param[in]   id instance id
 * @param[in]   edge high-impedance edge selection
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_SetHIZ(enum timer30_id id, enum timer30_hi_z_edge_sel edge, uint8_t enable);

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
int32_t HAL_TIMER30_SetClock(enum timer30_id id, enum timer30_clk_sel clk_sel, uint16_t prescaler,
                           enum timer30_mcclk_sel mcclk_sel, uint8_t mcclk_div);

/**
 *******************************************************************************
 * @brief       Set TIMER interrupt
 * @param[in]   id instance id
 * @param[in]   type interrypt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER30_SetIRQ(enum timer30_id id, enum timer30_irq_type type, uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_TIMER30_H_ */

/** @} */
/** @} */

