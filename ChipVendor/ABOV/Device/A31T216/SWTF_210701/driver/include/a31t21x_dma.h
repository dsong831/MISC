/**
 *******************************************************************************
 * @file        a31t21x_dma.h
 * @brief       DMA driver
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
 * @defgroup    driver_dma DMA
 * @{
 * @brief       DMA driver
 */


#ifndef _A31T21X_DMA_H_
#define _A31T21X_DMA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"

/**
 * DMA id
 */
enum dma_ch
{
    DMA_ID_0    = 0,                        /**< DMA 0 */
    DMA_ID_1    = 1,                        /**< DMA 1 */
    DMA_ID_2    = 2,                        /**< DMA 2 */
    DMA_ID_3    = 3,                        /**< DMA 3 */
    DMA_ID_MAX  = 4,               	        /**< DMA max */
};

/**
 * DMA peripheral id
 */
enum dma_peri
{
    DMA_PERI_NONE       = 0,                /**< peripheral id none */
    DMA_PERI_UART0_RX   = 1,                /**< peripheral id UART0 RX */
    DMA_PERI_UART0_TX   = 2,                /**< peripheral id UART0 TX */
    DMA_PERI_UART1_RX   = 3,                /**< peripheral id UART1 RX */
    DMA_PERI_UART1_TX   = 4,                /**< peripheral id UART1 TX */
    DMA_PERI_USART10_RX = 5,                /**< peripheral id USART10 RX */
    DMA_PERI_USART10_TX = 6,                /**< peripheral id USART10 TX */
    DMA_PERI_USART11_RX = 7,                /**< peripheral id USART11 RX */
    DMA_PERI_USART11_TX = 8,                /**< peripheral id USART11 TX */
    DMA_PERI_SPI20_RX   = 9,								/**< peripheral id SPI20 RX */
    DMA_PERI_SPI20_TX   = 10,               /**< peripheral id SPI20 TX */
    DMA_PERI_SPI21_RX   = 11,               /**< peripheral id SPI21 RX */
    DMA_PERI_SPI21_TX   = 12,               /**< peripheral id SPI21 TX */
    DMA_PERI_CRC_TX     = 13,               /**< peripheral id CRC TX */
};

/**
 * DMA source transfer width and destination transfer width
 */
enum dma_bus_size
{
    DMA_BUS_SIZE_BYTE       = (0UL),        /**< size = 1 byte */
    DMA_BUS_SIZE_HALFWORD   = (1UL),        /**< size = 2 bytes */
    DMA_BUS_SIZE_WORD       = (2UL),        /**< size = 4 bytes */
};

/**
 * DMA address direction
 */
enum dma_dir
{
    DMA_DIR_MEM_TO_PERI = 0,                /**< memory to peripheral */
    DMA_DIR_PERI_TO_MEM = 1,                /**< peripheral to memory */
};

/**
 * DMA event
 */
enum dma_event
{
    DMA_EVENT_DONE  = 0,                    /**< generation complete */
    DMA_EVENT_ERROR = 1,                    /**< generation error */
};

/**
 * DMA configuration
 */

struct dma_cfg
{
    enum dma_peri       peri;               /**< peripheral id */
    enum dma_bus_size   size;               /**< bus size */
    enum dma_dir        dir;                /**< direction */
};


/**
 *******************************************************************************
 * @brief       Initialize DMA.
 *              DMA may be used by multiple peripherals, and initialization
 *              will enable DMA only once for the first time.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Init(void);

/**
 *******************************************************************************
 * @brief       Deinitialize DMA.
 *              DMA may be used by multiple peripherals, and deinitialzation
 *              will disable DMA only if no other peripherals are in use of DMA.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Deinit(void);

/**
 *******************************************************************************
 * @brief       Setup DMA channel.
 * @param[in]   ch DMA channel
 * @param[in]   cfg configuration
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Setup(enum dma_ch ch, struct dma_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Clear DMA channel.
 * @param[in]   ch DMA channel
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Clear(enum dma_ch ch);

/**
 *******************************************************************************
 * @brief       Start DMA.
 * @param[in]   ch DMA channel
 * @param[in]   src source address for the DMA transfer
 * @param[in]   dst destination address for the DMA transfer
 * @param[in]   len length of the DMA transfer, should be less than 0xFFF
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Start(enum dma_ch ch, uint32_t src, uint32_t dst, uint32_t len);

/**
 *******************************************************************************
 * @brief       Stop DMA channel.
 * @param[in]   ch DMA channel
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Stop(enum dma_ch id);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_DMA_H_ */

/** @} */
/** @} */

