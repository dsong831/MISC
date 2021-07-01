/**
 *******************************************************************************
 * @file        a31t21x_uart.c
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

#include "a31t21x_uart.h"


#define UART_IIR_RX_LINE_ERROR  (0x06)
#define UART_IIR_RX_DATA_AVAIL  (0x04)
#define UART_IIR_TX_HOLD_EMPTY  (0x02)

#define UART_REG_OFFSET     0x100

static __INLINE UART_Type *UART_REG(enum uart_id id)
{
    return (UART_Type *)(UART0_BASE + (UART_REG_OFFSET * id));
}

static __INLINE IRQn_Type UART_IRQ_NUM(enum uart_id id)
{
    return (IRQn_Type)((uint8_t)UART0_IRQn + id);
}

static __INLINE enum dma_peri UART_RX_DMA_PERI(enum uart_id id)
{
    return (enum dma_peri)(DMA_PERI_UART0_RX + (id * 2));
}

static __INLINE enum dma_peri UART_TX_DMA_PERI(enum uart_id id)
{
    return (enum dma_peri)(DMA_PERI_UART0_TX + (id * 2));
}

struct uart_ctrl_block
{
    uart_irq_handler_t  handler;
    void *              context;
    uint32_t            tx_busy : 1;
    uint32_t            rx_busy : 1;
    uint8_t *           tx_buf;
    uint8_t *           rx_buf;
    uint32_t            tx_len;
    uint32_t            rx_len;
    volatile uint32_t   tx_count;
    volatile uint32_t   rx_count;
    enum dma_ch         tx_dma_ch;
    uint8_t             tx_dma_enable;
    enum dma_ch         rx_dma_ch;
    uint8_t             rx_dma_enable;
};

static struct uart_ctrl_block g_ucb[UART_ID_MAX];

//////////////////////////////////////////////////////////////////////////////////////
static int32_t UART_Configure(struct uart_ctrl_block *ucb,
                              UART_Type *uart,
                              struct uart_cfg *cfg)
{
    uint32_t cfg_val = 0;
    uint32_t numerator;
    uint32_t denominator;
    uint32_t bdr, bfr;
    uint32_t fd;
		uint32_t cal_index1, cal_index2;

		/* Set Data Length */
    switch (cfg->data_bit)
    {
        case UART_DATA_BIT_5:
        case UART_DATA_BIT_6:
        case UART_DATA_BIT_7:
        case UART_DATA_BIT_8:
            cfg_val |= (cfg->data_bit << 0);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }
		/* Set Parity */
    switch (cfg->parity_bit)
    {
        case UART_PARITY_BIT_NONE:
						cfg_val &= ~(1 << 3);
            break;
        case UART_PARITY_BIT_ODD:
						cfg_val |= (1 << 3);
            cfg_val &= ~(1 << 4);
            break;
        case UART_PARITY_BIT_EVEN:
						cfg_val |= (1 << 3);
            cfg_val |= (1 << 4);
            break;
        case UART_PARITY_BIT_STUCK:
						cfg_val |= (1 << 3);
            cfg_val |= (1 << 5);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }
		/* Set Stop Bit */
    switch (cfg->stop_bit)
    {
        case UART_STOP_BIT_1:
						cfg_val &= ~(1 << 2);
            break;
        case UART_STOP_BIT_2:
						cfg_val |= (1 << 2);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }
    uart->LCR = cfg_val;

		/* Set Baudrate */
    numerator = SystemCoreClock / 2;
    denominator = 16 * cfg->baudrate;

		// Cal. bdr
		cal_index1 = (numerator / (denominator / 10));
		cal_index2 = (numerator / denominator) * 10;
		if ((cal_index1 - cal_index2) < 6)
		{
				bdr = (numerator / denominator);
		}
		else
		{
				bdr = (numerator / denominator) + 1;
		}

    fd = numerator - (bdr * denominator);
    bfr = (fd * 256) / denominator;

    uart->BDR = bdr & 0xFFFF;
    uart->BFR = bfr & 0xFF;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_Init(enum uart_id id, struct uart_cfg *cfg,
                      uart_irq_handler_t handler,
                      void *context)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;
    uint32_t reg_val;
    int32_t ret;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

		/* Get Register Name */
    ucb = &g_ucb[id];
    uart = UART_REG(id);

    memset(ucb, 0, sizeof(struct uart_ctrl_block));
    ucb->handler = handler;
    ucb->context = context;

		/* UART Block Enable */
		if (uart == UART0)
		{
				SCU->PER2 &= ~(1<<12);
				SCU->PCER2 &= ~(1<<12);
				SCU->PER2 |= (1<<12);
				SCU->PCER2 |= (1<<12);		
		}
		else if(uart == UART1)
		{
				SCU->PER2 &= ~(1<<13);
				SCU->PCER2 &= ~(1<<13);
				SCU->PER2 |= (1<<13);
				SCU->PCER2 |= (1<<13);
		}

		/* Check Data Line Status */
    while (uart->LSR & UART_LINE_STATUS_RX_DONE)
    {
        reg_val = uart->RBR;
    }
    while (!(uart->LSR & UART_LINE_STATUS_TX_READY))
    {
    }

		/* UART Register Initialize */
    uart->IER = 0;
    uart->LCR = 0;
    uart->DCR = 0;
    reg_val = uart->LSR;
    reg_val = uart->IIR;
    (void)reg_val;

		/* Configure UART Line */
    ret = UART_Configure(ucb, uart, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_UART_Deinit(id);
        return ret;
    }

		/* Set NVIC */
		NVIC_ClearPendingIRQ(UART_IRQ_NUM(id));
		NVIC_SetPriority(UART_IRQ_NUM(id), cfg->irq_prio);
		NVIC_EnableIRQ(UART_IRQ_NUM(id));

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_Deinit(enum uart_id id)
{
    DRIVER_ASSERT(id < UART_ID_MAX);

		/* UART Block Disable */
		if (id == UART_ID_0)
		{
				SCU->PER2 &= ~(1<<12);
				SCU->PCER2 &= ~(1<<12);
		}
		else if(id == UART_ID_1)
		{
				SCU->PER2 &= ~(1<<13);
				SCU->PCER2 &= ~(1<<13);
		}

		/* Clear NVIC */
		NVIC_DisableIRQ(UART_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_Transmit(enum uart_id id, uint8_t *out, uint32_t len,
                          enum uart_io_type io_type)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;
    DMA_Type *dma;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get Register Name */
    ucb = &g_ucb[id];
    uart = UART_REG(id);
		dma = (DMA_Type *)(DMA0_BASE + (0x10 * ucb->tx_dma_ch));

		/* Check Tx Busy */
    if (ucb->tx_busy == 1)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Set Tx Parameter */
    ucb->tx_busy = 1;
    ucb->tx_buf = (uint8_t *)out;
    ucb->tx_len = len;
    ucb->tx_count = 0;

		/* BLOCK TX */
    if (io_type == UART_IO_TYPE_BLOCK)
    {
        while (ucb->tx_len > ucb->tx_count)
        {
            uart->THR = ucb->tx_buf[ucb->tx_count++];
            while (!(uart->LSR & UART_LINE_STATUS_TX_READY))
            {
            }
        }
        ucb->tx_busy = 0;
    }
		/* NON-BLOCK TX */
    else
    {
				// Use Dma Tx
        if (ucb->tx_dma_enable)
        {
            uart->IER |= (1 << UART_IRQ_TX_DMA_DONE);	// Enable DMA transmit done interrupt

            HAL_DMA_Start(ucb->tx_dma_ch, (uint32_t)ucb->tx_buf, (uint32_t)&uart->THR, ucb->tx_len);
						while(!(dma->SR & (1 << 7)));
						dma->SR &= ~(1 << 0);
        }
				// Use Normal Tx
        else
        {
            if (uart->LSR & UART_LINE_STATUS_TX_READY)
            {
                uart->THR = ucb->tx_buf[ucb->tx_count++];
            }
            uart->IER |= (1 << UART_IRQ_TX_READY);
        }
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_Receive(enum uart_id id, uint8_t *in, uint32_t len,
                         enum uart_io_type io_type)
{
    struct uart_ctrl_block *ucb;
    uint32_t tmp;
    UART_Type *uart;
    DMA_Type *dma;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get Register Name */
    ucb = &g_ucb[id];
    uart = UART_REG(id);
		dma = (DMA_Type *)(DMA0_BASE + (0x10 * ucb->tx_dma_ch));

		/* Check Rx Busy */
    if (ucb->rx_busy == 1)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Set Rx Parameter */
    ucb->rx_busy = 1;
    ucb->rx_buf = (uint8_t *)in;
    ucb->rx_len = len;
    ucb->rx_count = 0;

		/* BLOCK RX */
    if (io_type == UART_IO_TYPE_BLOCK)
    {
        while (ucb->rx_len > ucb->rx_count)
        {
            uint32_t status = 0;

            while (!(uart->LSR & UART_LINE_STATUS_RX_DONE))
            {
            }

            status |= uart->LSR & UART_LINE_STATUS_FRAME_ERROR;
            status |= uart->LSR & UART_LINE_STATUS_PARITY_ERROR;
            status |= uart->LSR & UART_LINE_STATUS_OVERRUN_ERROR;

            if (status)
            {
                ucb->rx_busy = 0;
                return DRIVER_ERROR_HW;
            }
            tmp = uart->RBR;
            ucb->rx_buf[ucb->rx_count++] = tmp;
        }
        ucb->rx_busy = 0;
    }
		/* NON-BLOCK RX */
    else
    {
				// Use Dma Rx
        if (ucb->rx_dma_enable)
        {
            uart->IER |= (1 << UART_IRQ_RX_DMA_DONE);	// Enable DMA receive done interrupt

            HAL_DMA_Start(ucb->rx_dma_ch, (uint32_t)&uart->RBR, (uint32_t)ucb->rx_buf, ucb->rx_len);
						while(!(dma->SR & (1 << 7)));
						dma->SR &= ~(1 << 0);
        }
				// Use Normal Rx
        else
        {
            uart->IER |= (1 << UART_IRQ_RX_AVAIL);
        }
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_Abort(enum uart_id id)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

		/* Get Register Name */
    ucb = &g_ucb[id];
    uart = UART_REG(id);

		/* Break transmit mode */
    uart->LCR |= (1 << 6);
    uart->LCR &= ~(1 << 6);

    ucb->tx_busy = 0;
    ucb->rx_busy = 0;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_SetTransmitDMA(enum uart_id id, enum dma_ch ch, uint8_t enable)
{
    struct uart_ctrl_block *ucb;

    DRIVER_ASSERT(id < UART_ID_MAX);

    ucb = &g_ucb[id];

    ucb->tx_dma_enable = enable;
    ucb->tx_dma_ch = ch;

    if (enable)
    {
				/* Set DMA Parameter */
        struct dma_cfg cfg =
        {
            .peri = UART_TX_DMA_PERI(id),
            .size = DMA_BUS_SIZE_BYTE,
            .dir = DMA_DIR_MEM_TO_PERI,
        };

				/* Enable DMA Block */
        HAL_DMA_Init();

				/* Configure DMA */
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
				/* Clear DMA Parameter */
        HAL_DMA_Clear(ucb->tx_dma_ch);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_SetReceiveDMA(enum uart_id id, enum dma_ch ch, uint8_t enable)
{
    struct uart_ctrl_block *ucb;

    DRIVER_ASSERT(id < UART_ID_MAX);

    ucb = &g_ucb[id];

    ucb->rx_dma_enable = enable;
    ucb->rx_dma_ch = ch;

    if (enable)
    {
				/* Set DMA Parameter */
        struct dma_cfg cfg =
        {
            .peri = UART_RX_DMA_PERI(id),
            .size = DMA_BUS_SIZE_BYTE,
            .dir = DMA_DIR_PERI_TO_MEM,
        };

				/* Enable DMA Block */
        HAL_DMA_Init();

				/* Configure DMA */
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
				/* Clear DMA Parameter */
        HAL_DMA_Clear(ucb->rx_dma_ch);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_GetBusyStatus(enum uart_id id, uint8_t *busy)
{
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

		/* Get Register Name */
    uart = UART_REG(id);

		/* Check Transmit register is empty(TEMT) */
    if (uart->LSR & UART_LINE_STATUS_TX_EMPTY)
    {
        *busy = 0;
    }
    else
    {
        *busy = 1;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_GetLineStatus(enum uart_id id, uint8_t *status)
{
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

		/* Get Register Name */
    uart = UART_REG(id);

    *status = (uart->LSR & 0x7F);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_SetIFDelay(enum uart_id id, uint8_t delay)
{
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(delay <= 7);

		/* Get Register Name */
    uart = UART_REG(id);

    uart->IDTR = delay;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_UART_SetIRQ(enum uart_id id, enum uart_irq_type type, uint8_t enable)
{
    uint32_t reg_val = 0;
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

		/* Get Register Name */
    uart = UART_REG(id);

    switch (type)
    {
        case UART_IRQ_RX_AVAIL:
            reg_val = (1 << UART_IRQ_RX_AVAIL);
            break;
        case UART_IRQ_TX_READY:
            reg_val = (1 << UART_IRQ_TX_READY);
            break;
        case UART_IRQ_TX_EXIT:
            reg_val = (1 << UART_IRQ_TX_EXIT);
            break;
        case UART_IRQ_LINE_STATUS:
            reg_val = (1 << UART_IRQ_LINE_STATUS);
            break;
        case UART_IRQ_RX_DMA_DONE:
            reg_val = (1 << UART_IRQ_RX_DMA_DONE);
            break;
        case UART_IRQ_TX_DMA_DONE:
            reg_val = (1 << UART_IRQ_TX_DMA_DONE);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    if (enable)
    {
        uart->IER |= reg_val;
    }
    else
    {
        uart->IER &= ~reg_val;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void UART_IRQHandler(enum uart_id id)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;
    volatile uint32_t ier;
    volatile uint32_t int_status;
    volatile uint32_t line_status;
    volatile uint32_t event = 0;
    uint32_t tmp;

		/* Get Register Name */
    ucb = &g_ucb[id];
    uart = UART_REG(id);

		ier = uart->IER;
    int_status = uart->IIR;

		/* Line Error Interrupt */
    if ((int_status & UART_IIR_RX_LINE_ERROR) == UART_IIR_RX_LINE_ERROR)
    {
        volatile uint8_t reg_val;

        event |= UART_EVENT_LINE_ERROR;
        line_status = uart->LSR;
        if (line_status & UART_LINE_STATUS_BREAK)
        {
            event |= UART_EVENT_BREAK_ERROR;
        }
        if (line_status & UART_LINE_STATUS_FRAME_ERROR)
        {
            event |= UART_EVENT_FRAME_ERROR;
        }
        if (line_status & UART_LINE_STATUS_PARITY_ERROR)
        {
            event |= UART_EVENT_PARITY_ERROR;
        }
        if (line_status & UART_LINE_STATUS_OVERRUN_ERROR)
        {
            event |= UART_EVENT_OVERFLOW_ERROR;
        }

        reg_val = uart->RBR;
        uart->IER = 0;
        (void)(reg_val);
    }
    else
    {
				/* RX DMA Interrupt */
        if ((ier & (1 << UART_IRQ_RX_DMA_DONE)) == (1 << UART_IRQ_RX_DMA_DONE))
        {
            ucb->rx_busy = 0;
						ucb->rx_dma_enable = 0;
            uart->IER &= ~(1 << UART_IRQ_RX_DMA_DONE);
            event |= UART_EVENT_RECEIVE_DONE;
        }
				/* RX Interrupt */
        else if (int_status & UART_IIR_RX_DATA_AVAIL)
        {
            tmp = uart->RBR;
            ucb->rx_buf[ucb->rx_count] = tmp;
 
            ucb->rx_count++;
            if (ucb->rx_len == ucb->rx_count)
            {
                ucb->rx_busy = 0;
                uart->IER &= ~(1 << UART_IRQ_RX_AVAIL);
                event |= UART_EVENT_RECEIVE_DONE;
            }
        }

				/* TX DMA Interrupt */
        if ((ier & (1 << UART_IRQ_TX_DMA_DONE)) == (1 << UART_IRQ_TX_DMA_DONE))
        {
            ucb->tx_busy = 0;
						ucb->tx_dma_enable = 0;
            uart->IER &= ~(1 << UART_IRQ_TX_DMA_DONE);
            event |= UART_EVENT_TRANSMIT_DONE;
        }
				/* TX Interrupt */
        else if (int_status & UART_IIR_TX_HOLD_EMPTY)
        {
            if (ucb->tx_len != ucb->tx_count)
            {
                uart->THR = ucb->tx_buf[ucb->tx_count++];
            }
            else
            {
                ucb->tx_busy = 0;
                uart->IER &= ~(1 << UART_IRQ_TX_READY);
                event |= UART_EVENT_TRANSMIT_DONE;
            }
        }
    }

		/* Callback function */
    if (ucb->handler && event)
    {
        ucb->handler(event, ucb->context);
    }
}

//////////////////////////////////////////////////////////////////////////////////////
void UART0_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_0);
}

void UART1_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_1);
}

