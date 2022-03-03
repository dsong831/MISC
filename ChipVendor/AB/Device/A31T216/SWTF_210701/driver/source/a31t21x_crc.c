/**
 *******************************************************************************
 * @file        a31t21x_crc.c
 * @brief       CRC driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_crc.h"

struct crc_ctrl_block
{
    crc_irq_handler_t   handler;
    void *              context;
    uint32_t *          value;
    enum dma_ch         dma_ch;
    uint8_t             dma_enable;
};

static struct crc_ctrl_block g_ccb;
static uint16_t crc_table[256];			

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_CRC_Init(struct crc_cfg *cfg, crc_irq_handler_t handler, void *context)
{
    uint32_t ctrl = 0;

    DRIVER_ASSERT(cfg != NULL);

    SCU->PER2 &= ~(1UL << 31);
    SCU->PCER2 &= ~(1UL << 31);
    SCU->PER2 |= (1UL << 31);
    SCU->PCER2 |= (1UL << 31);

		if(cfg->poly == CRC_POLY_CCITT)
		{
				ctrl &= ~(1 << 5);
				ctrl &= ~(1 << 4);
		}
		else if (cfg->poly == CRC_POLY_CRC16)
		{
				ctrl &= ~(1 << 5);
				ctrl |= (1 << 4);
		}
		else	// CRC_POLY_CHECKSUM
		{
				ctrl |= (1 << 5);
		}

		if (cfg->mode == CRC_AUTO_MODE)
		{
				ctrl |= (1 << 7);
		}
		else	// CRC_USER_MODE
		{
				ctrl &= ~(1 << 7);
		}

		if (cfg->bit == CRC_LSB_FIRST)
		{
				ctrl |= (1 << 1);
		}
		else	// CRC_MSB_FIRST
		{
				ctrl &= ~(1 << 1);
		}

		if (cfg->block != CRC_IO_TYPE_BLOCK)
		{
				ctrl |= (1 << 9);
		}
		else	// Polling
		{
				ctrl &= ~(1 << 9);
		}
		
		CRC->CR = ctrl;
		
    g_ccb.handler = handler;
    g_ccb.context = context;
		
    NVIC_ClearPendingIRQ(CRC_IRQn);
    NVIC_SetPriority(CRC_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(CRC_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_CRC_Deinit(void)
{
    SCU->PER2 &= ~(1UL << 31);
    SCU->PCER2 &= ~(1UL << 31);

    memset(&g_ccb, 0, sizeof(struct crc_ctrl_block));

    NVIC_DisableIRQ(CRC_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_CRC_SetDMA(enum dma_ch ch, uint8_t enable)
{
    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = DMA_PERI_CRC_TX,
            .size = DMA_BUS_SIZE_WORD,
            .dir = DMA_DIR_MEM_TO_PERI,
        };

        if (g_ccb.dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        g_ccb.dma_enable = 1;
        g_ccb.dma_ch = ch;

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
        if (!g_ccb.dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        g_ccb.dma_enable = 0;
        HAL_DMA_Clear(g_ccb.dma_ch);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_CRC_Compute(uint32_t init, uint32_t *data, uint32_t len, uint32_t *value)
{
    uint32_t i;

    CRC->INIT = init;
		CRC->CR |= (0x1UL << 6);
		CRC->CR |= (1 << 0);

    if (g_ccb.dma_enable)
    {
        DRIVER_ASSERT(g_ccb.handler != NULL);

        g_ccb.value = value;
        HAL_DMA_Start(g_ccb.dma_ch, (uint32_t)data, (uint32_t)&CRC->IN, len);
    }
    else
    {
        for (i = 0; i < len; i++)
        {
            CRC->IN = data[i];
        }

        *value = CRC->RLT;

				CRC->CR &= ~(1 << 0);
    }

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_CRC_GetData(uint32_t *value)
{
		*value = CRC->RLT;

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
/* For Software CRC */
//////////////////////////////////////////////////////////////////////////////////////
static void gen_crc_table(uint8_t crc_poly) 
{
	uint16_t i, j;
	uint16_t crc_accum;
	uint16_t polynominal;
	
	if(crc_poly == CRC_POLY_CCITT)				polynominal = 0x1021;
	else if(crc_poly == CRC_POLY_CRC16)	polynominal = 0x8005;
	for( i = 0; i < 256; i++ ) {
		crc_accum = i<<8;
		for (j=0; j<8; j++) {
			if(crc_accum & 0x8000) crc_accum = (crc_accum<<1 ) ^ polynominal;
			else crc_accum <<= 1;
		}
		crc_table[i] = crc_accum;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
uint16_t HAL_CRC_SW_Generate(uint16_t crc_init, uint32_t *data, uint32_t len, uint8_t crc_poly, uint8_t bit) 
{
	uint8_t i;
	uint32_t j;
	uint32_t tmp=0;
	uint32_t *p = data;
	
	if(crc_poly != CRC_POLY_CHECKSUM) {
		gen_crc_table(crc_poly);
		if(bit != CRC_MSB_FIRST) {
			for(j=0;j<len;j++) {
				for(i=0 ; i<32 ; i++) {
					tmp <<= 1;
					if(*p & 0x00000001ul) tmp |= 0x00000001;
					*p >>= 1;
				}
				*p = tmp;
				p++;
			}
		}
	}

	for(j=0; j<len ; j++)	{
		for(i=0 ; i<4 ; i++) {
			if(crc_poly != CRC_POLY_CHECKSUM) {
				crc_init = (crc_init<<8) ^ crc_table[((crc_init>>8) ^ (unsigned short)(*data>>(24-i*8)))&0x00FF];
			}
			else {
				crc_init +=(*data & (0xFF<<(24-i*8)));
			}
		}
		data++;
	}
	
	return crc_init;
}

//////////////////////////////////////////////////////////////////////////////////////
void CRC_IRQHandler(void)
{
    uint32_t status;
    uint32_t event;

    status = (CRC->CR & (1 << 8));

    if (status & (1 << 8))
    {
        *g_ccb.value = CRC->RLT;
        event = CRC_EVENT_DONE;
    }
    else
    {
        event = CRC_EVENT_ERROR;
    }

    if (g_ccb.handler)
    {
        g_ccb.handler(event, g_ccb.context);
    }

    CRC->CR |= status;
}

