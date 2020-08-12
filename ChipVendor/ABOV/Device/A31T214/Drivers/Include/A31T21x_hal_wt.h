/**********************************************************************//**
* @file				A31T21x_hal_wt.h
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


#ifndef A31T21X_HAL_WT_H
#define A31T21X_HAL_WT_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
#define WT_DISABLE					(0)
#define WT_ENABLE					(1)
#define WT_INT_DISABLE		(0)
#define WT_INT_ENABLE		(1)
	
/* Public Types --------------------------------------------------------------- */
enum {
	WT_CLKDIV_2_7 = 0,										/*!< WT Clock / (2^7) */
	WT_CLKDIV_2_13,											/*!< WT Clock / (2^13) */
	WT_CLKDIV_2_14,											/*!< WT Clock / (2^14) */
	WT_CLKDIV_2_14_MUL_WTDR		/*!< WT Clock / ((2^14) * (WTDR+1)) */
};

typedef struct
{
	uint32_t WTCLKDIV;		       /* !< wtClkDiv */
	uint32_t WTDR;		/*!< Set Watch Timer Data Register	*/
} WT_CFG_Type;


/* Public Functions ----------------------------------------------------------- */
HAL_Status_Type HAL_WT_Init(WT_CFG_Type wt_cfg, unsigned char wt_en, unsigned char int_en);
void HAL_WT_TimerStart(EN_DIS_Type wt_en);
void HAL_WT_ClearStatus(void);


#endif /* A31T21X_HAL_WT_H */


/* --------------------------------- End Of File ------------------------------ */
