/**
 *******************************************************************************
 * @file        debug_serial.h
 * @brief       Debugging serial
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
 * @addtogroup  library
 * @{
 * @defgroup    debug_serial Serial
 * @{
 * @brief       Debugging with serial
 */

#ifndef _DEBUG_SERIAL_H_
#define _DEBUG_SERIAL_H_

#include <stdint.h>
#include <string.h>


typedef void (*serial_op_init_t)(void);             /**< prototype for debug initialization function */
typedef void (*serial_op_abort_t)(void);            /**< prototype for debug abort function */
typedef void (*serial_op_rx_t)(uint8_t *ch);        /**< prototype for debug rx non-blocking function */
typedef void (*serial_op_rx_poll_t)(uint8_t *ch);   /**< prototype for debug rx blocking function */
typedef void (*serial_op_tx_t)(uint8_t *ch);        /**< prototype for debug tx non-blocking function */
typedef void (*serial_op_tx_poll_t)(uint8_t *ch);   /**< prototype for debug tx blocking function */

/**
 * Serial operation for debugging
 */
struct serial_op
{
    serial_op_init_t    init;           /**< initialization function */
    serial_op_abort_t   abort;          /**< abort function */
    serial_op_rx_t      rx;             /**< rx non-blocking function */
    serial_op_rx_poll_t rx_poll;        /**< rx blocking function */
    serial_op_tx_t      tx;             /**< tx non-blocking function */
    serial_op_tx_poll_t tx_poll;        /**< tx blocking function */
};

#if (CONFIG_DEBUG)

/**
 *******************************************************************************
 * @brief
 ******************************************************************************/

/**
 *******************************************************************************
 * @brief       This function initializes the serial port with fifo
 * @param[in]   op serial operation
 * @return      a non-zero on error, otherwise 0
 ******************************************************************************/
int serial_init(struct serial_op *op);

/**
 *******************************************************************************
 * @brief       This function puts a character to the fifo, which will be
 *              transmitted automatically
 * @param[in]   c character to put
 * @return      character written as int type
 ******************************************************************************/
int serial_putc(char c);

/**
 *******************************************************************************
 * @brief       This function gets a character from the fifo.
 *              If receive callback is specified, the function returns EOF always,
 *              and callback is invoked when a character is received.
 * @param[in]   rx_cb receive callback function.
 * @return      character received or EOF
 ******************************************************************************/
int serial_getc(void (*rx_cb)(uint8_t));

/**
 *******************************************************************************
 * @brief       This function continues transmission
 * @return      none
 ******************************************************************************/
void serial_continue_tx(void);

/**
 *******************************************************************************
 * @brief       This function continues reception
 * @return      none
 ******************************************************************************/
void serial_continue_rx(void);

#else

#define serial_init(...)
#define serial_putc(...)
#define serial_getc(...)
#define serial_continue_tx(...)
#define serial_continue_rx(...)

#endif

#endif /* _DEBUG_SERIAL_H_ */

/** @} */
/** @} */

