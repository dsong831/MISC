/**
 *******************************************************************************
 * @file        a31t21x_wt.c
 * @brief       wt driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_wt.h"


struct wt_ctrl_block
{
		enum wt_irq_type			irq_status;
    wt_irq_handler_t			handler;
    void *								context;
};

static struct wt_ctrl_block g_wcb;

//////////////////////////////////////////////////////////////////////////////////////
static int32_t WT_Configure(struct wt_ctrl_block *wcb, struct wt_cfg *cfg)
{
		/* Get WT parameter */
    wcb = &g_wcb;

		/* Configure WT Clock Selection */
    switch (cfg->clk_sel)
    {
        case WT_CLK_MCCLK:		// default LSI 500kHz
						SCU->PPCLKSR &= ~(3 << 3);
						SCU->PPCLKSR |= (WT_CLK_MCCLK << 3);

						SCU->MCCR3 &= ~(0x7FF << 16);
						SCU->MCCR3 |= (1 << 16);
						break;
        case WT_CLK_LSE:			// LSE 32.768kHz
						SCU->PPCLKSR &= ~(3 << 3);
						SCU->PPCLKSR |= (WT_CLK_LSE << 3);
						break;
        case WT_CLK_WDTRC:		// WDTRC 31.250kHz
						SCU->PPCLKSR &= ~(3 << 3);
						SCU->PPCLKSR |= (WT_CLK_WDTRC << 3);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

		/* Check WT Interval */
    switch (cfg->interval_sel)
    {
        case WT_INTERVAL_DIV_2_7:
						WT->CR &= ~(3 << 4);
						WT->CR |= (WT_INTERVAL_DIV_2_7 << 4);
						break;
        case WT_INTERVAL_DIV_2_13:
						WT->CR &= ~(3 << 4);
						WT->CR |= (WT_INTERVAL_DIV_2_13 << 4);
						break;
        case WT_INTERVAL_DIV_2_14:
						WT->CR &= ~(3 << 4);
						WT->CR |= (WT_INTERVAL_DIV_2_14 << 4);
            break;
        case WT_INTERVAL_DIV_2_14_WTDR:
						WT->CR &= ~(3 << 4);
						WT->CR |= (WT_INTERVAL_DIV_2_14_WTDR << 4);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

		/* Configure Interrupt */
		if (wcb->irq_status)
		{
				WT->CR |= (1 << 3);		// Enable WT Interrupt
		}
		else
		{
				WT->CR &= ~(1 << 3);	// Disable WT Interrupt
		}

		/* Clear Counter */
		WT->CR |= (1 << 0);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_Init(struct wt_cfg *cfg, wt_irq_handler_t handler, void *context)
{
    struct wt_ctrl_block *wcb;
    int32_t ret;

    DRIVER_ASSERT(cfg != NULL);

		/* Get WT parameter */
    wcb = &g_wcb;

    memset(wcb, 0, sizeof(struct wt_ctrl_block));

    wcb->handler = handler;
    wcb->context = context;

		/* WT Block Enable */
		SCU->PER1 &= ~(0x01UL << 31);
		SCU->PER1 |= (0x01UL << 31);
		SCU->PCER1 &= ~(0x01UL << 31);
		SCU->PCER1 |= (0x01UL << 31);
		WT->CR = 0;		

		/* Configure wt Parameter */
    ret = WT_Configure(wcb, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_WT_Deinit();
        return ret;
    }

		/* Set NVIC */
    NVIC_ClearPendingIRQ(WT_IRQn);
    NVIC_SetPriority(WT_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(WT_IRQn);

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_Deinit(void)
{
		/* WT Block Disable */
		SCU->PER1 &= ~(0x01UL << 31);
		SCU->PCER1 &= ~(0x01UL << 31);
		WT->CR = 0;

		/* Clear NVIC */
    NVIC_DisableIRQ(WT_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_GetStatus(uint8_t *status)
{
		/* Get Interrupt Flags */
    *status = WT->CR & (1 << 1);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_Start(uint16_t count)
{
		/* Set Data Parameter */
    WT->DR = count;

		/* Counter clear and start */
    WT->CR |= (1 << 7);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_Stop(void)
{
		/* Disable wt operation */
    WT->CR &= ~(1 << 7);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_GetCount(uint16_t *count)
{
    *count = WT->CNT;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_ClearCount(void)
{
    WT->CR |= (1 << 0);		// Clear wt Counter

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_SetClock(enum wt_clk_sel clk_sel, enum wt_mcclk_sel mcclk_sel, uint8_t mcclk_div)
{
		uint32_t reg_val;

		/* Set WT Clock Source */
		if (clk_sel == WT_CLK_MCCLK)
		{
				// Select MCCR Source
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(3 << 3);
				SCU->PPCLKSR = reg_val;

				// Set MCCR Clock
				SCU->MCCR3 &= ~(0x7FF << 16);
				SCU->MCCR3 |= (mcclk_sel << 24);
				SCU->MCCR3 |= (mcclk_div << 16);
		}
		else if (clk_sel == WT_CLK_LSE)
		{
				// Select LSE Source (32.768kHz)
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(3 << 3);
				reg_val |= (1 << 3);
				SCU->PPCLKSR = reg_val;
		}
		else
		{
				// Select WDTRC Source (31.250kHz = LSI/16)
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(3 << 3);
				reg_val |= (2 << 3);
				SCU->PPCLKSR = reg_val;
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WT_SetIRQ(enum wt_irq_type type, uint8_t enable)
{
		/* Set WT Interrupt */
    switch (type)
    {
				case WT_IRQ_COUNT_MATCH:
						if (enable)
						{
								WT->CR |= (1 << 3);
						}
						else
						{
								WT->CR &= ~(1 << 3);
						}
						break;
				case WT_IRQ_NONE:
						WT->CR &= ~(1 << 3);
						break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void WT_IRQHandlerC(void)
{
    struct wt_ctrl_block *wcb;
    volatile uint32_t event = 0;
    volatile uint8_t status;

		/* Get WT parameter */
    wcb = &g_wcb;

		/* Get WT Interrupt Flags */
    status = (WT->CR & (1 << 1));
    WT->CR |= status;	// Clear Flags

    if (wcb->handler)
    {
        if (status)
        {
						event |= WT_EVENT_COUNT_MATCH;
        }

				/* Callback function */
        if (event)
        {
            wcb->handler(event, wcb->context);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////
void WT_IRQHandler(void)
{
		WT_IRQHandlerC();
}

