/**
 *******************************************************************************
 * @file        debug_serial.c
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


#if (CONFIG_DEBUG == 1)

#include <stdio.h>
#include <stdlib.h>
#include <cmsis_compiler.h>

#include "debug_serial.h"


static uint8_t serial_tx;
static uint8_t serial_rx;
static void (*serial_rx_cb)(uint8_t);

static struct serial_op *s_op;

#if (CONFIG_DEBUG_SERIAL_FIFO == 1)

#if !defined(CONFIG_DEBUG_SERIAL_RX_FIFO_SIZE)
#define CONFIG_DEBUG_SERIAL_RX_FIFO_SIZE    16
#endif
#if !defined(CONFIG_DEBUG_SERIAL_TX_FIFO_SIZE)
#define CONFIG_DEBUG_SERIAL_TX_FIFO_SIZE    512
#endif

struct serial_fifo
{
    uint8_t *           data;
    uint16_t            mask;
    volatile uint8_t    active;
    volatile uint32_t   in_pos;
    volatile uint32_t   out_pos;
};

static uint8_t rx_fifo_buf[CONFIG_DEBUG_SERIAL_RX_FIFO_SIZE];
static uint8_t tx_fifo_buf[CONFIG_DEBUG_SERIAL_TX_FIFO_SIZE];
static struct serial_fifo rx_fifo;
static struct serial_fifo tx_fifo;


static void serial_fifo_init(struct serial_fifo *fifo, uint8_t *data, uint32_t len)
{
    fifo->data = data;
    fifo->active = 0;
    fifo->mask = len - 1;
    fifo->in_pos = 0;
    fifo->out_pos = 0;
}

/* check if fifo is empty */
static __INLINE uint8_t serial_fifo_empty(struct serial_fifo *fifo)
{
    uint32_t in_pos;
    uint32_t out_pos;
    
    in_pos = fifo->in_pos;
    out_pos = fifo->out_pos;
    
    return (in_pos == out_pos) ? 1 : 0;
}

/* check if fifo is not full, and have availabe space */
static __INLINE uint8_t serial_fifo_avail(struct serial_fifo *fifo)
{
   uint32_t in_pos;
   uint32_t out_pos;  
   uint16_t mask;
   
   in_pos = fifo->in_pos;
   out_pos = fifo->out_pos;
   mask = fifo->mask;
   
  return ((in_pos - out_pos) < mask) ? 1 : 0;
}

/* check fifo is active */
static __INLINE uint8_t serial_fifo_is_active(struct serial_fifo *fifo)
{
    return fifo->active;
}

/* set fifo active */
static __INLINE void serial_fifo_set_active(struct serial_fifo *fifo, uint8_t active)
{
    fifo->active = active;
}

/* put fifo: check for fifo available before call to this function */
static __INLINE void serial_fifo_put(struct serial_fifo *fifo, uint8_t c)
{
    fifo->data[fifo->in_pos & fifo->mask] = c;
    fifo->in_pos++;
}

/* get fifo: check for fifo not empty before call to this function */
static __INLINE void serial_fifo_get(struct serial_fifo *fifo, uint8_t *c)
{
    *c = fifo->data[fifo->out_pos & fifo->mask];
    fifo->out_pos++;
}

#endif

void serial_continue_rx(void)
{
    /* echo the character */
    if (serial_rx == '\r')
    {
        serial_putc('\n');
    }
    else
    {
        serial_putc(serial_rx);
    }

    if (serial_rx_cb)
    {
        serial_rx_cb(serial_rx);
    }
    else
    {
#if (CONFIG_DEBUG_SERIAL_FIFO == 1)
        serial_fifo_put(&rx_fifo, serial_rx);
        s_op->rx(&serial_rx);
#endif
    }
}

void serial_continue_tx(void)
{
#if (CONFIG_DEBUG_SERIAL_FIFO == 1)
    if (serial_fifo_empty(&tx_fifo))
    {
        serial_fifo_set_active(&tx_fifo, 0);
    }
    else
    {
        serial_fifo_get(&tx_fifo, &serial_tx);
        s_op->tx(&serial_tx);
    }
#endif
}

int serial_init(struct serial_op *op)
{
    s_op = op;

    s_op->init();

#if (CONFIG_DEBUG_SERIAL_FIFO == 1)
    serial_fifo_init(&rx_fifo, rx_fifo_buf, sizeof(rx_fifo_buf));
    serial_fifo_init(&tx_fifo, tx_fifo_buf, sizeof(tx_fifo_buf));
#endif

#if (__GNUC__)
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    return 0;
}

int serial_putc(char c)
{
    /* automatically convert LF to CRLF */
    if (c == '\n')
    {
        serial_putc('\r');
    }

#if (CONFIG_DEBUG_SERIAL_FIFO == 1)

    __disable_irq();

    if (serial_fifo_is_active(&tx_fifo) == 0)
    {
        /* send directly if fifo is not active */
        serial_fifo_set_active(&tx_fifo, 1);
        serial_tx = c;
        s_op->tx(&serial_tx);
    }
    else
    {
        /* add to fifo if it is active */
        if (serial_fifo_avail(&tx_fifo))
        {
            /* if fifo has more space available */
            serial_fifo_put(&tx_fifo, c);
        }
    }

    __enable_irq();

#else
    serial_tx = c;
    s_op->tx_poll(&serial_tx);
#endif

    return c;
}

int serial_getc(void (*rx_cb)(uint8_t))
{
    serial_rx_cb = rx_cb;

    if (serial_rx_cb)
    {
        /* use callback if it is set */
        s_op->rx(&serial_rx);
    }
    else
    {
#if (CONFIG_DEBUG_SERIAL_FIFO == 1)
        /* callback is not set, return synchronously */
        if (serial_fifo_is_active(&rx_fifo) == 0)
        {
            serial_fifo_set_active(&rx_fifo, 1);
            s_op->rx(&serial_rx);
        }

        if (serial_fifo_empty(&rx_fifo) == 0)
        {
            uint8_t c;
            serial_fifo_get(&rx_fifo, &c);
            return c;
        }
#endif
    }

    return EOF;
}

void serial_hardfault_abort(char c)
{
    s_op->abort();
}

void serial_hardfault_putc(char c)
{
    s_op->tx_poll((uint8_t *)&c);
}

#endif

