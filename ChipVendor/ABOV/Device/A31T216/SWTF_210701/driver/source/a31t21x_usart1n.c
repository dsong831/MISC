/**
 *******************************************************************************
 * @file        a31t21x_usart1n.c
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

#include "a31t21x_usart1n.h"


#define USART_REG_OFFSET     0x100

static __INLINE USART_Type *USART_REG(enum usart_id id)
{
    return (USART_Type *)(USART10_BASE + (USART_REG_OFFSET * id));
}

static __INLINE IRQn_Type USART_IRQ_NUM(enum usart_id id)
{
		if (id == 0)
		{
				return (IRQn_Type)((uint8_t)USART10_IRQn);
		}
		else
		{
				return (IRQn_Type)((uint8_t)USART10_IRQn + id + 5);
		}
}

static __INLINE enum dma_peri USART_RX_DMA_PERI(enum usart_id id)
{
    return (enum dma_peri)(DMA_PERI_USART10_RX + (id * 2));
}

static __INLINE enum dma_peri USART_TX_DMA_PERI(enum usart_id id)
{
    return (enum dma_peri)(DMA_PERI_USART10_TX + (id * 2));
}

struct usart_ctrl_block
{
    usart_irq_handler_t  handler;
    void *               context;
    uint32_t             tx_busy : 1;
    uint32_t             rx_busy : 1;
    uint8_t *            tx_buf;
    uint8_t *            rx_buf;
    uint32_t             tx_len;
    uint32_t             rx_len;
    volatile uint32_t    tx_count;
    volatile uint32_t    rx_count;
    enum dma_ch          tx_dma_ch;
    uint8_t              tx_dma_enable;
    enum dma_ch          rx_dma_ch;
    uint8_t              rx_dma_enable;
};

static struct usart_ctrl_block g_ucb[USART_ID_MAX];

//////////////////////////////////////////////////////////////////////////////////////
static __INLINE uint32_t USART_StatusCheck(USART_Type *reg, uint32_t status_mask)
{
    return reg->ST & status_mask;
}

//////////////////////////////////////////////////////////////////////////////////////
static int32_t USART_Configure(struct usart_ctrl_block *ucb,
                              USART_Type *usart,
                              struct usart_cfg *cfg)
{
    uint32_t cfg_val = 0;
    uint32_t numerator;
    uint32_t denominator;
    uint32_t bdr, bfr;
    uint32_t fd;
		uint32_t cal_index1;
		uint32_t cal_index2;

		if ((cfg->mode == USART_MODE_ASYNC) || (cfg->mode == USART_MODE_SYNC))
		{
				/* Set Mode */
				usart->CR1 &= ~(3 << 14);

				/* Set Data Length */
				switch (cfg->data_bit)
				{
						case USART_DATA_BIT_5:
						case USART_DATA_BIT_6:
						case USART_DATA_BIT_7:
						case USART_DATA_BIT_8:
						case USART_DATA_BIT_9:
								cfg_val |= (cfg->data_bit << 9);
								break;
						default:
								return DRIVER_ERROR_PARAMETER;
				}
				/* Set Parity */
				switch (cfg->parity_bit)
				{
						case USART_PARITY_BIT_NONE:
								cfg_val &= ~(3 << 12);
								break;
						case USART_PARITY_BIT_ODD:
								cfg_val |= (3 << 12);
								break;
						case USART_PARITY_BIT_EVEN:
								cfg_val &= ~(1 << 12);
								cfg_val |= (1 << 13);
								break;
						default:
								return DRIVER_ERROR_PARAMETER;
				}
				usart->CR1 |= cfg_val;
				cfg_val = 0;

				/* Set Stop Bit */
				switch (cfg->stop_bit)
				{
						case USART_STOP_BIT_1:
								cfg_val &= ~(1 << 2);
								break;
						case USART_STOP_BIT_2:
								cfg_val |= (1 << 2);
								break;
						default:
								return DRIVER_ERROR_PARAMETER;
				}
				usart->CR2 = cfg_val;

				/* Set Baudrate */
				usart->CR2 &= ~(1 << 8);
				numerator = SystemCoreClock;
				denominator = 16 * cfg->baudrate;

				// Cal. bdr
				cal_index1 = (numerator / (denominator / 10));
				cal_index2 = (numerator / denominator) * 10;
				if ((cal_index1 - cal_index2) < 6)
				{
						bdr = (numerator / denominator) - 1;
				}
				else
				{
						bdr = (numerator / denominator);
				}

				// To use double speed mode
				if (bdr <= 5)
				{
						usart->CR2 |= (1 << 8);
						numerator = SystemCoreClock;
						denominator = 8 * cfg->baudrate;

						// Cal. bdr
						cal_index1 = (numerator / (denominator / 10));
						cal_index2 = (numerator / denominator) * 10;
						if ((cal_index1 - cal_index2) < 6)
						{
								bdr = (numerator / denominator) - 1;
						}
						else
						{
								bdr = (numerator / denominator);
						}
				}

				fd = numerator - (bdr * denominator);
				bfr = (fd * 256) / denominator;

				usart->BDR = bdr & 0xFFFF;
				usart->BFR = bfr & 0xFF;

				if (cfg->mode == USART_MODE_SYNC)
				{
						/* Set USART Parameter */
						/* Set Mode */
						usart->CR1 &= ~(3 << 14);
						usart->CR1 |= (1 << 14);

						/* Configure Sync mode */
						switch (cfg->sync_mode)
						{
								case USART_SYHC_MODE_CPOL_1_CPHA_0:
										usart->CR1 |= (1 << 7);
										break;
								case USART_SYHC_MODE_CPOL_0_CPHA_0:
										usart->CR1 &= ~(1 << 7);
										break;
								default:
										DRIVER_ASSERT(0);
						}

						/* Select master/slave type */
						switch (cfg->master_sel)
						{
								case USART_SEL_MASTER:
										usart->CR2 |= (1 << 7);
										usart->CR2 |= (1 << 5);
										break;
								case USART_SEL_SLAVE:
										usart->CR2 &= ~(1 << 7);
										usart->CR2 |= (1 << 5);
										break;
								default:
										DRIVER_ASSERT(0);
						}

						/* Set Baudrate */
						numerator = SystemCoreClock;
						denominator = 2 * cfg->baudrate;
						bdr = (numerator / denominator) - 1;

						fd = numerator - (bdr * denominator);
						bfr = (fd * 256) / denominator;

						usart->BDR = bdr & 0xFFFF;
						usart->BFR = bfr & 0xFF;
				}
		}

		else if (cfg->mode == USART_MODE_SPI)
		{
				/* Set SPI Parameter */
				/* Set Mode */
				usart->CR1 |= (3 << 14);

				/* Set Data Length */
				switch (cfg->data_bit)
				{
						case USART_DATA_BIT_5:
						case USART_DATA_BIT_6:
						case USART_DATA_BIT_7:
						case USART_DATA_BIT_8:
						case USART_DATA_BIT_9:
								cfg_val |= (cfg->data_bit << 9);
								break;
						default:
								return DRIVER_ERROR_PARAMETER;
				}

				/* Configure bit order */
				switch (cfg->bit_order)
				{
						case USART_BIT_ORDER_MSB_FIRST:
								cfg_val |= (1 << 8);
								break;
						case USART_BIT_ORDER_LSB_FIRST:
								cfg_val &= ~(1 << 8);
								break;
						default:
								DRIVER_ASSERT(0);
				}

				/* Configure Sync mode */
				switch (cfg->sync_mode)
				{
						case USART_SYHC_MODE_CPOL_1_CPHA_0:
								cfg_val |= (1 << 7);
								cfg_val &= ~(1 << 6);
								break;
						case USART_SYHC_MODE_CPOL_1_CPHA_1:
								cfg_val |= (1 << 7);
								cfg_val |= (1 << 6);
								break;
						case USART_SYHC_MODE_CPOL_0_CPHA_0:
								cfg_val &= ~(1 << 7);
								cfg_val &= ~(1 << 6);
								break;
						case USART_SYHC_MODE_CPOL_0_CPHA_1:
								cfg_val &= ~(1 << 7);
								cfg_val |= (1 << 6);
								break;
						default:
								DRIVER_ASSERT(0);
				}

				usart->CR1 |= cfg_val;
				cfg_val = 0;

				/* Select master/slave type */
				switch (cfg->master_sel)
				{
						case USART_SEL_MASTER:
								cfg_val |= (1 << 7);
								break;
						case USART_SEL_SLAVE:
								cfg_val &= ~(1 << 7);
								break;
						default:
								DRIVER_ASSERT(0);
				}

				/* Configure ss type */
				switch (cfg->ss_type)
				{
						case USART_SS_MANUAL:
								cfg_val &= ~(1 << 4);
								break;
						case USART_SS_AUTO:
								cfg_val |= (1 << 4);
								break;
						default:
								DRIVER_ASSERT(0);
				}

				usart->CR2 |= cfg_val;

				/* Set Baudrate */
				numerator = SystemCoreClock;
				denominator = 2 * cfg->baudrate;
				bdr = (numerator / denominator) - 1;

				fd = numerator - (bdr * denominator);
				bfr = (fd * 256) / denominator;

				usart->BDR = bdr & 0xFFFF;
				usart->BFR = bfr & 0xFF;
		}


		/* Enable USART */
		usart->CR1 |= (3 << 0);	// <TXEn>,<RXEn>
		usart->CR2 |= (1 << 9);	// <USTnEN>

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_USART_Init(enum usart_id id, struct usart_cfg *cfg,
                      usart_irq_handler_t handler,
                      void *context)
{
    struct usart_ctrl_block *ucb;
    USART_Type *usart;
    int32_t ret;

    DRIVER_ASSERT(id < USART_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

		/* Get Register Name */
    ucb = &g_ucb[id];
    usart = USART_REG(id);

    memset(ucb, 0, sizeof(struct usart_ctrl_block));
    ucb->handler = handler;
    ucb->context = context;

		/* USART Block Enable */
		if (usart == USART10)
		{
				SCU->PER2 &= ~(1<<8);
				SCU->PCER2 &= ~(1<<8);
				SCU->PER2 |= (1<<8);
				SCU->PCER2 |= (1<<8);
		}
		else if(usart == USART11)
		{
				SCU->PER2 &= ~(1<<9);
				SCU->PCER2 &= ~(1<<9);
				SCU->PER2 |= (1<<9);
				SCU->PCER2 |= (1<<9);
		}

		/* USART Register Initialize */
		usart->CR1 = 0;
		usart->CR2 = 0;

		/* Configure UART Line */
    ret = USART_Configure(ucb, usart, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_USART_Deinit(id);
        return ret;
    }

		/* Set NVIC */
		NVIC_ClearPendingIRQ(USART_IRQ_NUM(id));
		NVIC_SetPriority(USART_IRQ_NUM(id), cfg->irq_prio);
		NVIC_EnableIRQ(USART_IRQ_NUM(id));

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_USART_Deinit(enum usart_id id)
{
    DRIVER_ASSERT(id < USART_ID_MAX);

		/* USART Block Disable */
		if (id == USART_ID_10)
		{
				SCU->PER2 &= ~(1<<8);
				SCU->PCER2 &= ~(1<<8);
		}
		else if(id == USART_ID_11)
		{
				SCU->PER2 &= ~(1<<9);
				SCU->PCER2 &= ~(1<<9);
		}

		/* Clear NVIC */
		NVIC_DisableIRQ(USART_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_USART_Transmit(enum usart_id id, uint8_t *out, uint32_t len,
                          enum usart_io_type io_type)
{
    struct usart_ctrl_block *ucb;
    USART_Type *usart;

    DRIVER_ASSERT(id < USART_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get Register Name */
    ucb = &g_ucb[id];
    usart = USART_REG(id);

		///////////////////////////////////////
		/* SPI MODE */
		if ((usart->CR1 & 0xC000) == 0xC000)
		{
				HAL_USART_Transfer(id, out, len, NULL, 0, io_type);
		}
		///////////////////////////////////////
		/* ASYNCHRONOUS or SYNCHRONOUS MODE */
		else
		{
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
				if (io_type == USART_IO_TYPE_BLOCK)
				{
						while (ucb->tx_len > ucb->tx_count)
						{
								usart->DR = ucb->tx_buf[ucb->tx_count++];
								while (!(usart->ST & USART_ST_DRE))
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
								HAL_DMA_Start(ucb->tx_dma_ch, (uint32_t)ucb->tx_buf, (uint32_t)&usart->DR, ucb->tx_len);
						}
						// Use Normal Tx
						else
						{
								usart->CR1 |= (1 << USART_IRQ_DRIE);
						}
				}
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_USART_Receive(enum usart_id id, uint8_t *in, uint32_t len,
                         enum usart_io_type io_type)
{
    struct usart_ctrl_block *ucb;
    uint32_t tmp;
    USART_Type *usart;

    DRIVER_ASSERT(id < USART_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get Register Name */
    ucb = &g_ucb[id];
    usart = USART_REG(id);

		///////////////////////////////////////
		/* SPI MODE */
		if ((usart->CR1 & 0xC000) == 0xC000)
		{
				HAL_USART_Transfer(id, NULL, 0, in, len, io_type);
		}
		///////////////////////////////////////
		/* ASYNCHRONOUS or SYNCHRONOUS MODE */
		else
		{
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
				if (io_type == USART_IO_TYPE_BLOCK)
				{
						while (ucb->rx_len > ucb->rx_count)
						{
								while (!(usart->ST & USART_ST_RXC))
								{
								}

								tmp = usart->DR;
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
								HAL_DMA_Start(ucb->rx_dma_ch, (uint32_t)&usart->DR, (uint32_t)ucb->rx_buf, ucb->rx_len);
						}
						// Use Normal Rx
						else
						{
								usart->CR1 |= (1 << USART_IRQ_RXCIE);
						}
				}
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_USART_Transfer(enum usart_id id,
                         uint8_t *out, uint32_t out_len,
                         uint8_t *in, uint32_t in_len,
                         enum usart_io_type io_type)
{
    struct usart_ctrl_block *ucb;
    USART_Type *usart;

    DRIVER_ASSERT(id < USART_ID_MAX);

		/* Get USART Register */
    ucb = &g_ucb[id];
    usart = USART_REG(id);

		/* Check status */
    if (ucb->rx_busy || ucb->tx_busy)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Update USART Parameter */
    if (out_len)
    {
        ucb->tx_busy = 1;
    }
    if (in_len)
    {
        ucb->rx_busy = 1;
    }
    ucb->tx_len = out_len;
    ucb->rx_len = in_len;
    ucb->rx_count = 0;
    ucb->tx_count = 0;
    ucb->rx_buf = (uint8_t *)in;
    ucb->tx_buf = (uint8_t *)out;

		/* BLOCK TRX */
    if (io_type == USART_IO_TYPE_BLOCK)
    {
        while (ucb->rx_len > ucb->rx_count || ucb->tx_len > ucb->tx_count)
        {
            uint32_t data;

						/* Master mode */
						if (usart->CR2 & (1 << 7))
						{
								// Master Transmit
								if (ucb->tx_len <= ucb->tx_count)
								{
										while (!USART_StatusCheck(usart, (1 << 7)))	// Check Tx Buffer Empty <DRE>
										{
										}
										usart->DR = 0xFF;	// Dummy Write
								}
								else
								{
										while (!USART_StatusCheck(usart, (1 << 7)))	// Check Tx Buffer Empty <DRE>
										{
										}
										data = *ucb->tx_buf++;
										usart->DR = data;	// Tx Data Write
										ucb->tx_count++;
								}

								// Master Receive
								while (1)
								{
										if ((USART_StatusCheck(usart, (1 << 6)) && USART_StatusCheck(usart, (1 << 5))))	break;	// <TXC>&&<RXC>
								}
								data = usart->DR;

								if (ucb->rx_len > ucb->rx_count)
								{
										*(ucb->rx_buf++) = (uint8_t)data;
										ucb->rx_count++;
								}
						}
						/* Slave mode */
						else
						{
								while (!(USART_StatusCheck(usart, (1 << 5))))	// <RXC>
								{
								}
								data = usart->DR;

								// Slave Receive
								if (ucb->rx_len > ucb->rx_count)
								{
										*(ucb->rx_buf++) = (uint8_t)data;
										ucb->rx_count++;
								}
								// Slave Transmit
								if (ucb->tx_len > ucb->tx_count)
								{
										while (!USART_StatusCheck(usart, (1 << 7)))	// Check Tx Buffer Empty <DRE>
										{
										}
										data = *ucb->tx_buf++;
										usart->DR = data;	// Tx Data Write
										ucb->tx_count++;
								}
						}
        }

        ucb->rx_busy = 0;
        ucb->tx_busy = 0;

				/* Check Error Flags */
        if (USART_StatusCheck(usart, (1 << 2)))		// <OVRF>
        {
            ucb->tx_busy = 0;
            return DRIVER_ERROR_HW;
        }
    }

		/* NON-BLOCK TRX */
    else
    {
				if ((ucb->tx_dma_enable) || (ucb->rx_dma_enable))
				{
						if (usart->CR2 & (1 << 7))
						{
								HAL_DMA_Start(ucb->tx_dma_ch, (uint32_t)ucb->tx_buf, (uint32_t)&usart->DR, ucb->tx_len);
						}
						else
						{
								HAL_DMA_Start(ucb->rx_dma_ch, (uint32_t)&usart->DR, (uint32_t)ucb->rx_buf, ucb->rx_len);
						}
				}
        else
        {
						/* Master mode */
						if (usart->CR2 & (1 << 7))
						{
								usart->CR1 |= ((1 << USART_IRQ_DRIE));
						}
						/* Slave mode */
						else
						{
								usart->DR = 0x00;
								usart->CR1 |= ((1 << USART_IRQ_RXCIE));
						}
        }
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_USART_SetTransmitDMA(enum usart_id id, enum dma_ch ch, uint8_t enable)
{
    struct usart_ctrl_block *ucb;

    DRIVER_ASSERT(id < USART_ID_MAX);

    ucb = &g_ucb[id];

    ucb->tx_dma_enable = enable;
    ucb->tx_dma_ch = ch;

    if (enable)
    {
				/* Set DMA Parameter */
        struct dma_cfg cfg =
        {
            .peri = USART_TX_DMA_PERI(id),
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
int32_t HAL_USART_SetReceiveDMA(enum usart_id id, enum dma_ch ch, uint8_t enable)
{
    struct usart_ctrl_block *ucb;

    DRIVER_ASSERT(id < USART_ID_MAX);

    ucb = &g_ucb[id];

    ucb->rx_dma_enable = enable;
    ucb->rx_dma_ch = ch;

    if (enable)
    {
				/* Set DMA Parameter */
        struct dma_cfg cfg =
        {
            .peri = USART_RX_DMA_PERI(id),
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
int32_t HAL_USART_GetBusyStatus(enum usart_id id, uint8_t *busy)
{
    USART_Type *usart;

    DRIVER_ASSERT(id < USART_ID_MAX);

		/* Get Register Name */
    usart = USART_REG(id);

		/* Check Transmit Complete */
    if (usart->ST & USART_ST_TXC)
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
int32_t HAL_USART_GetLineStatus(enum usart_id id, uint8_t *status)
{
    USART_Type *usart;

    DRIVER_ASSERT(id < USART_ID_MAX);

		/* Get Register Name */
    usart = USART_REG(id);

    *status = (usart->ST & 0xFF);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_USART_SetIRQ(enum usart_id id, enum usart_irq_type type, uint8_t enable)
{
    uint32_t reg_val = 0;
    USART_Type *usart;

    DRIVER_ASSERT(id < USART_ID_MAX);

		/* Get Register Name */
    usart = USART_REG(id);

    switch (type)
    {
        case USART_IRQ_WAKEIE:
            reg_val = (1 << USART_IRQ_WAKEIE);
            break;
        case USART_IRQ_RXCIE:
            reg_val = (1 << USART_IRQ_RXCIE);
            break;
        case USART_IRQ_TXCIE:
            reg_val = (1 << USART_IRQ_TXCIE);
            break;
        case USART_IRQ_DRIE:
            reg_val = (1 << USART_IRQ_DRIE);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    if (enable)
    {
        usart->CR1 |= reg_val;
    }
    else
    {
        usart->CR1 &= ~reg_val;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void USART_IRQHandler(enum usart_id id)
{
    struct usart_ctrl_block *ucb;
    USART_Type *usart;
    volatile uint32_t int_status;
    volatile uint32_t event = 0;
		volatile uint32_t data;
    uint32_t tmp;

		/* Get Register Name */
    ucb = &g_ucb[id];
    usart = USART_REG(id);

    int_status = usart->ST;

		///////////////////////////////////////
		/* SPI MODE */
		if ((usart->CR1 & 0xC000) == 0xC000)
		{
				/* Master Mode */
				if ((usart->CR2 & (1 << 7)))
				{
						// Master Send
						if (ucb->tx_count < ucb->tx_len)
						{
								if ((USART_StatusCheck(usart, USART_ST_DRE)) && (ucb->tx_count == ucb->rx_count))
								{
										data = *ucb->tx_buf++;
										usart->DR = data;			// ## Data Write ## //
										ucb->tx_count++;
								}
								while (!(USART_StatusCheck(usart, USART_ST_TXC))){}
								if (USART_StatusCheck(usart, USART_ST_RXC))
								{
										data = usart->DR;			// ## Data Read ## //
										ucb->rx_count++;
								}
						}
						// Master Recv
						else if (ucb->rx_count < ucb->rx_len)
						{
								if ((USART_StatusCheck(usart, USART_ST_DRE)) && (ucb->tx_count == ucb->rx_count))
								{
										data = 0xFF;
										usart->DR = data;			// ## Data Write ## //
										ucb->tx_count++;
								}
								while (!(USART_StatusCheck(usart, USART_ST_TXC))){}
								if (USART_StatusCheck(usart, USART_ST_RXC))
								{
										data = usart->DR;			// ## Data Read ## //
										*(ucb->rx_buf++) = (uint8_t)data;
										ucb->rx_count++;
								}
						}
						// Complete
						else
						{
								ucb->rx_busy = 0;
								ucb->tx_busy = 0;
								usart->CR1 &= ~((1 << 5) | (1 << 3));	// Disable TRX Interrupt

								if (ucb->handler)
								{
										if (USART_StatusCheck(usart, USART_ST_DOR))
										{
												event |= USART_EVENT_OVERFLOW_ERROR;
										}

										event |= USART_EVENT_TRANSMIT_DONE;
								}
						}
				}
				/* Slave Mode */
				else
				{
						if (USART_StatusCheck(usart, USART_ST_RXC))
						{
								data = usart->DR;					// ## Data Read ## //

								// Slave Recv
								if (ucb->rx_count < ucb->rx_len)
								{
										*(ucb->rx_buf++) = (uint8_t)data;
										ucb->rx_count++;
										if (USART_StatusCheck(usart, USART_ST_DRE))
										{
												usart->DR = 0x00;	// ## Data Write ## //
										}

										// Complete
										if (ucb->rx_count >= ucb->rx_len)
										{
												ucb->rx_busy = 0;
												ucb->tx_busy = 0;
												usart->CR1 &= ~((1 << 5) | (1 << 3));	// Disable TRX Interrupt

												if (ucb->handler)
												{
														if (USART_StatusCheck(usart, USART_ST_DOR))
														{
																event |= USART_EVENT_OVERFLOW_ERROR;
														}

														event |= USART_EVENT_TRANSMIT_DONE;
												}
										}
								}
								// Slave Send
								else if (ucb->tx_count < ucb->tx_len)
								{
										if (USART_StatusCheck(usart, USART_ST_DRE))
										{
												data = *ucb->tx_buf++;
												usart->DR = data;	// ## Data Write ## //
												ucb->tx_count++;
										}

										// Complete
										if (ucb->tx_count >= ucb->tx_len)
										{
												ucb->rx_busy = 0;
												ucb->tx_busy = 0;
												usart->CR1 &= ~((1 << 5) | (1 << 3));	// Disable TRX Interrupt

												if (ucb->handler)
												{
														if (USART_StatusCheck(usart, USART_ST_DOR))
														{
																event |= USART_EVENT_OVERFLOW_ERROR;
														}

														event |= USART_EVENT_TRANSMIT_DONE;
												}
										}
								}
						}
				}
		}
		///////////////////////////////////////
		/* ASYNCHRONOUS or SYNCHRONOUS MODE */
		else
		{
				/* Line Error Interrupt */
				if ((int_status & USART_ST_DOR) || (int_status & USART_ST_FE) || (int_status & USART_ST_PE))
				{
						volatile uint8_t reg_val;

						event |= USART_EVENT_LINE_ERROR;
						if (int_status & USART_ST_DOR)
						{
								event |= USART_EVENT_OVERFLOW_ERROR;
						}
						if (int_status & USART_ST_FE)
						{
								event |= USART_EVENT_FRAME_ERROR;
						}
						if (int_status & USART_ST_PE)
						{
								event |= USART_EVENT_PARITY_ERROR;
						}

						reg_val = usart->DR;
						(void)(reg_val);
				}
				else
				{
						/* RX Interrupt */
						if (int_status & USART_ST_RXC)
						{
								tmp = usart->DR;
								ucb->rx_buf[ucb->rx_count] = tmp;
		 
								ucb->rx_count++;
								if (ucb->rx_len == ucb->rx_count)
								{
										ucb->rx_busy = 0;
										usart->CR1 &= ~(1 << USART_IRQ_RXCIE);
										event |= USART_EVENT_RECEIVE_DONE;
								}
						}

						/* TX Interrupt */
						if (int_status & USART_ST_DRE)
						{
								if (ucb->tx_len != ucb->tx_count)
								{
										usart->DR = ucb->tx_buf[ucb->tx_count++];
								}
								else
								{
										ucb->tx_busy = 0;
										usart->CR1 &= ~(1 << USART_IRQ_DRIE);
										event |= USART_EVENT_TRANSMIT_DONE;
								}
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
void USART10_IRQHandler(void)
{
    USART_IRQHandler(USART_ID_10);
}

void USART11_IRQHandler(void)
{
    USART_IRQHandler(USART_ID_11);
}

