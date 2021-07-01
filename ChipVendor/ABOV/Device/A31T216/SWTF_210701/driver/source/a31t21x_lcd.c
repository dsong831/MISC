/**
 *******************************************************************************
 * @file        a31t21x_lcd.c
 * @brief       lcd driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_lcd.h"


struct lcd_ctrl_block
{
    lcd_irq_handler_t			handler;
    void *								context;
};

static struct lcd_ctrl_block g_lcb;

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LCD_Init(struct lcd_cfg *cfg, lcd_irq_handler_t handler, void *context)
{
		DRIVER_ASSERT(cfg != NULL);

		g_lcb.handler = handler;
		g_lcb.context = context;

		/* LCD Block Enable */
		SCU->PER2 &= ~(1 << 28);
		SCU->PCER2 &= ~(1 << 28);
		SCU->PER2 |= (1 << 28);
		SCU->PCER2 |= (1 << 28);

		/* Configure LCD Parameter */
		LCD->CR = cfg->lcddr		// LCD Driving Resistor (Internal or External)
				| cfg->irsel				// Internal LCD Bias (RLCD1 ~ RLCD4)
				| cfg->dbs					// LCD Duty & Bias
				| cfg->lclk					// LCD Clock Selection (128Hz ~ 1024Hz)
				;

		LCD->BCCR = cfg->lcdabc	// LCD Automatic bias On/Off
				| cfg->bmsel				// Bias mode time selection
				| cfg->lcten				// enable lcd driver contrast
				| cfg->vlcd					// VLC0 Voltage Control
				;

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LCD_DISP_ON(void)
{
		volatile uint32_t event = 0;

		LCD->CR |= (1 << 0);
		event |= LCD_EVENT_DONE;

		/* Callback Function */
		if (g_lcb.handler && event)
		{
				g_lcb.handler(event, g_lcb.context);
		}

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LCD_DISP_OFF(void)
{
		volatile uint32_t event = 0;

		LCD->CR &= ~(1 << 0);
		event |= LCD_EVENT_ERROR;

		/* Callback Function */
		if (g_lcb.handler && event)
		{
				g_lcb.handler(event, g_lcb.context);
		}

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LCD_SetClock(enum lcd_clk_sel clk_sel, enum lcd_mcclk_sel mcclk_sel, uint8_t mcclk_div)
{
		uint32_t reg_val;

		/* Set WT Clock Source */
		if (clk_sel == LCD_CLK_MCCLK)
		{
				// Select MCCR Source
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(3 << 6);
				SCU->PPCLKSR = reg_val;

				// Set MCCR Clock
				SCU->MCCR5 &= ~(0x7FF << 16);
				SCU->MCCR5 |= (mcclk_sel << 24);
				SCU->MCCR5 |= (mcclk_div << 16);
		}
		else if (clk_sel == LCD_CLK_LSE)
		{
				// Select LSE Source (32.768kHz)
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(3 << 6);
				reg_val |= (1 << 6);
				SCU->PPCLKSR = reg_val;
		}
		else
		{
				// Select WDTRC Source (31.250kHz = LSI/16)
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(3 << 6);
				reg_val |= (2 << 6);
				SCU->PPCLKSR = reg_val;
		}

    return DRIVER_ERROR_OK;
}

