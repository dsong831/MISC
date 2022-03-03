/**
 *******************************************************************************
 * @file        a34m41x_dma.c
 * @brief       DMA driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_dma.h"


#define DMA_REG_OFFSET  0x10

static __INLINE DMA_Type *DMA_REG(enum dma_ch ch)
{
    return (DMA_Type *)(DMA0_BASE + (DMA_REG_OFFSET * ch));
}

static uint8_t g_dma_ref_count = 0;
static uint8_t g_dma_setup[DMA_ID_MAX];

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_DMA_Init(void)
{
    if (g_dma_ref_count == DMA_ID_MAX)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    if (g_dma_ref_count == 0)
    {
        SCU->PER1 &= ~(1UL << 4);
        SCU->PCER1 &= ~(1UL << 4);
        SCU->PER1 |= (1UL << 4);
        SCU->PCER1 |= (1UL << 4);
    }

    g_dma_ref_count++;


    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_DMA_Deinit(void)
{
    if (g_dma_ref_count == 0)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    g_dma_ref_count--;

    if (g_dma_ref_count == 0)
    {
        SCU->PER1 &= ~(1UL << 4);
        SCU->PCER1 &= ~(1UL << 4);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_DMA_Setup(enum dma_ch ch, struct dma_cfg *cfg)
{
    DMA_Type *reg;

    DRIVER_ASSERT(ch < DMA_ID_MAX);

    if (g_dma_setup[ch] != 0)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Get Register Name */
    reg = DMA_REG(ch);

		/* Configure DMA */
    reg->CR = 0;
    reg->CR = ((cfg->peri << 8) & (0x0F << 8)) |
              ((cfg->size << 2) & (0x03 << 2)) |
              ((cfg->dir << 1) & (0x01 << 1));

    g_dma_setup[ch] = 1;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_DMA_Clear(enum dma_ch ch)
{
    DRIVER_ASSERT(ch < DMA_ID_MAX);

    g_dma_setup[ch] = 0;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_DMA_Start(enum dma_ch ch, uint32_t src, uint32_t dst, uint32_t len)
{
    DMA_Type *reg;

    DRIVER_ASSERT(ch < DMA_ID_MAX);
    DRIVER_ASSERT(((len << 16) & ~(0xfff << 16)) == 0);

		/* Get Register Name */
    reg = DMA_REG(ch);

		/* Configure DMA bus transfer size */
    reg->CR &= ~(0xfff << 16);
    reg->CR |= (len << 16);

		/* DMA TX */
    if (((reg->CR & 0x02) >> 1) == DMA_DIR_MEM_TO_PERI)
    {
        reg->PAR = dst;
        reg->MAR = src;
    }
		/* DMA RX */
    else
    {
        reg->PAR = src;
        reg->MAR = dst;
    }

		/* DMA Start */
    reg->SR |= (1 << 0);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_DMA_Stop(enum dma_ch id)
{
    DMA_Type *reg;

    DRIVER_ASSERT(id < DMA_ID_MAX);

		/* Get Register Name */		
    reg = DMA_REG(id);

		/* DMA Disable */
    reg->SR &= ~(1 << 0);

    return DRIVER_ERROR_OK;
}

