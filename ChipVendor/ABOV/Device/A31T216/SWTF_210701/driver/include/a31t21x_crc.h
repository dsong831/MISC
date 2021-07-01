/**
 *******************************************************************************
 * @file        a31t21x_crc.h
 * @brief       CRC driver
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
 * @defgroup    driver_crc CRC
 * @{
 * @brief       CRC driver
 */


#ifndef _A31T21X_CRC_H_
#define _A31T21X_CRC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_dma.h"
#include "a31t21x_driver_common.h"


/**
 * CRC Polinomial type
 */
enum crc_poly
{
    CRC_POLY_CCITT 		 = 0,                  /**< CRC-CCITT mode */
    CRC_POLY_CRC16 		 = 1,                  /**< CRC-16 mode */
    CRC_POLY_CHECKSUM  = 2,                  /**< Checksum mode */
};

/**
 * CRC Mode selection
 */
enum crc_mode
{
    CRC_USER_MODE  = 0,                 	   /**< User mode */
    CRC_AUTO_MODE  = 1,               	     /**< Auto mode */
};

/**
 * CRC Bit selection
 */
enum crc_bit
{
    CRC_MSB_FIRST  = 0,             	       /**< MSB 1st */
    CRC_LSB_FIRST  = 1,           	         /**< LSB 1st */
};

/**
 * CRC IO type
 */
enum crc_io_type
{
    CRC_IO_TYPE_BLOCK       = 0,						/**< blocking */
    CRC_IO_TYPE_NON_BLOCK   = 1,            /**< non-blocking */
};

/**
 * CRC event
 */
enum crc_event
{
    CRC_EVENT_DONE  = 0,                    /**< generation complete */
    CRC_EVENT_ERROR = 1,                    /**< generation error */
};

/**
 *******************************************************************************
 * @brief       CRC interrupt callback function type
 * @param[in]   event event type ::crc_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*crc_irq_handler_t)(uint32_t event, void *context);

/**
 * CRC configuration
 */
struct crc_cfg
{
		enum crc_poly			poly;							/**< polinomial */
		enum crc_mode 		mode;							/**< user/auto mode selection */
		enum crc_bit			bit;							/**< First shifted-in selection */
		enum crc_io_type	block;						/**< interrupt/polling selection */
    uint8_t     irq_prio;								/**< interrupt priority */
};


/**
 *******************************************************************************
 * @brief       Initialize CRC.
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_CRC_Init(struct crc_cfg *cfg, crc_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Deinitialize CRC.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_CRC_Deinit(void);

/**
 *******************************************************************************
 * @brief       Enable DMA for CRC.
 * @param[in]   ch DMA channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_CRC_SetDMA(enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Compute CRC.
 *              If DMA is enabled before calling this function, the result
 *              value is available when DONE event is signalled.
 *              Otherwise, the function waits until the result is available.
 * @param[in]   init initial value
 * @param[in]   data address of the data
 * @param[in]   len length of the data
 * @param[out]  value value of the computed CRC
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_CRC_Compute(uint32_t init, uint32_t *data, uint32_t len, uint32_t *value);

/**
 *******************************************************************************
 * @brief       Get CRC Data.
 * @param[out]  value value of the computed CRC
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_CRC_GetData(uint32_t *value);


/**
 *******************************************************************************
 * @brief       Generate SW CRC.
 * @param[in]   init initial value
 * @param[in]   data address of the data
 * @param[in]   len length of the data
 * @param[in]   Polinomial type
 * @param[in]   Bit selection
 * @return      result SW CRC
 ******************************************************************************/
uint16_t HAL_CRC_SW_Generate(uint16_t crc_init, uint32_t *data, uint32_t len, uint8_t crc_poly, uint8_t bit);


#ifdef __cplusplus
}
#endif


#endif /* _A31T21X_CRC_H_ */

/** @} */
/** @} */

