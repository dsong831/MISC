/**********************************************************************//**
* @file				A31T21x_hal_led.c
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			29. JUNE. 2020
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


/* Includes ------------------------------------------------------------ */
#include "A31T21x_hal_led.h"


/**********************************************************************
 * @brief						HAL_LED Init
 * @param[in]	LED Type, LED Config Struct
 * @return				None
 **********************************************************************/
void HAL_LED_Init(LED_Type *LEDn, LED_CFG_Type *LED_Config)
{
	/* LED Block Enable */
	SCU->PER2 &= ~(1<<29);
	SCU->PCER2 &= ~(1<<29);
	SCU->PER2 |= (1<<29);
	SCU->PCER2 |= (1<<29);

	/* LED Period Configure */
	LEDn->PRESD = (LED_Config->tPRESD & 0xFF);
	LEDn->COMPWID = (LED_Config->tCOMPWID & 0xFF);
	LEDn->LEDPD = (LED_Config->tLEDPD & 0xFFFFF);

	/* Set LED Interrupt */
	LEDn->SR = 0;
	if(LED_Config->tSR_INTE)
	{
		LEDn->SR |= (LED_Config->tSR_INTE << 1);
		NVIC_SetPriority(LED_IRQn, 3);
		NVIC_EnableIRQ(LED_IRQn);
	}
	else
	{
		LEDn->SR &= ~(LED_Config->tSR_INTE << 1);
	}

	/* Set Overlap time */
	if(LED_Config->tOVERLAP < 8)
	{
		LEDn->LEDCON2 &= ~(1<<3);
		LEDn->LEDCON2 |= (LED_Config->tOVERLAP << 0);
	}
	else
	{
		LEDn->LEDCON2 |= (1<<3);	// Overlap time = LED_CLK / 2
	}

	/* Set LED mode */
	LEDn->LEDCON1 = (LED_Config->tMODE << 2) | (1<<1);	// Mode setting & LED enable
}

/**********************************************************************
 * @brief						HAL_LED Display On
 * @param[in]	None
 * @return				None
 **********************************************************************/
void HAL_LED_DISP_ON(void)
{
	LED->LEDCON1 |= (1<<0);	// Start LED
}

/**********************************************************************
 * @brief						HAL_LED Display Off
 * @param[in]	None
 * @return				None
 **********************************************************************/
void HAL_LED_DISP_OFF(void)
{
	LED->LEDCON1 &= ~(1<<0);	// Stop LED
}

/*********************************************************************//**
 * @brief						Clear LED Interrupt Flags
 * @param[in]	None
 * @return				None
 *********************************************************************/
void HAL_LED_ClearStatus(void)
{
	LED->SR &= ~(1<<2);		// LED INTF [bit2]
}


/* --------------------------------- End Of File ------------------------------ */
