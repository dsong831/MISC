/**********************************************************************//**
* @file				A31T21x_hal_pcu.h
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


#ifndef A31T21X_HAL_PCU_H
#define A31T21X_HAL_PCU_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private macros -------------------------------------------------------- */
#define PORT_ACCESS_EN()		do{PCU->PORTEN = 0x15; PCU->PORTEN = 0x51;} while(0)
#define PORT_ACCESS_DIS()	do{PCU->PORTEN = 0x00;} while(0) 

#define PORT_MODE_MASK		(0x03UL)
#define PORT_FUNC_MASK			(0x0FUL)
#define PORT_PUPD_MASK			(0x03UL)

#define ALTERN_FUNC0						(0x00UL)	// AF0
#define ALTERN_FUNC1						(0x01UL)	// AF1
#define ALTERN_FUNC2						(0x02UL)	// AF2
#define ALTERN_FUNC3						(0x03UL)	// AF3
#define ALTERN_FUNC4						(0x04UL)	// AF4
#define ALTERN_FUNC5						(0x05UL)	// AF5

#define PUPD_DISABLE						(0x00UL)	// Disable pull-up/down
#define PULLUP_ENABLE					(0x01UL)	// Enable pull-up
#define PULLDOWN_ENABLE		(0x02UL)	// Enable pull-down
#define ANALOG_INPUT						(0x03UL)	// analog input


/* Private typedef -----------------------------------------------------------*/
typedef enum {
	INPUT = 0,
	PUSH_PULL_OUTPUT,
	ALTERN_FUNC,
	OPEN_DRAIN_OUTPUT
} PIN_MODE_Type;

enum {
	IER_DISABLE_EINT = 0,
	IER_LEVEL_EINT = 1,
	IER_EDGE_EINT = 3
};

enum {
	ICR_PROHIBIT_EINT = 0,
	ICR_LOW_LEVEL_EINT = 1,
	ICR_FALLING_EDGE_EINT = ICR_LOW_LEVEL_EINT,
	ICR_HIGH_LEVEL_EINT = 2,
	ICR_RISING_EDGE_EINT = ICR_HIGH_LEVEL_EINT,
	ICR_BOTH_EDGE_EINT = 3	
};


/* Public Functions ----------------------------------------------------------- */
void HAL_GPIO_ModeConfig(PORT_Type *Pn, uint8_t pin_no, PIN_MODE_Type pin_mode);
void HAL_GPIO_AlternativeConfig(PORT_Type *Pn, uint8_t pin_no, uint32_t func_no);
void HAL_GPIO_PUPDConfig(PORT_Type *Pn, uint8_t pin_no, uint8_t pupd_sel);
uint16_t HAL_GPIO_ReadPin(PORT_Type *Pn);
void HAL_GPIO_WritePin(PORT_Type *Pn, uint16_t Value);
void HAL_GPIO_SetOutput(PORT_Type *Pn, uint16_t bitValue);
void HAL_GPIO_ClearOutput(PORT_Type *Pn, uint16_t bitValue);
void HAL_GPIO_OutDataMaskConfig(PORT_Type *Pn, uint8_t pin_no, uint8_t mask_sel);
void HAL_GPIO_SetDebouncePin(PORT_Type *Pn, uint8_t pin_no);
void HAL_GPIO_EXTI_Config(PORT_Type *Pn, uint8_t pin_no, uint8_t eint_en, uint8_t eint_mode);
uint32_t HAL_GPIO_EXTI_GetStatus(PORT_Type *Pn);
void HAL_GPIO_EXTI_ClearStatus(PORT_Type *Pn, uint8_t pin_no);


#ifdef __cplusplus
}
#endif


#endif		/* A31T21X_HAL_PCU_H */

/* --------------------------------- End Of File ------------------------------ */
