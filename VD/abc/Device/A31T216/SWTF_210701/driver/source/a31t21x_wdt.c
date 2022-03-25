/**
 *******************************************************************************
 * @file        a31t21x_wdt.c
 * @brief       WDT driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_wdt.h"


#define WDT_LOAD_MAX        0xFFFFFF

static uint8_t gu8ReloadCount = 0;	// For window data function

struct wdt_ctrl_block
{
    wdt_irq_handler_t   handler;
    void *              context;
    uint32_t            counter;
    uint32_t            window_count;
};

static struct wdt_ctrl_block g_wcb;

//////////////////////////////////////////////////////////////////////////////////////
static int32_t WDT_Configure(struct wdt_cfg *cfg)
{
    uint32_t reg_cfg = 0;

		/* Set WDT Clock Source */
    if (cfg->clk_src == WDT_CLK_SRC_MCCLK)
    {
        SCU->PPCLKSR |= (1 << 0);			// To Use MCCR3 Clock Source (Default LSI 500kHz)
				SCU->MCCR3 &= ~(0x7ff << 0);
				SCU->MCCR3 |= (1 << 0);
    }
		else
		{
        SCU->PPCLKSR &= ~(1 << 0);		// To Use WDTRC Clock Source (31.250kHz = LSI/16)
		}

		/* Set Clock Divider */
    if (cfg->clk_div > WDT_CLK_DIV_256)
    {
        return DRIVER_ERROR_PARAMETER;
    }
    reg_cfg |= (cfg->clk_div << 0);

		/* Set WDT Mode */
    if (cfg->mode == WDT_MODE_RESET)
    {
        reg_cfg &= ~(0x3f << 10);
				reg_cfg &= ~(3 << 2);
				reg_cfg |= (0x3f << 10);						// Enable Reset
    }
    else if (cfg->mode == WDT_MODE_IRQ)
    {
        reg_cfg &= ~(0x3f << 10);
				reg_cfg &= ~(3 << 2);
        reg_cfg |= (0x25 << 10) | (1 << 2);	// Enable Underflow Interrupt
    }
    else
    {
        return DRIVER_ERROR_PARAMETER;
    }
		WDT->CR = (0x5A69 << 16) | (0x1A << 4) | reg_cfg;

		/* Set Window Data */
		if (cfg->window_count != 0)
		{
				g_wcb.window_count = cfg->window_count;
		}
		else
		{
				g_wcb.window_count = 0x00FFFFFF;
		}

		/* Set NVIC */
    NVIC_ClearPendingIRQ(WDT_IRQn);
    NVIC_SetPriority(WDT_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(WDT_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_Init(struct wdt_cfg *cfg, wdt_irq_handler_t handler, void *context)
{
    DRIVER_ASSERT(cfg != NULL);

    if (cfg->mode == WDT_MODE_IRQ)
    {
        DRIVER_ASSERT(handler);
    }

    g_wcb.handler = handler;
    g_wcb.context = context;

    WDT_Configure(cfg);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_Deinit(void)
{
    memset(&g_wcb, 0, sizeof(struct wdt_ctrl_block));

		/* Disable WDT Reset & IRQ */
    WDT->CR = (0x5A69 << 16) | (0x25 << 10) | (0x1A << 4);

		/* Clear NVIC */
    NVIC_DisableIRQ(WDT_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_Start(uint32_t counter)
{
    uint32_t reg_cfg = 0;

		g_wcb.counter = counter;

		/* Check Window Data */
		if(g_wcb.counter > g_wcb.window_count)
		{
				WDT->DR = g_wcb.counter & 0x00FFFFFF;
				WDT->WINDR = g_wcb.window_count & 0x00FFFFFF;

				// Enable window data match interrupt
				reg_cfg = WDT->CR;
				reg_cfg &= ~(0x3f << 4);
				reg_cfg |= (1 << 3);
				WDT->CR = (0x5A69 << 16) | (0x1A << 4) | reg_cfg;
		}
		else
		{
				WDT->DR = g_wcb.counter & 0x00FFFFFF;
				WDT->WINDR = 0x00FFFFFF;
		}

		/* Enable WDT Counter */
		reg_cfg = WDT->CR;
		reg_cfg &= ~(0x3f << 4);
		WDT->CR = (0x5A69 << 16) | reg_cfg;
		
    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_Stop(void)
{
    uint32_t reg_cfg = 0;

		reg_cfg = WDT->CR;
		reg_cfg &= ~(0x3f << 4);
		WDT->CR = (0x5A69 << 16) | (0x1A << 4) | reg_cfg;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_Feed(void)
{
		if(!(WDT->CR & (0x1A<<4)))
		{
			if(gu8ReloadCount == 0)
			{
				while(WDT->CNT >= WDT->WINDR);	// if it is first time to reload, you should wait until counter value is lower than window data value.
				gu8ReloadCount = 1;
			}
		}
		WDT->CNTR = 0x6A;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_GetCounter(uint32_t *counter)
{
    *counter = WDT->CNT;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_ClearStatus(uint32_t int_flag)
{
    uint32_t reg_cfg;

		reg_cfg = (WDT->SR & 0xFF);

		if ((reg_cfg & (1 << 0)) || (reg_cfg & (1 << 1)))
		{
				WDT->SR = int_flag | 0x80;
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_WDT_SetClock(enum wdt_clk_src clk_src, enum wdt_mcclk_sel mcclk_sel, uint8_t mcclk_div)
{
		/* Set WDT Clock Source */
		if (clk_src == WDT_CLK_SRC_MCCLK)
		{
				// Select MCCR Source
				SCU->PPCLKSR |= (1 << 0);			// To Use MCCR3 Clock Source

				// Set MCCR3 Clock
				SCU->MCCR3 &= ~(0x7ff << 0);
				SCU->MCCR3 |= (mcclk_sel << 8);
				SCU->MCCR3 |= (mcclk_div << 0);
		}
		else
		{
				SCU->PPCLKSR &= ~(1 << 0);		// To Use WDTRC Clock Source (31.250kHz = LSI/16)
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
void WDT_IRQHandlerC(unsigned long *args)
{
		/* Clear Window Match Interrupt */
		if (WDT->SR & (1 << 1))
		{
				HAL_WDT_ClearStatus((1 << 1));
		}
		
		/* Clear Underflow Interrupt */
		if (WDT->SR & (1 << 0))
		{
				HAL_WDT_ClearStatus((1 << 0));
		}

		/* Callback function */
    if (g_wcb.handler)
    {
        g_wcb.handler(args, g_wcb.context);
    }
}

