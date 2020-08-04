/**********************************************************************//**
* @file				A31T21x_hal_dmac.c
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
#include "A31T21x_hal_dmac.h"


/*********************************************************************//**
 * @brief						Initialize DMA controller
 * @param[in]	DMACx	Pointer to selected DMAC peripheral, should be:
 *											- DMAC0~3	:DMAC0~3 peripheral
 * @param[in]	DMAC_Config	Pointer to a DMAC_CFG_Type structure
 *											that contains the configuration information for the specified
 *											channel peripheral.
 * @param[in]	dmac_en	
 * @return				HAL_Status_Type
 *********************************************************************/
HAL_Status_Type HAL_DMAC_Init(DMA_Type *DMACx, DMAC_CFG_Type *DMAC_Config, EN_DIS_Type dmac_en)
{
	/* DMA function and clock disable */
	SCU->PER1 &= ~(1<<4);				// DMA peri enable
	SCU->PCER1 &= ~(1<<4);	// DMA peri clock enable

	if(dmac_en == ENABLE)
	{
		/* DMA function and clock enable */
		SCU->PER1 |= (1<<4);				// DMA peri enable
		SCU->PCER1 |= (1<<4);		// DMA peri clock enable

		/* Set DMA configuration */
		DMACx->CR = ((DMAC_Config->TransferCnt & 0x0FFF) << 16)
			| ((DMAC_Config->PeriSel & 0x0F) << 8)
			| ((DMAC_Config->TransferSize & 0x03) << 2)
			| ((DMAC_Config->Dir & 0x01) << 1)
			;

		/* Set Peri/Memory Address */
		DMACx->PAR = DMAC_Config->PeriAddr;	
		DMACx->MAR = DMAC_Config->MemAddr;
	}

	return HAL_OK;
}

/**********************************************************************//**
 * @brief						Enable/Disable DMA channel
 * @param[in]	DMACx	Pointer to selected DMAC peripheral, should be:
 *											- DMAC0~3	:DMAC0~3 peripheral
 * @param[in]	NewState	New State of this command, should be:
*											- SET			: 1 (dma is running or enabled)
*											- RESET	: 0 (dma is in stop or hold state)
 * @return				HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_DMAC_Cmd(DMA_Type *DMACx, SET_RESET_Type NewState)
{
	if(NewState == SET)
	{
		DMACx->SR |= DMAC_SR_DMAEN_Msk;			// DMA is running or enabled
	}
	else
	{
		DMACx->SR &= ~DMAC_SR_DMAEN_Msk;		// DMA is in stop or hold state
	}

	return HAL_OK;
}

/**********************************************************************//**
 * @brief						Check if corresponding channel does have an active interrupt
 *											request or not
 * @param[in]	DMACx	Pointer to selected DMAC peripheral, should be:
 *											- DMAC0~3	: DMAC0~3 peripheral
 * @return				SET_RESET	status of DMA channel interrupt after masking
 *											- SET			: 1 (all data is transferred)
 *											- RESET	: 0 (data to be transferred is existing)
 **********************************************************************/
SET_RESET_Type HAL_DMAC_GetStatus(DMA_Type *DMACx)
{
	if(DMACx->SR & DMAC_SR_EOT_Msk)
	{
		return SET;			// All data is transferred
	}
	else
	{
		return RESET;	// Data to be transferred is existing
	}
}


/* --------------------------------- End Of File ------------------------------ */

