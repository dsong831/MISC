/**
 *******************************************************************************
 * @file        a3at21x_system.c
 * @brief       System configuration
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include <stdint.h>
#include "a31t21x.h"

uint32_t SystemCoreClock;		/*!< System Clock Frequency - (Core Clock) HLCK */
uint32_t SystemPeriClock;		/*!< System Clock Frequency - (Peri Clock) PCLK */
uint32_t SystemDelayCount;

void SystemInit(void)
{
	/* Disable Global Interrupt */
	__disable_irq();

	/* WDT Disable */
	WDT->CR = (0x5A69<<16)
		|(0x25<<10)
		|(0x1A<<4);

	/* GPIO Access Enable */
	PORTEN->EN = 0x15;
	PORTEN->EN = 0x51;

	/* Flash Access Time Configure */
	FMC->MR = 0x81;
	FMC->MR = 0x28;
	FMC->CFG = (0x7858 << 16) | (3 << 8);		// Flash Access in 4 cycles (3-wait)
	FMC->MR = 0;
}

void SystemDelay(uint32_t ms)
{
	uint32_t i;

	for(i=0; i<(SystemDelayCount*ms); i++)
	{
		__nop();
	}
}

