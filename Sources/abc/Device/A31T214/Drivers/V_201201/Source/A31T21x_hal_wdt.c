/**********************************************************************//**
* @file				A31T21x_hal_wdt.c
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
#include "A31T21x_hal_wdt.h"


/* Private variables ---------------------------------------------------------*/
static unsigned char gu8ReloadCount = 0;	// For window data function


/**********************************************************************//**
* @brief						Initialization for Watchdog function
* @param[in]	None
* @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_WDT_Init(WDT_CFG_Type wdt_config)
{
	uint32_t reg32;

	/* Set WDT DR/WINDR Register */
	WDT->DR = (wdt_config.DR & 0x00FFFFFF);
	WDT->WINDR = (wdt_config.WINDR & 0x00FFFFFF);

	/* Set WDT CLKDIV */
	reg32 = (wdt_config.CLKDIV & 0x03);

	/* Config WDT Reset */
	if(wdt_config.RSTEN)
	{
		reg32 &= ~(0x3F << 10);
		reg32 |= (0x3F << 10);
	}
	else
	{
		reg32 &= ~(0x3F << 10);
		reg32 |= (0x25 << 10);
	}

	/* Config WDT Underflow Interrupt */
	if(wdt_config.UNFIEN)
	{
		reg32 &= ~(0x01 << 2);
		reg32 |= (0x01 << 2);
	}
	else
	{
		reg32 &= ~(0x01 << 2);
	}

	/* Config WDT Window Match Interrupt */
	if(wdt_config.WINMIEN)
	{
		reg32 &= ~(0x01 << 3);
		reg32 |= (0x01 << 3);
	}
	else
	{
		reg32 &= ~(0x01 << 3);
	}

	WDT->CR = (0x5A69 << 16) | (0x1A << 4) | reg32;

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Deinitialization for Watchdog function
 * @param[in]	None
 * @return				HAL_Status_Type
 *********************************************************************/
HAL_Status_Type HAL_WDT_DeInit(void)
{
	/* Disable WDT */
	WDT->CR = 0
		|(0x5A69 << 16)	// Write Identification Key
		|(0x25 << 10)			// Disable Watch-dog timer reset
		|(0x1A << 4)				// Disable Watch-dog timer counter
		;

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Enable or disable WDT interrupt
 * @param[in]	EN_DIS_Type int_en
 *											- ENABLE: wdt interrupt enable
 *											- DISABLE: wdt interrupt disable 
 * @return				HAL_Status_Type
 *********************************************************************/
HAL_Status_Type HAL_WDT_ConfigInterrupt(WDT_CFG_Type wdt_config, EN_DIS_Type int_en)
{
	uint32_t reg32;
	
	reg32 = WDT->CR;

	/* Enable Interrupt */
	if(int_en == ENABLE)
	{
		// Config WDT Underflow Interrupt
		if(wdt_config.UNFIEN)
		{
			reg32 &= ~(0x01 << 2);
			reg32 |= (0x01 << 2);
		}
		else
		{
			reg32 &= ~(0x01 << 2);
		}
		//Config WDT Window Match Interrupt
		if(wdt_config.WINMIEN)
		{
			reg32 &= ~(0x01 << 3);
			reg32 |= (0x01 << 3);
		}
		else
		{
			reg32 &= ~(0x01 << 3);
		}
	}

	/* Diable Interrupt */
	else
	{
		reg32 &= ~(0xC0);
	}
	
	WDT->CR = (0x5A69 << 16) | (0x1A << 4) | reg32;
	
	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Reload WDT counter
 * @param[in]	None
 * @return				None
 *********************************************************************/
void HAL_WDT_ReloadCounter(void)
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
}

/**********************************************************************//**
* @brief						Enable WDT timer counter
* @param[in]	cnt_en Counter Enable
*											- DISABLE: wdt disable
*											- ENABLE: wdt enable 
* @return					None
 **********************************************************************/
void HAL_WDT_CounterStart(EN_DIS_Type cnt_en)
{
	uint32_t reg32;

	reg32 = WDT->CR & 0xFFFF;
	reg32 |= (0x1A << 4);			// Disable WDT timer counter

	if(cnt_en == ENABLE)
	{
		reg32 &= ~(0x3F << 4);	// Enable WDT timer counter, 
	}

	 WDT->CR = (0x5A69 << 16) | reg32;
}

/*********************************************************************
 * @brief						Clear WDT Interrupt Flags
 * @param[in]	int_flag
 *											- WDT_SR_UNFIFLAG_Msk : Clear underflow interrupt flag
 *											- WDT_SR_WINMIFLAG_Msk : Clear window match interrupt flag
 * @return				None
 *********************************************************************/
void HAL_WDT_ClearStatus(uint32_t int_flag)
{
	uint32_t reg32;

	reg32 = (WDT->SR & 0xFF);

	if((reg32 & (1<<0)) || (reg32 & (1<<1)))
	{
		WDT->SR = int_flag | 0x80;
	}
}


/* --------------------------------- End Of File ------------------------------ */
