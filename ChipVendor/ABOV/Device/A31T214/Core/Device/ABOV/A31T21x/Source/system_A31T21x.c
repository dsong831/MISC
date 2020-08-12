/**********************************************************************//**
* @file				system_A31T21x.c
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			26. JUNE. 2020
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements, 
*modifications, and improvements to ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and 
*ABOV assumes no liability for application assistance or the design of purchasers¡¯ products. No license, 
*express or implied, to any intellectual property rights is granted by ABOV herein. 
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications. 
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners. 
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
**********************************************************************/


/* Includes --------------------------------------------------------------- */
#include "system_A31T21x.h"

/* Public variables ---------------------------------------------------------*/
uint32_t SystemCoreClock;		/*!< System Clock Frequency (Core Clock)  HLCK */
uint32_t SystemPeriClock;		/*!< System Clock Frequency (Peri Clock)  PCLK */


/**********************************************************************//**
 * @brief				System Initialization
 * @param		None
 * @return		None
 **********************************************************************/
void SystemInit(void)
{
	__disable_irq();	// Disable Global Interrupt

	WDT->CR = 0			// WDT OFF
		|(0x5A69<<16)
		|(0x25<<10)
		|(0x1A<<4)
		;

	SystemCoreClock = 500000;		// System Clock = 500kHz
	SystemPeriClock = 500000;		// System Clock = 500kHz

	/* Set Flach Access Time */
	FMC->MR = 0x81;
	FMC->MR = 0x28;
	FMC->CFG = (0x7858 << 16) | (3 << 8);		// Flash Access in 4 cycles (3-wait)
	FMC->MR = 0;
}


/* --------------------------------- End Of File ------------------------------ */
