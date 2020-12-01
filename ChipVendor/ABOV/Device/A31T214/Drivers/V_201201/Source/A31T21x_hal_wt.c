/**********************************************************************//**
* @file				A31T21x_hal_wt.c
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
#include "A31T21x_hal_wt.h"


/**********************************************************************//**
* @brief						Intialization for Watch Timer Function
* @param[in]	WT_CFG_Type
* @param[in]	wt_en		Enable/Disable WT Function
*											- WT_DISABLE : wt disable
*											- WT_ENABLE : wt enable 
* @param[in]	int_en		Enable/Disable WT Interrupt
*											- WT_INT_DISABLE : wt interrupt disable
*											- WT_INT_ENABLE : wt interrupt enable 
* @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_WT_Init(WT_CFG_Type wt_cfg)
{
	uint32_t reg32 = 0;

	/* Reset WT block and clock */
	SCU->PER1 &= ~(0x01UL << 31);
	SCU->PER1 |= (0x01UL << 31);
	SCU->PCER1 &= ~(0x01UL << 31);
	SCU->PCER1 |= (0x01UL << 31);
	WT->CR = 0;

	/* Set WT Data (0x001~0xFFF) */
	reg32 = wt_cfg.WTDR & 0x0FFF;

	/* Set WT Interval */
	reg32 |= ((wt_cfg.WTCLKDIV & 0x03UL) << 4);

	WT->CR = reg32;

	return HAL_OK;
}

/**********************************************************************//**
* @brief						Deintialization for Watch Timer Function
* @param[in]	None
* @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_WT_DeInit(void)
{
	/* Disable WT block and clock */
	SCU->PER1 &= ~(0x01UL << 31);
	SCU->PCER1 &= ~(0x01UL << 31);
	WT->CR = 0;

	return HAL_OK;
}

/**********************************************************************//**
* @brief						Enable or disable WT interrupt
* @param[in]	EN_DIS_Type wt_en
*											- ENABLE: wdt interrupt enable
*											- DISABLE: wdt interrupt disable 
* @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_WT_ConfigInterrupt(EN_DIS_Type int_en)
{
	uint32_t reg32;
	
	reg32 = WT->CR;
	
	/* Enable WT Interrupt */
	if(int_en == ENABLE)
	{
		reg32 |= 0x08;				// Enable interrupt
	}
	/* Disable WT Interrupt */
	else
	{
		reg32 &= ~(0x08);	// Disable interrupt
	}
	
	WT->CR = reg32;
	
	return HAL_OK;
}

/**********************************************************************//**
* @brief						Enable WT activity
* @param[in]	EN_DIS_Type wt_en
*											- DISABLE: wt enable
*											- ENABLE: wt disable 
* @return					HAL_Status_Type
 **********************************************************************/
void HAL_WT_TimerStart(EN_DIS_Type wt_en)
{
	uint32_t reg32;

	reg32 = WT->CR & 0xFF;
	reg32 &= ~(0x01UL << 7);		// Disable watch timer counter

	if(wt_en == ENABLE)
	{
		reg32 |= (0x01UL << 7);		// Enable watch timer counter
		reg32 |= (0x01UL << 0);		// Clear counter
	}

	WT->CR = reg32;
}

/*********************************************************************//**
 * @brief						Clear WT Interrupt Flags
 * @param[in]	None
 * @return				None
 *********************************************************************/
void HAL_WT_ClearStatus(void)
{
	WT->CR |= ((uint32_t)(3<<0));	// WTIFLAG[bit1], WTCLR[bit0] Clear to '0' when write '1'.
}


/* --------------------------------- End Of File ------------------------------ */
