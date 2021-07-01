/**
 *******************************************************************************
 * @file        a31t21x_spi.c
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

#include "a31t21x_spi.h"


#define SPI_REG_OFFSET     0x100

static __INLINE SPI_Type *SPI_REG(enum spi_id id)
{
    return (SPI_Type *)(SPI20_BASE + (SPI_REG_OFFSET * id));
}

static __INLINE IRQn_Type SPI_IRQ_NUM(enum spi_id id)
{
    return (IRQn_Type)((uint8_t)SPI20_IRQn + id);
}

static __INLINE enum dma_peri SPI_RX_DMA_PERI(enum spi_id id)
{
    return (enum dma_peri)(DMA_PERI_SPI20_RX + (id * 2));
}

static __INLINE enum dma_peri SPI_TX_DMA_PERI(enum spi_id id)
{
    return (enum dma_peri)(DMA_PERI_SPI20_TX + (id * 2));
}

struct spi_ctrl_block
{
    spi_irq_handler_t   handler;
    void *              context;
    uint8_t             tx_busy : 1;
    uint8_t             rx_busy : 1;
    uint8_t *           tx_buf;
    uint8_t *           rx_buf;
    uint32_t            tx_len;
    uint32_t            rx_len;
    volatile uint32_t   tx_count;
    volatile uint32_t   rx_count;
    uint8_t             tx_dma_enable;
    enum dma_ch         tx_dma_ch;
    uint8_t             rx_dma_enable;
    enum dma_ch         rx_dma_ch;
};


static struct spi_ctrl_block g_scb[SPI_ID_MAX];

//////////////////////////////////////////////////////////////////////////////////////
static __INLINE uint32_t SPI_StatusCheck(SPI_Type *reg, uint32_t status_mask)
{
    return reg->SR & status_mask;
}

//////////////////////////////////////////////////////////////////////////////////////
static int32_t SPI_Configure(struct spi_ctrl_block *scb, SPI_Type *spi,
                             struct spi_cfg *cfg)
{
    uint32_t cfg_val = 0;

		/* Disable SPI */
    spi->EN = 0;

		/* Configure SPI Mode */
    switch (cfg->mode)
    {
        case SPI_MODE_MASTER:
            cfg_val |= (1 << 5);
            break;
        case SPI_MODE_SLAVE:
            cfg_val &= ~(1 << 5);
            break;
        default:
            DRIVER_ASSERT(0);
    }

		/* Configure Transmit/Receive Data Bits */
    switch (cfg->data_bit)
    {
        case SPI_DATA_BIT_8:
						cfg_val &= ~(1 << 0);
            break;
        case SPI_DATA_BIT_9:
            cfg_val |= (1 << 0);
            break;
        case SPI_DATA_BIT_16:
            cfg_val |= (2 << 0);
            break;
        case SPI_DATA_BIT_17:
            cfg_val |= (3 << 0);
            break;
        default:
            DRIVER_ASSERT(0);
    }

		/* Configure Sync mode */
    switch (cfg->sync_mode)
    {
        case SPI_SYHC_MODE_CPOL_1_CPHA_0:
						cfg_val &= ~(1 << 3);
            cfg_val |= (1 << 2);
            break;
        case SPI_SYHC_MODE_CPOL_1_CPHA_1:
						cfg_val |= (1 << 3);
            cfg_val |= (1 << 2);
            break;
        case SPI_SYHC_MODE_CPOL_0_CPHA_0:
						cfg_val &= ~(1 << 3);
            cfg_val &= ~(1 << 2);
            break;
        case SPI_SYHC_MODE_CPOL_0_CPHA_1:
						cfg_val |= (1 << 3);
            cfg_val &= ~(1 << 2);
            break;
        default:
            DRIVER_ASSERT(0);
    }

		/* Configure bit order */
    switch (cfg->bit_order)
    {
        case SPI_BIT_ORDER_MSB_FIRST:
            cfg_val |= (1 << 4);
            break;
        case SPI_BIT_ORDER_LSB_FIRST:
            cfg_val &= ~(1 << 4);
            break;
        default:
            DRIVER_ASSERT(0);
    }

		/* Configure ss type */
    switch (cfg->ss_type)
    {
        case SPI_SS_AUTO:
            cfg_val &= ~(1 << 13);
            break;
        case SPI_SS_MANUAL:
            cfg_val |= (1 << 13);
            break;
        default:
            DRIVER_ASSERT(0);
    }

		/* Enable SS output signal */
    cfg_val |= (1 << 9);

		/* Set SPI Parameters */
    spi->CR = cfg_val;

		/* Configure Baudrate */
		if (cfg->mode == SPI_MODE_MASTER)
		{
				spi->BR = ((SystemPeriClock / cfg->freq) - 1) & 0xFFFF;		// Baudrate = PCLK/(BR+1)
		}
		else
		{
				// if using slave, BR has fase clock set and LR has minimum set.
				spi->BR = 2;	// Baudrate = PCLK/(BR+1)
				spi->LR = 0x00010101UL;
		}

		/* Set Delay Length */
    spi->LR = 0x00101010UL;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SPI_Init(enum spi_id id, struct spi_cfg *cfg,
                     spi_irq_handler_t handler,
                     void *context)
{
    struct spi_ctrl_block *scb;
    SPI_Type *spi;
    int32_t ret;

    DRIVER_ASSERT(id < SPI_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

		/* Get SPI Register */
    scb = &g_scb[id];
    spi = SPI_REG(id);

    memset(scb, 0, sizeof(struct spi_ctrl_block));

		/* SPI Block Set */
		if (id == SPI_ID_20)
		{
				SCU->PER2 &= ~(1<<14);
				SCU->PCER2 &= ~(1<14);
				SCU->PER2 |= (1<<14);
				SCU->PCER2 |= (1<<14);
		}
		else if (id == SPI_ID_21)
		{
				SCU->PER2 &= ~(1<<15);
				SCU->PCER2 &= ~(1<<15);
				SCU->PER2 |= (1<<15);
				SCU->PCER2 |= (1<<15);
		}

    scb->context = context;
    scb->handler = handler;
    spi->CR = 0;

		/* Configure SPI Control Parameter */
    ret = SPI_Configure(scb, spi, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_SPI_Deinit(id);
        return ret;
    }

		/* Set the transmit buffer to be reset */
    spi->EN = 1; spi->EN = 0;

		/* Set NVIC */
    NVIC_ClearPendingIRQ(SPI_IRQ_NUM(id));
    NVIC_SetPriority(SPI_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(SPI_IRQ_NUM(id));

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SPI_Deinit(enum spi_id id)
{
    DRIVER_ASSERT(id < SPI_ID_MAX);

		/* SPI Block Reset */
		if (id == SPI_ID_20)
		{
				SCU->PER2 &= ~(1<<14);
				SCU->PCER2 &= ~(1<14);
		}
		else if (id == SPI_ID_21)
		{
				SCU->PER2 &= ~(1<<15);
				SCU->PCER2 &= ~(1<<15);
		}

		/* Clear NVIC */
    NVIC_DisableIRQ(SPI_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SPI_SetDelay(enum spi_id id, uint8_t start, uint8_t burst, uint8_t stop)
{
    SPI_Type *spi;

    DRIVER_ASSERT(id < SPI_ID_MAX);

		/* Get SPI Register */
    spi = SPI_REG(id);

		/* Configure Delay Parameter */
    spi->LR = (start << 0)
              | (burst << 8)
              | (stop << 16);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SPI_SSManual(enum spi_id id, enum ss_state ss_state)
{
    SPI_Type *spi;

    DRIVER_ASSERT(id < SPI_ID_MAX);

		/* Get SPI Register */
    spi = SPI_REG(id);

		/* Check Buffer */
		while (SPI_StatusCheck(spi, SPI_SR_SBUSY_Msk))
		{
		}

		/* Configure SS output signal */
    switch (ss_state)
    {
        case SPI_SS_STATE_LOW:
            spi->CR &= ~(1 << 12);
            break;
        case SPI_SS_STATE_HIGH:
            spi->CR |= (1 << 12);
            break;
        default:
            DRIVER_ASSERT(0);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SPI_Transfer(enum spi_id id,
                         uint8_t *out, uint32_t out_len,
                         uint8_t *in, uint32_t in_len,
                         enum spi_io_type io_type)
{
    struct spi_ctrl_block *scb;
    SPI_Type *spi;

    DRIVER_ASSERT(id < SPI_ID_MAX);

		/* Get SPI Register */
    scb = &g_scb[id];
    spi = SPI_REG(id);

		/* Check status */
    if (scb->rx_busy || scb->tx_busy)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Update SPI Parameter */
    if (out_len)
    {
        scb->tx_busy = 1;
    }
    if (in_len)
    {
        scb->rx_busy = 1;
    }
    scb->tx_len = out_len;
    scb->rx_len = in_len;
    scb->rx_count = 0;
    scb->tx_count = 0;
    scb->rx_buf = (uint8_t *)in;
    scb->tx_buf = (uint8_t *)out;

		/* BLOCK TRX */
    if (io_type == SPI_IO_TYPE_BLOCK)
    {
        while (scb->rx_len > scb->rx_count || scb->tx_len > scb->tx_count)
        {
            uint32_t data;

						/* Master mode */
						if (spi->CR & SPI_CR_MS_Msk)
						{
								// Master Transmit
								if (scb->tx_len <= scb->tx_count)
								{
										while (!(SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk) && SPI_StatusCheck(spi, SPI_SR_TRDY_Msk)))
										{
										}
										spi->EN = 0;
										spi->RDR_TDR = 0xFF;	// Dummy Write
										spi->EN = 1;
								}
								else
								{
										while (!(SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk) && SPI_StatusCheck(spi, SPI_SR_TRDY_Msk)))
										{
										}
										spi->EN = 0;
										data = *scb->tx_buf++;
										if (spi->CR & SPI_CR_BITSZ_Msk)
										{
												data |= *(scb->tx_buf++) << 8;
												if ((spi->CR & SPI_CR_BITSZ_Msk) == SPI_CR_BITSZ_Msk)
												{
														data |= *(scb->tx_buf++) << 16;
												}
										}
										spi->RDR_TDR = data;	// Tx Data Write
										spi->EN = 1;
										scb->tx_count++;
								}

								// Master Receive
								while (!SPI_StatusCheck(spi, SPI_SR_RRDY_Msk))
								{
								}
								data = spi->RDR_TDR;

								if (scb->rx_len > scb->rx_count)
								{
										*(scb->rx_buf++) = (uint8_t)data;

										if (spi->CR & SPI_CR_BITSZ_Msk)
										{
												*(scb->rx_buf++) = (uint8_t)(data >> 8);
												if ((spi->CR & SPI_CR_BITSZ_Msk) == SPI_CR_BITSZ_Msk)
												{
														*(scb->rx_buf++) = (uint8_t)(data >> 16);
												}
										}
										scb->rx_count++;
								}
						}
						/* Slave mode */
						else
						{
								spi->EN = 1;
								while (!SPI_StatusCheck(spi, SPI_SR_RRDY_Msk))
								{
								}
								data = spi->RDR_TDR;

								// Slave Receive
								if (scb->rx_len > scb->rx_count)
								{
										*(scb->rx_buf++) = (uint8_t)data;

										if (spi->CR & SPI_CR_BITSZ_Msk)
										{
												*(scb->rx_buf++) = (uint8_t)(data >> 8);
												if ((spi->CR & SPI_CR_BITSZ_Msk) == SPI_CR_BITSZ_Msk)
												{
														*(scb->rx_buf++) = (uint8_t)(data >> 16);
												}
										}
										scb->rx_count++;
								}
								// Slave Transmit
								if (scb->tx_len > scb->tx_count)
								{
										while (!(SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk) && SPI_StatusCheck(spi, SPI_SR_TRDY_Msk)))
										{
										}
										spi->EN = 0;
										data = *scb->tx_buf++;
										if (spi->CR & SPI_CR_BITSZ_Msk)
										{
												data |= *(scb->tx_buf++) << 8;
												if ((spi->CR & SPI_CR_BITSZ_Msk) == SPI_CR_BITSZ_Msk)
												{
														data |= *(scb->tx_buf++) << 16;
												}
										}
										spi->RDR_TDR = data;	// Tx Data Write
										spi->EN = 1;
										scb->tx_count++;
								}
						}
        }

        scb->rx_busy = 0;
        scb->tx_busy = 0;

        if (SPI_StatusCheck(spi, SPI_SR_OVRF_Msk)
            || SPI_StatusCheck(spi, SPI_SR_UDRF_Msk))
        {
            scb->tx_busy = 0;
            return DRIVER_ERROR_HW;
        }
    }

		/* NON-BLOCK TRX */
    else
    {
        if ((scb->tx_dma_enable) || (scb->rx_dma_enable))
        {
						if (spi->CR & SPI_CR_MS_Msk)
						{
								spi->EN = 1;
								spi->CR |= (1 << SPI_CR_DTXIE_Pos) | (1 << SPI_CR_DRXIE_Pos);
								HAL_DMA_Start(scb->tx_dma_ch, (uint32_t)scb->tx_buf, (uint32_t)spi, scb->tx_len);
								HAL_DMA_Start(scb->rx_dma_ch, (uint32_t)spi, (uint32_t)scb->rx_buf, scb->rx_len);
						}
						else
						{
								if (scb->tx_len)
								{
										spi->EN = 1;
										spi->CR |= (1 << SPI_CR_DTXIE_Pos);
										HAL_DMA_Start(scb->tx_dma_ch, (uint32_t)scb->tx_buf, (uint32_t)spi, scb->tx_len);
								}
								else if (scb->rx_len)
								{
										spi->EN = 1;
										spi->CR |= (1 << SPI_CR_DRXIE_Pos);
										HAL_DMA_Start(scb->rx_dma_ch, (uint32_t)spi, (uint32_t)scb->rx_buf, scb->rx_len);
								}
						}
        }
        else
        {
						/* Master mode */
						if (spi->CR & SPI_CR_MS_Msk)
						{
								spi->CR |= (1 << SPI_CR_TXIE_Pos);
								spi->CR &= ~(1 << SPI_CR_RXIE_Pos);
						}
						/* Slave mode */
						else
						{
								spi->EN = 1;
								spi->CR &= ~(1 << SPI_CR_TXIE_Pos);
								spi->CR |= (1 << SPI_CR_RXIE_Pos);
						}
        }
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SPI_SetTransmitDMA(enum spi_id id, enum dma_ch ch, uint8_t enable)
{
    struct spi_ctrl_block *scb;
    SPI_Type *spi;

    DRIVER_ASSERT(id < SPI_ID_MAX);

		/* Get SPI Register */
    scb = &g_scb[id];
    spi = SPI_REG(id);

    scb->tx_dma_enable = enable;
    scb->tx_dma_ch = ch;

		/* Configure TX DMA */
    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = SPI_TX_DMA_PERI(id),
            .size = DMA_BUS_SIZE_BYTE,
            .dir = DMA_DIR_MEM_TO_PERI,
        };

        switch (spi->CR & 0x3UL)
        {
            case SPI_DATA_BIT_8:
                cfg.size = DMA_BUS_SIZE_BYTE;
                break;
            case SPI_DATA_BIT_16:
                cfg.size = DMA_BUS_SIZE_HALFWORD;
                break;
            default:
                DRIVER_ASSERT(0);
        }

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
		else
		{
				HAL_DMA_Clear(scb->tx_dma_ch);
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SPI_SetReceiveDMA(enum spi_id id, enum dma_ch ch, uint8_t enable)
{
    struct spi_ctrl_block *scb;
    SPI_Type *spi;

    DRIVER_ASSERT(id < SPI_ID_MAX);

		/* Get SPI Register */
    scb = &g_scb[id];
    spi = SPI_REG(id);

    scb->rx_dma_enable = enable;
    scb->rx_dma_ch = ch;

		/* Configure RX DMA */
    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = SPI_RX_DMA_PERI(id),
            .size = DMA_BUS_SIZE_BYTE,
            .dir = DMA_DIR_PERI_TO_MEM,
        };

        switch (spi->CR & 0x3UL)
        {
            case SPI_DATA_BIT_8:
                cfg.size = DMA_BUS_SIZE_BYTE;
                break;
            case SPI_DATA_BIT_16:
                cfg.size = DMA_BUS_SIZE_HALFWORD;
                break;
            default:
                DRIVER_ASSERT(0);
        }

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
		else
		{
				HAL_DMA_Clear(scb->rx_dma_ch);
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void SPI_IRQHandler(enum spi_id id)
{
    struct spi_ctrl_block *scb;
    SPI_Type *spi;
    volatile uint32_t data;
    volatile uint32_t event = 0;

    scb = &g_scb[id];
    spi = SPI_REG(id);

    if (!(spi->CR & SPI_CR_MS_Msk))
    {
        if (SPI_StatusCheck(spi, SPI_SR_SSDET_Msk))
        {
            spi->SR &= ~SPI_SR_SSDET_Msk;
        }
    }

		/* DMA Operation */
    if (SPI_StatusCheck(spi, SPI_SR_TXDMAF_Msk)
        && SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk))
    {
        spi->SR &= ~SPI_SR_TXDMAF_Msk;
        scb->tx_busy = 0;
				scb->tx_dma_enable = 0;
				spi->CR &= ~SPI_CR_DTXIE_Msk;

        if (scb->handler && !scb->rx_len)
        {
            scb->handler(SPI_EVENT_TRANSFER_DONE, scb->context);
        }

        return;
    }

    if (SPI_StatusCheck(spi, SPI_SR_RXDMAF_Msk))
    {
        spi->SR &= ~SPI_SR_RXDMAF_Msk;
        scb->rx_busy = 0;
				scb->rx_dma_enable = 0;
				spi->CR &= ~SPI_CR_DRXIE_Msk;

        if (scb->handler)
        {
            scb->handler(SPI_EVENT_TRANSFER_DONE, scb->context);
        }

        return;
    }

    if (scb->rx_dma_enable || scb->tx_dma_enable)
    {
        return;
    }

		/* Master Mode */
		if (spi->CR & SPI_CR_MS_Msk)
		{
				// Master Send
				if (scb->tx_count < scb->tx_len)
				{
						if (SPI_StatusCheck(spi, SPI_SR_TRDY_Msk) && SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk))
						{
								if (scb->tx_count == scb->rx_count)
								{
										data = *scb->tx_buf++;
										if (spi->CR & SPI_CR_BITSZ_Msk)
										{
												data |= *(scb->tx_buf++) << 8;
												if ((spi->CR & SPI_CR_BITSZ_Msk) == SPI_CR_BITSZ_Msk)
												{
														data |= *(scb->tx_buf++) << 16;
												}
										}
										spi->EN = 0;
										spi->RDR_TDR = data;	// ## Data Write ## //
										spi->EN = 1;
										scb->tx_count++;
								}
						}
						if (SPI_StatusCheck(spi, SPI_SR_RRDY_Msk))
						{
								data = spi->RDR_TDR;	// ## Data Read ## //
								scb->rx_count++;
						}
				}
				// Master Recv
				else if (scb->rx_count < scb->rx_len)
				{
						if (SPI_StatusCheck(spi, SPI_SR_TRDY_Msk) && SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk))
						{
								if (scb->tx_count == scb->rx_count)
								{
										data = 0xFF;
										spi->EN = 0;
										spi->RDR_TDR = data;	// ## Data Write ## //
										spi->EN = 1;
										scb->tx_count++;
								}
						}

						if (SPI_StatusCheck(spi, SPI_SR_RRDY_Msk))
						{
								data = spi->RDR_TDR;	// ## Data Read ## //
								*(scb->rx_buf++) = (uint8_t)data;
								scb->rx_count++;
						}
				}
				// Complete
				else
				{
						scb->rx_busy = 0;
						scb->tx_busy = 0;
						spi->CR &= ~(SPI_CR_RXIE_Msk | SPI_CR_TXIE_Msk);

						if (scb->handler)
						{
								if (SPI_StatusCheck(spi, SPI_SR_OVRF_Msk))
								{
										event |= SPI_EVENT_OVERRUN;
								}

								if (SPI_StatusCheck(spi, SPI_SR_UDRF_Msk))
								{
										event |= SPI_EVENT_UNDERRUN;
								}

								event |= SPI_EVENT_TRANSFER_DONE;
						}
				}
		}
		/* Slave Mode */
		else
		{
				if (SPI_StatusCheck(spi, SPI_SR_RRDY_Msk))
				{
						data = spi->RDR_TDR;	// ## Data Read ## //

						// Slave Recv
						if (scb->rx_count < scb->rx_len)
						{
								*(scb->rx_buf++) = (uint8_t)data;
								scb->rx_count++;
								if (SPI_StatusCheck(spi, SPI_SR_TRDY_Msk) && SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk))
								{
										spi->RDR_TDR = 0x00;	// ## Data Write ## //
								}

								// Complete
								if (scb->rx_count >= scb->rx_len)
								{
										scb->rx_busy = 0;
										scb->tx_busy = 0;
										spi->CR &= ~(SPI_CR_RXIE_Msk | SPI_CR_TXIE_Msk);

										if (scb->handler)
										{
												if (SPI_StatusCheck(spi, SPI_SR_OVRF_Msk))
												{
														event |= SPI_EVENT_OVERRUN;
												}

												if (SPI_StatusCheck(spi, SPI_SR_UDRF_Msk))
												{
														event |= SPI_EVENT_UNDERRUN;
												}

												event |= SPI_EVENT_TRANSFER_DONE;
										}
								}
						}
						// Slave Send
						else if (scb->tx_count < scb->tx_len)
						{
								if (SPI_StatusCheck(spi, SPI_SR_TRDY_Msk) && SPI_StatusCheck(spi, SPI_SR_TXIDLE_Msk))
								{
										data = *scb->tx_buf++;
										spi->EN = 0;
										spi->RDR_TDR = data;	// ## Data Write ## //
										spi->EN = 1;
										scb->tx_count++;
								}

								// Complete
								if (scb->tx_count >= scb->tx_len)
								{
										scb->rx_busy = 0;
										scb->tx_busy = 0;
										spi->CR &= ~(SPI_CR_RXIE_Msk | SPI_CR_TXIE_Msk);

										if (scb->handler)
										{
												if (SPI_StatusCheck(spi, SPI_SR_OVRF_Msk))
												{
														event |= SPI_EVENT_OVERRUN;
												}

												if (SPI_StatusCheck(spi, SPI_SR_UDRF_Msk))
												{
														event |= SPI_EVENT_UNDERRUN;
												}

												event |= SPI_EVENT_TRANSFER_DONE;
										}
								}
						}
				}
		}

		/* Callback Function */
    if (scb->handler && event)
    {
        scb->handler(event, scb->context);
    }
}

//////////////////////////////////////////////////////////////////////////////////////
void SPI20_IRQHandler(void)
{
    SPI_IRQHandler(SPI_ID_20);
}

void SPI21_IRQHandler(void)
{

    SPI_IRQHandler(SPI_ID_21);
}

