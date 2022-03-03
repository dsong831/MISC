/**
 *******************************************************************************
 * @file        a31t21x_usart1n.h
 * @brief       USART driver
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
 * @defgroup    driver_usart USART
 * @{
 * @brief       USART driver
 */

#ifndef _A31T21X_USART1n_H_
#define _A31T21X_USART1n_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"
#include "a31t21x_dma.h"


/**
 * USART instance id
 */
enum usart_id
{
    USART_ID_10   = 0,                        /**< USART 0 */
    USART_ID_11   = 1,                        /**< USART 1 */
    USART_ID_MAX 	= 2,                        /**< USART max */
};

/**
 * USART operation mode
 */
enum usart_mode
{
    USART_MODE_ASYNC  = 0,                    /**< Asynchronous Mode */
    USART_MODE_SYNC   = 1,                    /**< Synchronous Mode */
    USART_MODE_SPI 		= 3,                    /**< SPI Mode */
};

/**
 * USART data bit
 */
enum usart_data_bit
{
    USART_DATA_BIT_5 = 0,                    /**< 5bit mode */
    USART_DATA_BIT_6 = 1,                    /**< 6bit mode */
    USART_DATA_BIT_7 = 2,                    /**< 7bit mode */
    USART_DATA_BIT_8 = 3,                    /**< 8bit mode */
    USART_DATA_BIT_9 = 4,                    /**< 8bit mode */
};

/**
 * USART parity bit
 */
enum usart_parity_bit
{
    USART_PARITY_BIT_NONE    = 0,            /**< parity not used */
    USART_PARITY_BIT_EVEN    = 2,            /**< even parity used */
    USART_PARITY_BIT_ODD     = 3,            /**< odd parity used */

};

/**
 * USART stop bit
 */
enum usart_stop_bit
{
    USART_STOP_BIT_1 = 0,                    /**< 1bit stop */
    USART_STOP_BIT_2 = 1,                    /**< 2bit stop */
};

/**
 * USART bit order
 */
enum usart_bit_order
{
    USART_BIT_ORDER_MSB_FIRST = 0,            /**< use MSB first */
    USART_BIT_ORDER_LSB_FIRST = 1,            /**< use LSB first */
};

/**
 * USART sync mode
 */
enum usart_master_sel
{
    USART_SEL_MASTER = 0,      			         /**< master mode */
    USART_SEL_SLAVE  = 1,         			     /**< slave mode */
};

/**
 * USART clock and data sync mode
 */
enum usart_sync_mode
{
    USART_SYHC_MODE_CPOL_0_CPHA_0 = 0,        /**< polarity 0, phase 0 */
    USART_SYHC_MODE_CPOL_0_CPHA_1 = 1,        /**< polarity 0, phase 1 */
    USART_SYHC_MODE_CPOL_1_CPHA_0 = 2,        /**< polarity 1, phase 0 */
    USART_SYHC_MODE_CPOL_1_CPHA_1 = 3,        /**< polarity 1, phase 1 */
};

/**
 * USART slave select type
 */
enum usart_ss_type
{
    USART_SS_AUTO     = 0,                    /**< automatic slave select signal */
    USART_SS_MANUAL   = 1,                    /**< manual slave select signal */
};

/**
 * USART line status
 */
enum usart_line_status
{
    USART_ST_DRE       = ((uint8_t)(1 << 7)), 		/*!<Transmit Data Register Empty Interrut Flag */
    USART_ST_TXC       = ((uint8_t)(1 << 6)), 		/*!<Transmit Complete Interrupt Flag */
    USART_ST_RXC       = ((uint8_t)(1 << 5)), 		/*!<Receive Complete Interrupt Flag */
    USART_ST_WAKE      = ((uint8_t)(1 << 4)), 		/*!<Wake-up Interrupt Flag */
    USART_ST_DOR    	 = ((uint8_t)(1 << 2)), 		/*!<Data Overrun occur */
    USART_ST_FE  			 = ((uint8_t)(1 << 1)), 		/*!<Frame Error occur */
    USART_ST_PE 			 = ((uint8_t)(1 << 0)), 		/*!<Parity Error occur */
    USART_ST_BITMASK   = ((uint8_t)(0xFF)), 		/*!<USART Status Register Bit Mask */
};

/**
 * USART IO type
 */
enum usart_io_type
{
    USART_IO_TYPE_BLOCK      = 0,            /**< blocking */
    USART_IO_TYPE_NON_BLOCK  = 1,            /**< non-blocking */
};

/**
 * USART interrupt type
 */
enum usart_irq_type
{
    USART_IRQ_WAKEIE       = 2,            /**< Wkaeup Interrupt */
    USART_IRQ_RXCIE    	   = 3,            /**< Receive Complete Interrupt */
    USART_IRQ_TXCIE   		 = 4,            /**< Transmit Complete Interrupt */
    USART_IRQ_DRIE     	   = 5,            /**< Transmit Data Register Empty */
};

/**
 * USART event
 */
enum usart_event
{
    USART_EVENT_TRANSMIT_DONE    = BIT(0),   /**< transmission is complete */
    USART_EVENT_RECEIVE_DONE     = BIT(1),   /**< reception is complete */
    USART_EVENT_LINE_ERROR       = BIT(2),   /**< line error */
    USART_EVENT_BREAK_ERROR      = BIT(3),   /**< break error */
    USART_EVENT_FRAME_ERROR      = BIT(4),   /**< frame error */
    USART_EVENT_PARITY_ERROR     = BIT(5),   /**< parity error */
    USART_EVENT_OVERFLOW_ERROR   = BIT(6),   /**< overflow error */
};

/**
 *******************************************************************************
 * @brief       USART interrupt callback function type
 * @param[in]   event event type ::uart_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*usart_irq_handler_t)(uint32_t event, void *context);

/**
 * USART configuration
 */
struct usart_cfg
{
		enum usart_mode					mode;				/** mode */
    uint32_t                baudrate;   /**< baud rate */
    enum usart_data_bit     data_bit;   /**< data bit */
    enum usart_parity_bit   parity_bit; /**< parity bit */
    enum usart_stop_bit     stop_bit;   /**< stop bit */
		enum usart_bit_order		bit_order;	/**< bit order */
		enum usart_sync_mode		sync_mode;	/**< sync mode */
		enum usart_ss_type			ss_type;		/**< slave selection type */
		enum usart_master_sel		master_sel;	/**< master or slave selection */
    uint8_t                 irq_prio;   /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize USART
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_USART_Init(enum usart_id id, struct usart_cfg *cfg,
                      usart_irq_handler_t handler,
                      void *context);

/**
 *******************************************************************************
 * @brief       Deintialize USART.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_USART_Deinit(enum usart_id id);

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
int32_t HAL_USART_Transfer(enum usart_id id,
                         uint8_t *out, uint32_t out_len,
                         uint8_t *in, uint32_t in_len,
                         enum usart_io_type io_type);

/**
 *******************************************************************************
 * @brief       Transmit USART data.
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
int32_t HAL_USART_Transmit(enum usart_id id, uint8_t *out, uint32_t len,
                          enum usart_io_type io_type);

/**
 *******************************************************************************
 * @brief       Receive USART data.
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
int32_t HAL_USART_Receive(enum usart_id id, uint8_t *in, uint32_t len,
                         enum usart_io_type io_type);

/**
 *******************************************************************************
 * @brief       DMA control for USART transmit.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_USART_SetTransmitDMA(enum usart_id id, enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       DMA control for USART receive.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_USART_SetReceiveDMA(enum usart_id id, enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Get USART busy status.
 * @param[in]   id instance id
 * @param[out]  busy busy status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_USART_GetBusyStatus(enum usart_id id, uint8_t *busy);

/**
 *******************************************************************************
 * @brief       Get USART the line status
 * @param[in]   id instance id
 * @param[out]  status line status, see ::uart_line_status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_USART_GetLineStatus(enum usart_id id, uint8_t *status);

/**
 *******************************************************************************
 * @brief       Set USART interrupt
 *              Different USART interrupt can be enabled or disable.
 * @param[in]   id instance id
 * @param[in]   type interrupt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_USART_SetIRQ(enum usart_id id, enum usart_irq_type type, uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_USART1n_H_ */

/** @} */
/** @} */

