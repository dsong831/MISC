/**
 *******************************************************************************
 * @file        a31t21x_uart.h
 * @brief       UART driver
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
 * @defgroup    driver_uart UART
 * @{
 * @brief       UART driver
 */

#ifndef _A31T21X_UART_H_
#define _A31T21X_UART_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"
#include "a31t21x_dma.h"


/**
 * UART instance id
 */
enum uart_id
{
    UART_ID_0   = 0,                        /**< UART 0 */
    UART_ID_1   = 1,                        /**< UART 1 */
    UART_ID_MAX = 2,                        /**< UART max */
};

/**
 * UART data bit
 */
enum uart_data_bit
{
    UART_DATA_BIT_5 = 0,                    /**< 5bit mode */
    UART_DATA_BIT_6 = 1,                    /**< 6bit mode */
    UART_DATA_BIT_7 = 2,                    /**< 7bit mode */
    UART_DATA_BIT_8 = 3,                    /**< 8bit mode */
};

/**
 * UART parity bit
 */
enum uart_parity_bit
{
    UART_PARITY_BIT_NONE    = 0,            /**< parity not used */
    UART_PARITY_BIT_ODD     = 1,            /**< odd parity used */
    UART_PARITY_BIT_EVEN    = 2,            /**< even parity used */
    UART_PARITY_BIT_STUCK   = 3,            /**< parity stuck used */
};

/**
 * UART stop bit
 */
enum uart_stop_bit
{
    UART_STOP_BIT_1 = 0,                    /**< 1bit stop */
    UART_STOP_BIT_2 = 1,                    /**< 2bit stop */
};

/**
 * UART line status
 */
enum uart_line_status
{
    UART_LINE_STATUS_TX_EMPTY       = (1 << 6),   /**< transmit data not set */
    UART_LINE_STATUS_TX_READY       = (1 << 5),   /**< ready for transmit */
    UART_LINE_STATUS_BREAK          = (1 << 4),   /**< a break condition detected */
    UART_LINE_STATUS_FRAME_ERROR    = (1 << 3),   /**< a frame error */
    UART_LINE_STATUS_PARITY_ERROR   = (1 << 2),   /**< a parity error */
    UART_LINE_STATUS_OVERRUN_ERROR  = (1 << 1),   /**< a overrun error */
    UART_LINE_STATUS_RX_DONE        = (1 << 0),   /**< data received */
};

/**
 * UART IO type
 */
enum uart_io_type
{
    UART_IO_TYPE_BLOCK      = 0,            /**< blocking */
    UART_IO_TYPE_NON_BLOCK  = 1,            /**< non-blocking */
};

/**
 * UART interrupt type
 */
enum uart_irq_type
{
    UART_IRQ_RX_AVAIL       = 0,            /**< reception available */
    UART_IRQ_TX_READY       = 1,            /**< transmission buffer empty */
    UART_IRQ_LINE_STATUS    = 2,            /**< line status error */
    UART_IRQ_TX_EXIT        = 3,            /**< transmission exit */
    UART_IRQ_RX_DMA_DONE    = 4,            /**< reception DMA complete */
    UART_IRQ_TX_DMA_DONE    = 5,            /**< transmission DMA complete */
};

/**
 * UART event
 */
enum uart_event
{
    UART_EVENT_TRANSMIT_DONE    = BIT(0),   /**< transmission is complete */
    UART_EVENT_RECEIVE_DONE     = BIT(1),   /**< reception is complete */
    UART_EVENT_LINE_ERROR       = BIT(2),   /**< line error */
    UART_EVENT_BREAK_ERROR      = BIT(3),   /**< break error */
    UART_EVENT_FRAME_ERROR      = BIT(4),   /**< frame error */
    UART_EVENT_PARITY_ERROR     = BIT(5),   /**< parity error */
    UART_EVENT_OVERFLOW_ERROR   = BIT(6),   /**< overflow error */
};

/**
 *******************************************************************************
 * @brief       UART interrupt callback function type
 * @param[in]   event event type ::uart_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*uart_irq_handler_t)(uint32_t event, void *context);

/**
 * UART configuration
 */
struct uart_cfg
{
    uint32_t                baudrate;   /**< baud rate */
    enum uart_data_bit      data_bit;   /**< data bit */
    enum uart_parity_bit    parity_bit; /**< parity bit */
    enum uart_stop_bit      stop_bit;   /**< stop bit */
    uint8_t                 irq_prio;   /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize UART
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_Init(enum uart_id id, struct uart_cfg *cfg,
                      uart_irq_handler_t handler,
                      void *context);

/**
 *******************************************************************************
 * @brief       Deintialize UART.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_Deinit(enum uart_id id);

/**
 *******************************************************************************
 * @brief       Transmit UART data.
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
int32_t HAL_UART_Transmit(enum uart_id id, uint8_t *out, uint32_t len,
                          enum uart_io_type io_type);

/**
 *******************************************************************************
 * @brief       Receive UART data.
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
int32_t HAL_UART_Receive(enum uart_id id, uint8_t *in, uint32_t len,
                         enum uart_io_type io_type);

/**
 *******************************************************************************
 * @brief       Abort UART transmission or receiption.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_Abort(enum uart_id id);

/**
 *******************************************************************************
 * @brief       DMA control for UART transmit.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_SetTransmitDMA(enum uart_id id, enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       DMA control for UART receive.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_SetReceiveDMA(enum uart_id id, enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Get UART busy status.
 * @param[in]   id instance id
 * @param[out]  busy busy status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_GetBusyStatus(enum uart_id id, uint8_t *busy);

/**
 *******************************************************************************
 * @brief       Get UART the line status
 * @param[in]   id instance id
 * @param[out]  status line status, see ::uart_line_status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_GetLineStatus(enum uart_id id, uint8_t *status);

/**
 *******************************************************************************
 * @brief       Set UART inter frame delay
 * @param[in]   id instance id
 * @param[in]   delay delay between each frame (0~7)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_SetIFDelay(enum uart_id id, uint8_t delay);

/**
 *******************************************************************************
 * @brief       Set UART interrupt
 *              Different UART interrupt can be enabled or disable.
 * @param[in]   id instance id
 * @param[in]   type interrupt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_UART_SetIRQ(enum uart_id id, enum uart_irq_type type, uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_UART_H_ */

/** @} */
/** @} */

