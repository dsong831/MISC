/**********************************************************************//**
* @file				A31T21x_hal_pwr.h
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


#ifndef A31T21X_HAL_PWR_H
#define A31T21X_HAL_PWR_H

/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
#define LVI_DISABLE				(0x0UL<<7)
#define LVI_ENABLE					(0x1UL<<7)

#define LVI_INTDIS						(0x0UL<<5)
#define LVI_INTEN						(0x1UL<<5)

#define LVI_LVL_1_90V			(0x3UL<<0)
#define LVI_LVL_1_99V			(0x4UL<<0)
#define LVI_LVL_2_12V			(0x5UL<<0)
#define LVI_LVL_2_30V			(0x6UL<<0)
#define LVI_LVL_2_47V			(0x7UL<<0)
#define LVI_LVL_2_67V			(0x8UL<<0)
#define LVI_LVL_3_04V			(0x9UL<<0)
#define LVI_LVL_3_18V			(0xAUL<<0)
#define LVI_LVL_3_59V			(0xBUL<<0)
#define LVI_LVL_3_72V			(0xCUL<<0)
#define LVI_LVL_4_03V			(0xDUL<<0)
#define LVI_LVL_4_20V			(0xEUL<<0)
#define LVI_LVL_4_48V			(0xFUL<<0)


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint32_t LVIEN;
	uint32_t LVIINTEN;
	uint32_t LVIFLAG;
	uint32_t LVIVS;
} LVI_CFG_Type;


/* Public Functions ----------------------------------------------------------- */
void HAL_PWR_EnterSleepMode(void);
void HAL_PWR_EnterPowerDownMode(unsigned char stop_select, unsigned char aon_select);
HAL_Status_Type HAL_LVI_Init(LVI_CFG_Type *LVI_ConfigStruct);
uint32_t HAL_SCU_LVI_GetFlag(void);
void HAL_SCU_LVI_ClearFlag(void);


#endif		/* A31T21X_HAL_PWR_H */

/* --------------------------------- End Of File ------------------------------ */
