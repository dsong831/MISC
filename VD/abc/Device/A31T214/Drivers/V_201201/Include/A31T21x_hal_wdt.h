/**********************************************************************//**
* @file				A31T21x_hal_wdt.h
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


#ifndef A31T21X_HAL_WDT_H
#define A31T21X_HAL_WDT_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
#define WDT_SR_UNFIFLAG_Msk			(1<<0)
#define WDT_SR_WINMIFLAG_Msk		(1<<1)

	/* Public Types --------------------------------------------------------------- */
enum {
	WDT_CLKDIV_4 = 0,
	WDT_CLKDIV_16,
	WDT_CLKDIV_64,
	WDT_CLKDIV_256	
};

typedef struct {
	uint8_t RSTEN;										/*!<	0: Disable WDT Reset , 1: Enable WDT Reset	*/
	uint8_t WINMIEN;								/*!<	0: Disable Window Match INT , 1: Enable Window Match INT	*/
	uint8_t UNFIEN;										/*!<	0: Disable Underflow INT , 1: Enable Underflow INT	*/
	uint8_t CLKDIV;										/*!<	0: fWDT/4 , 1: fWDT/16 , 2: fWDT/64 , 3: fWDT/256	*/
	uint32_t DR;												/*!<	WDT Data Value	: 0x000000~0xFFFFFF	*/
	uint32_t WINDR;									/*!<	WDT Window Data Value : 0x000000~0xFFFFFF	*/	
} WDT_CFG_Type;


/* Public Functions ----------------------------------------------------------- */
HAL_Status_Type HAL_WDT_Init(WDT_CFG_Type wdt_config);
HAL_Status_Type HAL_WDT_DeInit(void);
HAL_Status_Type HAL_WDT_ConfigInterrupt(WDT_CFG_Type wdt_config, EN_DIS_Type int_en);
void HAL_WDT_ReloadCounter(void);
void HAL_WDT_CounterStart(EN_DIS_Type cnt_en);
void HAL_WDT_ClearStatus(uint32_t int_flag);


#endif		/* A31T21X_HAL_WDT_H */


/* --------------------------------- End Of File ------------------------------ */
