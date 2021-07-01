/**
 *******************************************************************************
 * @file        a31t21x_spi.h
 * @brief       SPI driver
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
 * @defgroup    driver_spi SPI
 * @{
 * @brief       SPI driver
 */

#ifndef _A31T21X_SPI_H_
#define _A31T21X_SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"
#include "a31t21x_dma.h"


#define SPI_CR_TXBC_Pos                   (20UL)                    /*!< SPI CR: TXBC (Bit 20)                                 */
#define SPI_CR_TXBC_Msk                   (0x100000UL)              /*!< SPI CR: TXBC (Bitfield-Mask: 0x01)                    */
#define SPI_CR_RXBC_Pos                   (19UL)                    /*!< SPI CR: RXBC (Bit 19)                                 */
#define SPI_CR_RXBC_Msk                   (0x80000UL)               /*!< SPI CR: RXBC (Bitfield-Mask: 0x01)                    */
#define SPI_CR_DTXIE_Pos                  (18UL)                    /*!< SPI CR: DTXIE (Bit 18)                                */
#define SPI_CR_DTXIE_Msk                  (0x40000UL)               /*!< SPI CR: DTXIE (Bitfield-Mask: 0x01)                   */
#define SPI_CR_DRXIE_Pos                  (17UL)                    /*!< SPI CR: DRXIE (Bit 17)                                */
#define SPI_CR_DRXIE_Msk                  (0x20000UL)               /*!< SPI CR: DRXIE (Bitfield-Mask: 0x01)                   */
#define SPI_CR_SSCIE_Pos                  (16UL)                    /*!< SPI CR: SSCIE (Bit 16)                                */
#define SPI_CR_SSCIE_Msk                  (0x10000UL)               /*!< SPI CR: SSCIE (Bitfield-Mask: 0x01)                   */
#define SPI_CR_TXIE_Pos                   (15UL)                    /*!< SPI CR: TXIE (Bit 15)                                 */
#define SPI_CR_TXIE_Msk                   (0x8000UL)                /*!< SPI CR: TXIE (Bitfield-Mask: 0x01)                    */
#define SPI_CR_RXIE_Pos                   (14UL)                    /*!< SPI CR: RXIE (Bit 14)                                 */
#define SPI_CR_RXIE_Msk                   (0x4000UL)                /*!< SPI CR: RXIE (Bitfield-Mask: 0x01)                    */
#define SPI_CR_SSMOD_Pos                  (13UL)                    /*!< SPI CR: SSMOD (Bit 13)                                */
#define SPI_CR_SSMOD_Msk                  (0x2000UL)                /*!< SPI CR: SSMOD (Bitfield-Mask: 0x01)                   */
#define SPI_CR_SSOUT_Pos                  (12UL)                    /*!< SPI CR: SSOUT (Bit 12)                                */
#define SPI_CR_SSOUT_Msk                  (0x1000UL)                /*!< SPI CR: SSOUT (Bitfield-Mask: 0x01)                   */
#define SPI_CR_LBE_Pos                    (11UL)                    /*!< SPI CR: LBE (Bit 11)                                  */
#define SPI_CR_LBE_Msk                    (0x800UL)                 /*!< SPI CR: LBE (Bitfield-Mask: 0x01)                     */
#define SPI_CR_SSMASK_Pos                 (10UL)                    /*!< SPI CR: SSMASK (Bit 10)                               */
#define SPI_CR_SSMASK_Msk                 (0x400UL)                 /*!< SPI CR: SSMASK (Bitfield-Mask: 0x01)                  */
#define SPI_CR_SSMO_Pos                   (9UL)                     /*!< SPI CR: SSMO (Bit 9)                                  */
#define SPI_CR_SSMO_Msk                   (0x200UL)                 /*!< SPI CR: SSMO (Bitfield-Mask: 0x01)                    */
#define SPI_CR_SSPOL_Pos                  (8UL)                     /*!< SPI CR: SSPOL (Bit 8)                                 */
#define SPI_CR_SSPOL_Msk                  (0x100UL)                 /*!< SPI CR: SSPOL (Bitfield-Mask: 0x01)                   */
#define SPI_CR_MS_Pos                     (5UL)                     /*!< SPI CR: MS (Bit 5)                                    */
#define SPI_CR_MS_Msk                     (0x20UL)                  /*!< SPI CR: MS (Bitfield-Mask: 0x01)                      */
#define SPI_CR_MSBF_Pos                   (4UL)                     /*!< SPI CR: MSBF (Bit 4)                                  */
#define SPI_CR_MSBF_Msk                   (0x10UL)                  /*!< SPI CR: MSBF (Bitfield-Mask: 0x01)                    */
#define SPI_CR_CPHA_Pos                   (3UL)                     /*!< SPI CR: CPHA (Bit 3)                                  */
#define SPI_CR_CPHA_Msk                   (0x8UL)                   /*!< SPI CR: CPHA (Bitfield-Mask: 0x01)                    */
#define SPI_CR_CPOL_Pos                   (2UL)                     /*!< SPI CR: CPOL (Bit 2)                                  */
#define SPI_CR_CPOL_Msk                   (0x4UL)                   /*!< SPI CR: CPOL (Bitfield-Mask: 0x01)                    */
#define SPI_CR_BITSZ_Pos                  (0UL)                     /*!< SPI CR: BITSZ (Bit 0)                                 */
#define SPI_CR_BITSZ_Msk                  (0x3UL)                   /*!< SPI CR: BITSZ (Bitfield-Mask: 0x03)                   */

#define SPI_SR_TXDMAF_Pos                 (9UL)                     /*!< SPI SR: TXDMAF (Bit 9)                                */
#define SPI_SR_TXDMAF_Msk                 (0x200UL)                 /*!< SPI SR: TXDMAF (Bitfield-Mask: 0x01)                  */
#define SPI_SR_RXDMAF_Pos                 (8UL)                     /*!< SPI SR: RXDMAF (Bit 8)                                */
#define SPI_SR_RXDMAF_Msk                 (0x100UL)                 /*!< SPI SR: RXDMAF (Bitfield-Mask: 0x01)                  */
#define SPI_SR_SBUSY_Pos                  (7UL)                     /*!< SPI SR: SBUSY (Bit 7)                                 */
#define SPI_SR_SBUSY_Msk                  (0x80UL)                  /*!< SPI SR: SBUSY (Bitfield-Mask: 0x01)                   */
#define SPI_SR_SSDET_Pos                  (6UL)                     /*!< SPI SR: SSDET (Bit 6)                                 */
#define SPI_SR_SSDET_Msk                  (0x40UL)                  /*!< SPI SR: SSDET (Bitfield-Mask: 0x01)                   */
#define SPI_SR_SSON_Pos                   (5UL)                     /*!< SPI SR: SSON (Bit 5)                                  */
#define SPI_SR_SSON_Msk                   (0x20UL)                  /*!< SPI SR: SSON (Bitfield-Mask: 0x01)                    */
#define SPI_SR_OVRF_Pos                   (4UL)                     /*!< SPI SR: OVRF (Bit 4)                                  */
#define SPI_SR_OVRF_Msk                   (0x10UL)                  /*!< SPI SR: OVRF (Bitfield-Mask: 0x01)                    */
#define SPI_SR_UDRF_Pos                   (3UL)                     /*!< SPI SR: UDRF (Bit 3)                                  */
#define SPI_SR_UDRF_Msk                   (0x8UL)                   /*!< SPI SR: UDRF (Bitfield-Mask: 0x01)                    */
#define SPI_SR_TXIDLE_Pos                 (2UL)                     /*!< SPI SR: TXIDLE (Bit 2)                                */
#define SPI_SR_TXIDLE_Msk                 (0x4UL)                   /*!< SPI SR: TXIDLE (Bitfield-Mask: 0x01)                  */
#define SPI_SR_TRDY_Pos                   (1UL)                     /*!< SPI SR: TRDY (Bit 1)                                  */
#define SPI_SR_TRDY_Msk                   (0x2UL)                   /*!< SPI SR: TRDY (Bitfield-Mask: 0x01)                    */
#define SPI_SR_RRDY_Pos                   (0UL)                     /*!< SPI SR: RRDY (Bit 0)                                  */
#define SPI_SR_RRDY_Msk                   (0x1UL)                   /*!< SPI SR: RRDY (Bitfield-Mask: 0x01)                    */


/**
 * SPI instance id
 */
enum spi_id
{
    SPI_ID_20    = 0,												/**< SPI 20 */
    SPI_ID_21    = 1,												/**< SPI 21 */
    SPI_ID_MAX 	 = 2,												/**< SPI max */
};

/**
 * SPI master clock frequency
 */
enum spi_freq
{
    SPI_FREQ_4MHZ,										/**< frequency 4 MHz */
    SPI_FREQ_2MHZ,										/**< frequency 2 MHz */
    SPI_FREQ_1MHZ,										/**< frequency 1 MHz */
    SPI_FREQ_500KHZ,									/**< frequency 500 KHz */
    SPI_FREQ_250KHZ,									/**< frequency 250 KHz */
    SPI_FREQ_125KHZ,									/**< frequency 125 KHz */
};

/**
 * SPI bit order
 */
enum spi_bit_order
{
    SPI_BIT_ORDER_MSB_FIRST = 0,            /**< use MSB first */
    SPI_BIT_ORDER_LSB_FIRST = 1,            /**< use LSB first */
};

/**
 * SPI mode
 */
enum spi_mode
{
    SPI_MODE_MASTER = 0,                    /**< master mode */
    SPI_MODE_SLAVE  = 1,                    /**< slave mode */
};

/**
 * SPI clock and data sync mode
 */
enum spi_sync_mode
{
    SPI_SYHC_MODE_CPOL_0_CPHA_0 = 0,        /**< polarity 0, phase 0 */
    SPI_SYHC_MODE_CPOL_0_CPHA_1 = 1,        /**< polarity 0, phase 1 */
    SPI_SYHC_MODE_CPOL_1_CPHA_0 = 2,        /**< polarity 1, phase 0 */
    SPI_SYHC_MODE_CPOL_1_CPHA_1 = 3,        /**< polarity 1, phase 1 */
};

/**
 * SPI data bit
 */
enum spi_data_bit
{
    SPI_DATA_BIT_8  = 0,                    /**< 8 bit mode */
    SPI_DATA_BIT_9  = 1,                    /**< 9 bit mode */
    SPI_DATA_BIT_16 = 2,                    /**< 16 bit mode */
    SPI_DATA_BIT_17 = 3,                    /**< 17 bit mode */
};

/**
 * SPI slave select type
 */
enum spi_ss_type
{
    SPI_SS_AUTO     = 0,                    /**< automatic slave select signal */
    SPI_SS_MANUAL   = 1,                    /**< manual slave select signal */
};

/**
 * SPI slave select state
 */
enum ss_state
{
    SPI_SS_STATE_LOW    = 0,                /**< slave select low */
    SPI_SS_STATE_HIGH   = 1,                /**< slave select high */
};

/**
 * SPI IO type
 */
enum spi_io_type
{
    SPI_IO_TYPE_BLOCK       = 0,            /**< blocking */
    SPI_IO_TYPE_NON_BLOCK   = 1,            /**< non blocking */
};

/**
 * SPI event
 */
enum spi_event
{
    SPI_EVENT_TRANSFER_DONE			= BIT(0),       /**< transfer complete */
    SPI_EVENT_OVERRUN						= BIT(1),       /**< overrun error */
    SPI_EVENT_UNDERRUN					= BIT(2),       /**< underrun error */
    SPI_EVENT_DMA_TRANSFER_DONE	= BIT(3),       /**< dma transfer complete */
};

/**
 *******************************************************************************
 * @brief       SPI interrupt callback function type
 * @param[in]   event event type ::spi_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*spi_irq_handler_t)(uint32_t event, void *context);

/**
 * SPI configuration
 */
struct spi_cfg
{
    enum spi_mode       mode;               /**< mode */
    uint32_t 			      freq;               /**< frequency */
    enum spi_bit_order  bit_order;          /**< bit oder */
    enum spi_sync_mode  sync_mode;          /**< sync mode */
    enum spi_data_bit   data_bit;           /**< data bit */
    enum spi_ss_type    ss_type;            /**< slave selection type */
    uint8_t             irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_Init(enum spi_id id, struct spi_cfg *cfg,
                     spi_irq_handler_t handler,
                     void *context);

/**
 *******************************************************************************
 * @brief       Deintialize.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_Deinit(enum spi_id id);

/**
 *******************************************************************************
 * @brief		Sonfigure SPI delay
 *              For operation using flash memory data, it is recommended to
 *              use some delay, epecially for high speed SPI clock configuration.
 * @param[in]   id instance id
 * @param[in]	start start delay
 * @param[in]	burst burst delay
 * @param[in]	stop stop delay
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SetDelay(enum spi_id id, uint8_t start, uint8_t burst, uint8_t stop);

/**
 *******************************************************************************
 * @brief		Control SS Output in SPI peripheral
 * @param[in]   id instance id
 * @param[in]   ss slave selection state
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SSManual(enum spi_id id, enum ss_state ss);

/**
 *******************************************************************************
 * @brief       Trasnfer data.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is received.
 * @param[in]   id instance id
 * @param[in]   out data to transmit from
 * @param[in]   out_len length of data
 * @param[out]  in data to receive to
 * @param[in]   in_len len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_Transfer(enum spi_id id,
                         uint8_t *out, uint32_t out_len,
                         uint8_t *in, uint32_t in_len,
                         enum spi_io_type io_type);

/**
 *******************************************************************************
 * @brief       Transmit data.
 *              Data should not be changed until trasmittion is complete.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is transmitted.
 * @param[in]   id instance id
 * @param[in]   out data to transmit from
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
static __INLINE int32_t HAL_SPI_Transmit(enum spi_id id, uint8_t *out, uint32_t len,
                                         enum spi_io_type io_type)
{
    return HAL_SPI_Transfer(id, out, len, NULL, 0, io_type);
}

/**
 *******************************************************************************
 * @brief       Receive data.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is received.
 * @param[in]   id instance id
 * @param[out]  in data to receive to
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
static __INLINE int32_t HAL_SPI_Receive(enum spi_id id, uint8_t *in, uint32_t len,
                                        enum spi_io_type io_type)
{
    return HAL_SPI_Transfer(id, NULL, 0, in, len, io_type);
}

/**
 *******************************************************************************
 * @brief       DMA control for SPI transmit.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SetTransmitDMA(enum spi_id id, enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       DMA control for SPI receive.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SetReceiveDMA(enum spi_id id, enum dma_ch ch, uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_SPI_H_ */

/** @} */
/** @} */

