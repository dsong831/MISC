/**
 *******************************************************************************
 * @file        a31t21x_led.c
 * @brief       led driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_led.h"


struct led_ctrl_block
{
    led_irq_handler_t			handler;
    void *								context;
};

static struct led_ctrl_block g_lcb;

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LED_Init(struct led_cfg *cfg, led_irq_handler_t handler, void *context)
{
		DRIVER_ASSERT(cfg != NULL);

		g_lcb.handler = handler;
		g_lcb.context = context;

		/* LED Block Enable */
		SCU->PER2 &= ~(1 << 29);
		SCU->PCER2 &= ~(1 << 29);
		SCU->PER2 |= (1 << 29);
		SCU->PCER2 |= (1 << 29);

		/* LED Period Configure */
		LED->PRESD = (cfg->tPRESD & 0xFF);
		LED->COMPWID = (cfg->tCOMPWID & 0xFF);
		LED->LEDPD = (cfg->tLEDPD & 0xFFFFF);

		/* Set LED Interrupt */
		LED->SR = 0;
		if(cfg->tSR_INTE)
		{
			LED->SR |= (cfg->tSR_INTE << 1);
			NVIC_SetPriority(LED_IRQn, 3);
			NVIC_EnableIRQ(LED_IRQn);
		}
		else
		{
			LED->SR &= ~(cfg->tSR_INTE << 1);
		}

		/* Set Overlap time */
		if(cfg->tOVERLAP < 8)
		{
			LED->LEDCON2 &= ~(1<<3);
			LED->LEDCON2 |= (cfg->tOVERLAP << 0);
		}
		else
		{
			LED->LEDCON2 |= (1<<3);	// Overlap time = LED_CLK / 2
		}

		/* Set LED mode */
		LED->LEDCON1 = (cfg->tMODE << 2) | (1<<1);	// Mode setting & LED enable

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LED_DISP_ON(void)
{
		LED->LEDCON1 |= (1<<0);	// Start LED

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LED_DISP_OFF(void)
{
		LED->LEDCON1 &= ~(1<<0);	// Stop LED

		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LED_SetClock(enum led_clk_sel clk_sel, enum led_mcclk_sel mcclk_sel, uint8_t mcclk_div)
{
		uint32_t reg_val;

		/* Set WT Clock Source */
		if (clk_sel == LED_CLK_MCCLK)
		{
				// Select MCCR Source
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(1 << 10);
				SCU->PPCLKSR = reg_val;

				// Set MCCR Clock
				SCU->MCCR5 &= ~(0x7FF << 0);
				SCU->MCCR5 |= (mcclk_sel << 8);
				SCU->MCCR5 |= (mcclk_div << 0);
		}
		else
		{
				// Select PCLK Source
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(1 << 10);
				reg_val |= (1 << 10);
				SCU->PPCLKSR = reg_val;
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void LED_IRQHandlerC(void)
{
    volatile uint32_t event = 0;
    volatile uint8_t status;

		/* Get LED Interrupt Flags */
		status = (LED->SR & (1 << 2));
		LED->SR &= ~(1 << 2);		// Clear Flags

    if (g_lcb.handler)
    {
        if (status)
        {
						event |= LED_EVENT_DONE;
        }

				/* Callback function */
        if (event)
        {
            g_lcb.handler(event, g_lcb.context);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////
void LED_IRQHandler(void)
{
		LED_IRQHandlerC();
}


















