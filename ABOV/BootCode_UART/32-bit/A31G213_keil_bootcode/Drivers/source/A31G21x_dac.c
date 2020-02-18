/**********************************************************************
* @file		A31G21x_dac.c
* @brief	Contains all functions support for DAC firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "A31G21x_dac.h"
#include "A31G21x_scu.h"

/* Public Functions ----------------------------------------------------------- */
/**********************************************************************
 * @brief 		Initial for DAC
 * @param[in]	DACx pointer to DAC_Type, should be: DACx
 * @param[in]	DAC_ConfigStruct
 * @return 		None
 **********************************************************************/
void DAC_Init(DAC_Type *DACx, DAC_CFG_Type *DAC_ConfigStruct)
{
	uint32_t	tempreg;
	
	SCU->PER2 &= ~(1<<22);
	SCU->PCER2 &= ~(1<<22);
	
	// Set-up Peripheral Clock for DAC module
	SCU->PER2 |= (1<<22);
	SCU->PCER2 |= (1<<22);
	
	// PGA Control Setting
	DAC_SetGain(DAC_ConfigStruct->PGASel);
	
	// DAC Control Setting
	tempreg = 0
	| ((DAC_ConfigStruct->RefSel&(0x1))<<8)		// DAC Reference Selection
	| ((DAC_ConfigStruct->IntSel&(0x1))<<7)		// DAC Interrupt Enable/Disable
	| ((DAC_ConfigStruct->SoftOnOff&(0x1))<<5)	// DAC SoftOnOff Enable/Disable
	| (1<<4)								// DAC Clear Buffer
	| ((DAC_ConfigStruct->FetchSel&(0x1))<<3)		// DAC Fetch Selection
	| ((DAC_ConfigStruct->ReloadSel&(0x3))<<1)	// DAC Reload Selection
	| (0<<0)
	;
	
	DAC->CR = tempreg;
	
	
	// DAC Offset Setting
	DAC_SetOffset(DAC_ConfigStruct->OffsetSel, DAC_ConfigStruct->OffsetDIRSel, DAC_ConfigStruct->OffsetVal);
	DAC->OFSCR = tempreg;
	
}

/**********************************************************************
 * @brief 		Close for DAC
 * @param[in]	DACx pointer to DAC_Type, should be: DACx
 * @return 		None
 **********************************************************************/
void DAC_DeInit(DAC_Type *DACx)
{
	DAC->CR = 0;
	
	SCU->PER2 &= ~(1<<22);
	SCU->PCER2 &= ~(1<<22);
}


/**********************************************************************
 * @brief 		Get Result Conversion from D/A Data Register
 * @param[in]	DACx pointer to DAC_Type, should be: DACx
 * @return 		Result of Conversion
 **********************************************************************/
uint16_t	DAC_GetData(DAC_Type *DACx)
{
	uint16_t	dac_value;
	
	dac_value = DACx->DR;
	
	return DAC_DR_RESULT(dac_value);
}


/**********************************************************************
 * @brief 		Set D/A Data Register
 * @param[in]	DACx pointer to DAC_Type, should be: DACx
 * @return 		Result of Conversion
 **********************************************************************/
void	DAC_SetData(DAC_Type *DACx, uint16_t data_value)
{
	DACx->DR = data_value;

}


/**********************************************************************
* @brief 		Set start mode for DAC
* @param[in]	DACx pointer to DAC_Type, should be: DACx
* @return 		None
*********************************************************************/
void DAC_StartCmd(DAC_Type *DACx, FunctionalState NewState)
{
	uint32_t tmp;
	tmp = DACx->CR;
	tmp	&= ~(1<<0);
	tmp |= (NewState<<0);
	DACx->CR = tmp;
}



/**********************************************************************
* @brief 		DAC interrupt configuration
* @param[in]	DACx pointer to DAC_Type, should be: DACx
* @param[in]	NewState:
* 					- SET : enable DAC interrupt
* 					- RESET: disable DAC interrupt
* @return 		None
**********************************************************************/
void DAC_IntConfig(DAC_Type *DACx, FunctionalState NewState)
{
	uint32_t	tempreg;
	
	tempreg = DACx->CR;
	tempreg &= ~(1<<7);
	tempreg |= (NewState<<7);
	DACx->CR = tempreg;
	
}

void DAC_EnableSoftOnOff(DAC_Type *DACx, FunctionalState NewState){
	uint32_t	tempreg;
	
	tempreg = DACx->CR;
	tempreg &= ~(1<<5);
	tempreg |= (NewState<<5);
	DACx->CR = tempreg;
}


void DAC_EnableADPCMFetch(DAC_Type *DACx, FunctionalState NewState){
	uint32_t	tempreg;
	
	tempreg = DACx->CR;
	tempreg &= ~(1<<3);
	tempreg |= (NewState<<3);
	DACx->CR = tempreg;
}

/*
#define DAC_RLDS_ALWAYS				(0x0uL)
#define DAC_RLDS_FADPCM				(0x1uL)
#define DAC_RLDS_TIMER10			(0x2uL)
#define DAC_RLDS_TIMER11			(0x3uL)
*/


void DAC_SelReloadSrc(DAC_Type *DACx, uint32_t sel){
	uint32_t	tempreg;
	
	tempreg = DACx->CR;
	tempreg &= ~(3<<1);
	tempreg |= (sel<<1);
	DACx->CR = tempreg;
	
}

/**********************************************************************
* @brief 		Get DAC status 
* @param[in]	DACx pointer to DAC_Type, should be: DACx
* @return 		DAC status register
**********************************************************************/
uint32_t	DAC_GetStatus(DAC_Type *DACx)
{
	return ((DACx->CR>>6)&1);
}


/**********************************************************************
* @brief 		Clear DAC Chanel status
* @param[in]	DACx pointer to DAC_Type, should be: DACx
* @return 		None
**********************************************************************/
void DAC_ClearStatus(DAC_Type *DACx)
{
	uint32_t	tempreg;
	
	tempreg = DACx->CR;
	tempreg &= ~(1<<6);
	
	DACx->CR = tempreg;
}


/**********************************************************************
* @brief 		Get DAC Buffer
* @param[in]	DACx pointer to DAC_Type, should be: DACx
* @return 		DAC status register
**********************************************************************/
uint16_t	DAC_GetBuffer(DAC_Type *DACx)
{
	uint16_t	dac_fvalue;
	
	dac_fvalue = DACx->BR;
	
	return (dac_fvalue);
}

/**********************************************************************
 * @brief 		Set DAC output offset for DAC
 * @param[in]	enable	Offset enable
 * @param[in]	dir		Offset direction
 *						0: Minus (-)
 *						1: Plus (+)
 * @param[in]	value	Offset value (DAC_DR +- value)
 * @return 		None
 **********************************************************************/
void DAC_SetOffset(uint32_t enable, uint32_t dir, uint32_t value){
	uint32_t tempreg;
	tempreg = 0
	| ((enable&(0x1))<<7)	// DAC Offset Enable/Disable
	| ((dir&(0x1))<<6)	// DAC Offset Direction Selection
	| ((value&(0xF))<<0)	// DAC Offset Value
	;
	
	DAC->OFSCR = tempreg;
}

void DAC_SetGain(uint32_t gain){
	DAC->PGSR = gain;
}

/* --------------------------------- End Of File ------------------------------ */

