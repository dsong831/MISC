/**********************************************************************
* @file		A31G21x_dmac.c
* @brief	Contains all functions support for DMAC firmware library
* 			on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */

/* Includes ------------------------------------------------------------------- */
#include "A31G21x_dmac.h"
#include "A31G21x_scu.h"


/* Public Functions ----------------------------------------------------------- */
/*********************************************************************
 * @brief 		Initialize DMA controller
 * @param[in] 	None
 * @return 		None
 *********************************************************************/
void DMAC_Init(void)
{
	/* to be defined Enable DMA and clock */
	// enabled default on reset
	SCU->PER1&=~SCU_PER1_DMA_Msk; // (1<<4) DMA peri enable 
	SCU->PCER1&=~SCU_PCER1_DMA_Msk; // (1<<4) DMA peri clock enable 
	
	SCU->PER1|=SCU_PER1_DMA_Msk; // (1<<4) DMA peri enable 
	SCU->PCER1|=SCU_PCER1_DMA_Msk; // (1<<4) DMA peri clock enable 
}

/*********************************************************************
 * @brief 		Setup DMAC channel peripheral according to the specified
 *              parameters in the GPDMAChannelConfig.
 * @param[in]	DMACx	Pointer to selected DMAC peripheral, should be:
 * 					- DMAC0~3	:DMAC0~3 peripheral
 * @param[in]	DMACChannelConfig Pointer to a DMAC_CH_CFG_Type structure
 * 				that contains the configuration information for the specified
 * 				channel peripheral.
 * @return		None
 *********************************************************************/
void DMAC_Setup(DMA_Type *DMACx, DMA_Ch_CFG_Type *DMACChConfig)
{
	DMACx->CR = ((DMACChConfig->TransferCnt & 0xfff)<<16) 
		| ((DMACChConfig->PeriSel & 0x0f)<<8)	
		| ((DMACChConfig->TransferWidth & 0x03)<<2)
		| ((DMACChConfig->Dir & 0x01)<<1)	
		;
	DMACx->PAR = DMACChConfig->PeriAddr;	
	DMACx->MAR = DMACChConfig->MemAddr;
}

/**********************************************************************
 * @brief		Enable/Disable DMA channel
 * @param[in]	DMACx	Pointer to selected DMAC peripheral, should be:
 * 					- DMAC0~3	:DMAC0~3 peripheral
 * @param[in]	NewState	New State of this command, should be:
 * 					- ENABLE.
 * 					- DISABLE.
 * @return		None
 **********************************************************************/
void DMAC_ChCmd(DMA_Type *DMACx, FunctionalState NewState)
{
	if (NewState == ENABLE) {
		DMACx->SR |= DMAC_SR_DMAEN_Msk;
	} else {
		DMACx->SR &= ~DMAC_SR_DMAEN_Msk;
	}
}

/**********************************************************************
 * @brief		Check if corresponding channel does have an active interrupt
 * 				request or not
 * @param[in]	DMACx	Pointer to selected DMAC peripheral, should be:
 * 					- DMAC0~3	:DMAC0~3 peripheral
 * @return		FlagStatus	status of DMA channel interrupt after masking
 * 				Should be:
 * 					- SET	: all data is transferred 
 * 					- RESET	:data to be transferred is existing 
 **********************************************************************/
FlagStatus DMAC_GetStatus(DMA_Type *DMACx)
{
	if (DMACx->SR & DMAC_SR_EOT_Msk){	
		return SET;
	}
	else {
		return RESET;
	}
}

/* --------------------------------- End Of File ------------------------------ */

