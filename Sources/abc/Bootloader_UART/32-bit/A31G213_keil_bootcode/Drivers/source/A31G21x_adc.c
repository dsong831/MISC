/**********************************************************************
* @file		A31G21x_adc.c
* @brief	Contains all functions support for ADC firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "A31G21x_adc.h"
#include "A31G21x_scu.h"

/* Public Functions ----------------------------------------------------------- */
/**********************************************************************
 * @brief 		Initial for ADC
 * @param[in]	ADCx pointer to ADC_Type, should be: ADCx
 * @param[in]	ADC_ConfigStruct
 * @return 		None
 **********************************************************************/
void ADC_Init(ADC_Type *ADCx, ADC_CFG_Type *ADC_ConfigStruct)
{
	uint32_t tempreg;

	SCU->PER2&=~(1<<20);
	SCU->PCER2&=~(1<<20);
	/* Set up peripheral clock for ADC module */
	SCU->PER2|=(1<<20);
	SCU->PCER2|=(1<<20);
	
	ADCx->CR = (1<<15);  // ADCEN;
	
	tempreg = 0
		| (1<<15)  // ADCEN
		| (((ADC_ConfigStruct->TrgSel) & 7)<<11)  // TRGSRC
		| (((ADC_ConfigStruct->RefSel) & 1)<<10) 
		| (1<<4)  // clear flag
		;				
	ADCx->CR = tempreg;

	ADCx->PREDR = (ADC_ConfigStruct->InClkDiv&0x1f);
}

/**********************************************************************
* @brief 		Close ADC
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @return 		None
**********************************************************************/
void ADC_DeInit(ADC_Type *ADCx)
{
	ADCx->CR = 0;
	
	SCU->PER2&=~(1<<20);
	SCU->PCER2&=~(1<<20);
}

/**********************************************************************
* @brief 		Get Result conversion from A/D data register
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @return 		Result of conversion
**********************************************************************/
uint16_t ADC_GetData(ADC_Type *ADCx)
{
	uint16_t adc_value;

	adc_value = ADCx->DR;
	
	return ADC_DR_RESULT(adc_value);
}

/**********************************************************************
* @brief 		Set start mode for ADC
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @return 		None
*********************************************************************/
void ADC_StartCmd(ADC_Type *ADCx)
{
	ADCx->CR |=(1<<8);
}

/**********************************************************************
* @brief 		ADC interrupt configuration
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @param[in]	NewState:
* 					- SET : enable ADC interrupt
* 					- RESET: disable ADC interrupt
* @return 		None
**********************************************************************/
void ADC_IntConfig(ADC_Type *ADCx, FunctionalState NewState)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg &= ~(1<<5) ;

	if (NewState){
		tempreg |= (1<<5) ;
	}
	ADCx->CR = tempreg; 		
}

/**********************************************************************
* @brief 		Select ADC channel number
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @param[in]	Channel channel number
* @return 		None
**********************************************************************/
void ADC_ChannelSel(ADC_Type *ADCx, uint32_t Channel)
{
	uint32_t temp_reg;
	
	temp_reg = ADCx->CR & (uint32_t)(~0x0f);
	temp_reg |= (uint32_t)(Channel&0x0f);	
	ADCx->CR = temp_reg;
}

/**********************************************************************
* @brief 		Get ADC channel status 
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @return 		ADC status register
**********************************************************************/
uint32_t ADC_GetStatus(ADC_Type *ADCx)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg &= (1<<4) ;
	
	return tempreg;
}

/**********************************************************************
* @brief 		Clear ADC Chanel status
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @param[in]	status  channel number
* @return 		None
**********************************************************************/
void ADC_ClearStatus(ADC_Type *ADCx)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg |= (1<<4) ;
			
	ADCx->CR = tempreg;  // clear flag;	
}


/* --------------------------------- End Of File ------------------------------ */

