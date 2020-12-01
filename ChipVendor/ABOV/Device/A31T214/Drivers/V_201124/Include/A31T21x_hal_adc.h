/**********************************************************************//**
* @file				A31T21x_hal_adc.h
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
*ABOV assumes no liability for application assistance or the design of purchasers?? products. No license, 
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


#ifndef A31T21X_HAL_ADC_H
#define A31T21X_HAL_ADC_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros -------------------------------------------------------- */
//-------- ADC Enable/Disable Definition ------------
#define ADC_DISABLE                    (0x0uL<<15)
#define ADC_ENABLE                    (0x1uL<<15)

//-------- ADC Trigger Signal Definition ------------
#define ADC_ADSTEN                  (0x0uL<<11)
#define ADC_T10M                    (0x1uL<<11)
#define ADC_T11M                    (0x2uL<<11)
#define ADC_T12M                    (0x3uL<<11)
#define ADC_T30                    (0x4uL<<11)

//-------- ADC Reference Selection Definition ------------
#define ADC_VDD                    (0x0uL<<10)
#define ADC_AVREF                    (0x1uL<<10)

//-------- ADC Convesion Start Definition ------------
#define ADC_ADST                    (0x1uL<<8)

//-------- ADC Interrupt Enable/Disable Definition ------------
#define ADC_INTDIS                    (0x0uL<<5)
#define ADC_INTEN                    (0x1uL<<5)

//-------- ADC Channel Selection Definition ------------
#define ADC_AN0                    (0x00uL<<0)
#define ADC_AN1                    (0x01uL<<0)
#define ADC_AN2                    (0x02uL<<0)
#define ADC_AN3                    (0x03uL<<0)
#define ADC_AN4                    (0x04uL<<0)
#define ADC_AN5                    (0x05uL<<0)
#define ADC_AN6                    (0x06uL<<0)
#define ADC_AN7                    (0x07uL<<0)
#define ADC_AN8                    (0x08uL<<0)
#define ADC_AN9                    (0x09uL<<0)
#define ADC_AN10                    (0x0auL<<0)
#define ADC_AN11                    (0x0buL<<0)
#define ADC_AN12                    (0x0cuL<<0)
#define ADC_AN13                    (0x0duL<<0)
/* Private macros ------------------------------------------------------------- */

/* -------------------------- BIT DEFINITIONS ----------------------------------- */
/**********************************************************************
 * Macro defines for ADC Status register
**********************************************************************/
#define ADC_STAT_END					((1UL<<4))

/**********************************************************************
 * Macro defines for ADC Data register
 **********************************************************************/
/** When DONE is 1, this field contains result value of ADC conversion */
#define ADC_DR_RESULT(n)			((n)&0xFFF)


/* Public Types --------------------------------------------------------------- */
#define ADC_REF_VDD                  0  /*!< ADC ref source VDD */
#define ADC_REF_AVREF               1  /*!< ADC ref source AVREF */

#define ADC_TRIGGER_DISABLE           0  /*!< Event Trigger Disabled/Soft-Trigger Only */
#define ADC_TRIGGER_TIMER10           1  /*!< Timer10 Event Trigger */
#define ADC_TRIGGER_TIMER11           2  /*!< Timer11 Event Trigger */
#define ADC_TRIGGER_TIMER12           3  /*!< Timer12 Event Trigger */
#define ADC_TRIGGER_TIMER30           4  /*!< Timer30 Event Trigger */

/**
 * @brief  ADC configuration structure definition
 */
typedef struct {
/* ADCCR */	
	uint32_t RefSel;				/**< RestartEn = ENABLE or DISABLE */			
	uint32_t TrgSel;	
	
/* ADPREDR */
	uint32_t InClkDiv;
} ADC_CFG_Type;



/* Public Functions ----------------------------------------------------------- */
/* Init/DeInit ADC peripheral ----------------*/
HAL_Status_Type HAL_ADC_Init(ADC_Type *ADCx, ADC_CFG_Type *ADC_ConfigStruct);
HAL_Status_Type HAL_ADC_DeInit(ADC_Type *ADCx);

/* Enable/Disable ADC functions --------------*/
HAL_Status_Type HAL_ADC_ChannelSel(ADC_Type *ADCx, uint32_t Channel);

HAL_Status_Type HAL_ADC_Start(ADC_Type *ADCx);
HAL_Status_Type HAL_ADC_Stop(ADC_Type *ADCx);

/* Configure ADC functions -------------------*/
HAL_Status_Type HAL_ADC_ConfigInterrupt(ADC_Type *ADCx, EN_DIS_Type NewState);

/* Get ADC information functions -------------------*/
uint16_t HAL_ADC_GetData(ADC_Type *ADCx);

/* Get/Clear ADC status  -------------------*/
uint32_t HAL_ADC_GetStatus(ADC_Type *ADCx);
HAL_Status_Type HAL_ADC_ClearStatus(ADC_Type *ADCx);

#ifdef __cplusplus
}
#endif


#endif		/* A31T21X_HAL_ADC_H */


/* --------------------------------- End Of File ------------------------------ */
