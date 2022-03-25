/**
 *******************************************************************************
 * @file        a31t21x_debug.c
 * @brief       Application common
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include <stdint.h>
#include <stddef.h>

#include "a31t21x_system.h"
#include "a31t21x_gpio.h"
#include "a31t21x_uart.h"

#include "debug_serial.h"
#include "debug_log.h"
#include "main.h"

#define DEBUG_UART_ID   UART_ID_0

static void dbg_uart_irq_handler(uint32_t event, void *context)
{
    if (event & UART_EVENT_RECEIVE_DONE)
    {
        serial_continue_rx();
    }

    if (event & UART_EVENT_TRANSMIT_DONE)
    {
        serial_continue_tx();
    }
}

static void dbg_serial_init(void)
{
    struct uart_cfg cfg =
    {
        .baudrate = APP_UART_BAUD,
        .data_bit = UART_DATA_BIT_8,
        .parity_bit = UART_PARITY_BIT_NONE,
        .stop_bit = UART_STOP_BIT_1,
        .irq_prio = DRIVER_PRIORITY_LOWEST,
    };

    HAL_UART_Init(DEBUG_UART_ID, &cfg, dbg_uart_irq_handler, NULL);
}

static void dbg_serial_abort(void)
{
    HAL_UART_Abort(DEBUG_UART_ID);
}

static void dbg_serial_rx(uint8_t *ch)
{
    HAL_UART_Receive(DEBUG_UART_ID, ch, 1, UART_IO_TYPE_NON_BLOCK);
}

static void dbg_serial_rx_poll(uint8_t *ch)
{
    HAL_UART_Receive(DEBUG_UART_ID, ch, 1, UART_IO_TYPE_BLOCK);
}

static void dbg_serial_tx(uint8_t *ch)
{
    HAL_UART_Transmit(DEBUG_UART_ID, ch, 1, UART_IO_TYPE_NON_BLOCK);
}

static void dbg_serial_tx_poll(uint8_t *ch)
{
    HAL_UART_Transmit(DEBUG_UART_ID, ch, 1, UART_IO_TYPE_BLOCK);
}

struct serial_op s_op =
{
    .init = dbg_serial_init,
    .abort = dbg_serial_abort,
    .rx = dbg_serial_rx,
    .rx_poll = dbg_serial_rx_poll,
    .tx = dbg_serial_tx,
    .tx_poll = dbg_serial_tx_poll,
};

void Debug_Init(void)
{
    /* Configure UART TXD0 pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN4, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTB, PIN4, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTB, PIN4, GPIO_PULL_UP);

    /* Configure UART RXD0 pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN5, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTB, PIN5, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTB, PIN5, GPIO_PULL_UP);

    serial_init(&s_op);
}

void Assert(char *file, uint32_t line)
{
    LOG("ASSERT %s at %d\n", file, line);
    while (1)
    {
    }
}

