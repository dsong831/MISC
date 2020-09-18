/**********************************************************************//**
* @file				A31T21x_hal_led.h
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


#ifndef A31T21X_HAL_LED_H
#define A31T21X_HAL_LED_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
#define LED_AUTO_MODE								0
#define LED_HANDISHAKE_MODE		1
#define LED_STOPCOUNT_MODE		2
#define LED_SMARTSHARE_MODE	3
#define LED_ALONE_MODE							4

/* Public Types --------------------------------------------------------------- */
typedef struct {
	uint32_t	tPRESD;
	uint32_t	tCOMPWID;
	uint32_t	tLEDPD;
	uint32_t	tSR_INTE;
	uint32_t	tOVERLAP;
	uint32_t	tMODE;
}LED_CFG_Type;


/* Public Functions ----------------------------------------------------------- */
void HAL_LED_Init(LED_Type *LEDn, LED_CFG_Type *LED_Config);
void HAL_LED_DISP_ON(void);
void HAL_LED_DISP_OFF(void);
void HAL_LED_ClearStatus(void);

#endif /* A31T21X_HAL_LED_H */


/* --------------------------------- End Of File ------------------------------ */
