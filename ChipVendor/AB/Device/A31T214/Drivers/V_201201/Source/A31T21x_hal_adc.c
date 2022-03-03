/**********************************************************************//**
* @file				A31T21x_hal_adc.c
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
#include "A31T21x_hal_adc.h"


/* Private variable -------------------------------------------------------- */
/* Private Types --------------------------------------------------------------- */
/* Public Functions ------------------------------------------------------- */
/**********************************************************************//**
 * @brief 		Initial for ADC
 * @param[in]	ADCx pointer to ADC_Type, should be: ADCx
 * @param[in]	ADC_ConfigStruct
 * @return 		HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_ADC_Init(ADC_Type *ADCx, ADC_CFG_Type *ADC_ConfigStruct)
{
	uint32_t tempreg;
  
	
	/* Check ADC handle */
	if(ADCx == NULL)
	{
		return HAL_ERROR;
	}
	 
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
	return HAL_OK;
}

/**********************************************************************//**
* @brief 		Close ADC
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @return 		HAL_Status_Type
**********************************************************************/
HAL_Status_Type HAL_ADC_DeInit(ADC_Type *ADCx)
{
	ADCx->CR = 0;
	
	SCU->PER2&=~(1<<20);
	SCU->PCER2&=~(1<<20);
	return HAL_OK;
}


/**********************************************************************//**
* @brief      Start A/D conversion
* @param      ADCx : ADC peripheral selected, should be
*              - ADC : ADC peripheral
* @return     HAL_Status_Type
**********************************************************************/
HAL_Status_Type HAL_ADC_Start(ADC_Type *ADCx)
{
	/* Check ADC handle */
	if(ADCx == NULL)
	{
	  return HAL_ERROR;
	}

	ADCx->CR |=(1<<8);

	return HAL_OK;
}

/**********************************************************************//**
* @brief      Stop A/D conversion  
*             If this function called after a conversion cycle starts,  
*             the current conversion is completed
* @param      ADCx : ADC peripheral selected, should be
*              - ADC : ADC peripheral
* @return     HAL_Status_Type
**********************************************************************/
HAL_Status_Type HAL_ADC_Stop(ADC_Type *ADCx)
{
	/* Check ADC handle */
	if(ADCx == NULL)
	{
	  return HAL_ERROR;
	}

	ADCx->CR&=~(1<<8);
	return HAL_OK;
}

/**********************************************************************//**
* @brief 		Get Result conversion from A/D data register
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @return 		Result of conversion
**********************************************************************/
uint16_t HAL_ADC_GetData(ADC_Type *ADCx)
{
	uint16_t adc_value;

	adc_value = ADCx->DR;
	
	return ADC_DR_RESULT(adc_value);
}



/**********************************************************************//**
* @brief 		ADC interrupt configuration
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @param[in]	NewState:
* 					- SET : enable ADC interrupt
* 					- RESET: disable ADC interrupt
* @return 		HAL_Status_Type
**********************************************************************/
HAL_Status_Type HAL_ADC_ConfigInterrupt(ADC_Type *ADCx, EN_DIS_Type NewState)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg &= ~(1<<5) ;

	if (NewState){
		tempreg |= (1<<5) ;
	}
	ADCx->CR = tempreg; 	
	return HAL_OK;
}

/**********************************************************************//**
* @brief 		Select ADC channel number
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @param[in]	Channel channel number
* @return 		HAL_Status_Type
**********************************************************************/
HAL_Status_Type HAL_ADC_ChannelSel(ADC_Type *ADCx, uint32_t Channel)
{
	uint32_t temp_reg;
	
	temp_reg = ADCx->CR & (uint32_t)(~0x0f);
	temp_reg |= (uint32_t)(Channel&0x0f);	
	ADCx->CR = temp_reg;
	return HAL_OK;
}

/**********************************************************************//**
* @brief 		Get ADC channel status 
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @return 		ADC status register
**********************************************************************/
uint32_t HAL_ADC_GetStatus(ADC_Type *ADCx)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg &= (1<<4) ;
	
	return tempreg;
}

/**********************************************************************//**
* @brief 		Clear ADC Chanel status
* @param[in]	ADCx pointer to ADC_Type, should be: ADCx
* @param[in]	status  channel number
* @return 		HAL_Status_Type
**********************************************************************/
HAL_Status_Type HAL_ADC_ClearStatus(ADC_Type *ADCx)
{
	uint32_t tempreg;

	tempreg = ADCx->CR;
	tempreg |= (1<<4) ;
			
	ADCx->CR = tempreg;  // clear flag;	
	return HAL_OK;
}


/* --------------------------------- End Of File ------------------------------ */

