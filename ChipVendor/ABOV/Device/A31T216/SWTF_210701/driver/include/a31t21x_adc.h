/**
 *******************************************************************************
 * @file        a31t21x_adc.h
 * @brief       ADC driver
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
 * @defgroup    driver_adc ADC
 * @{
 * @brief       ADC driver
 */

#ifndef _A31T21X_ADC_H_
#define _A31T21X_ADC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"


/** ADC Ready time-out definitions in case of using function
 * 
 */
#define ADC_READY_TIMEOUT			(0x100UL)	

/**
 * ADC Trigger id
 */
enum adc_trigger_id
{
    ADC_TRIGGER_ADST 							  = 0,			/**< Trigger Signal ADST */
		ADC_TRIGGER_TIMER_ID_10					= 1,			/**< Trigger Signal Timer10 A-match */
		ADC_TRIGGER_TIMER_ID_11					= 2,			/**< Trigger Signal Timer11 A-match */
		ADC_TRIGGER_TIMER_ID_12					= 3,			/**< Trigger Signal Timer12 A-match */
		ADC_TRIGGER_TIMER_ID_30					= 4,			/**< Trigger Signal Timer30 A-match */
};

/**
 * ADC IO type
 */
enum adc_io_type
{
    ADC_IO_TYPE_BLOCK      = 0,             /**< blocking */
    ADC_IO_TYPE_NON_BLOCK  = 1,             /**< non-blocking */
};

/**
 * ADC reference selection
 */
enum adc_ref_sel
{
    ADC_REF_VDD		= 0,            	        /**< Select analog power(VDD) */
    ADC_REF_AVREF	= 1,											/**< Select external reference(AVREF) */
};

/**
 * ADC event
 */
enum adc_event
{
    ADC_EVENT_SINGLE    = BIT(0),           /**< ADC single conversion event*/
    ADC_EVENT_TRIGGER   = BIT(1),           /**< ADC trigger conversion event */};

/**
 * ADC configuration
 */
struct adc_cfg
{
		enum adc_trigger_id			trigger;				/**< trigger signal selection */
		enum adc_ref_sel				reference;			/**< reference selection */
    uint8_t                	prescaler;  	  /**< converter prescaler */
    uint8_t                 irq_prio;       /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       ADC interrupt callback function type
 * @param[in]   event event type
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*adc_irq_handler_t)(uint32_t event, void *context);

/**
 *******************************************************************************
 * @brief       Initialize ADC.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_Init(struct adc_cfg *cfg, adc_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Deintialize ADC.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_DeInit(void);

/**
 *******************************************************************************
 * @brief       Get ADC data.
 * @param[in]   id instance id
 * @param[out]  data data
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_GetData(uint16_t *data);

/**
 *******************************************************************************
 * @brief       Start ADC single mode
 * @param[in]   id instance id
 * @param[out]   data pointer to the data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_Start(uint16_t *data, enum adc_io_type io_type);

/**
 *******************************************************************************
 * @brief       Stop ADC
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_Stop(void);

/**
 *******************************************************************************
 * @brief       Select ADC channel number
 * @param[in]   id instance id
 * @param[in]   seq id seuence ID
 * @param[in]   channel channel number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SelectChannel(uint8_t channel);

/**
 *******************************************************************************
 * @brief       Get ADC channel status
 * @param[in]   id instance id
 * @param[in]   status status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_GetStatus(uint32_t *status);

/**
 *******************************************************************************
 * @brief       Clear ADC channel status
 * @param[in]   id instance id
 * @param[in]   status status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_ClearStatus(uint32_t status);

/**
 *******************************************************************************
 * @brief       Enable ADC interrupt
 * @param[in]   id instance id
 * @param[in]   type interrupt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SetIRQ(uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_ADC_H_ */

/** @} */
/** @} */

