/**********************************************************************
* @file		A31G21x_wt.h
* @brief	Contains all macro definitions and function prototypes
* 			support for WDT firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
#ifndef _WT_H_
#define _WT_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/** WTCR interrupt enable bit */
#define WTCR_WTIEN			    ((uint32_t)(1<<3))  // Watch Timer Interrupt Enable bit	
#define WT_INTERRUPT_BITMASK   0x0008

/** WTCR interrupt status flag bit */
#define WTCR_WTIFLAG			    ((uint32_t)(1<<1))  // Watch Timer Interrupt Flag bit	
#define WT_STATUS_BITMASK   0x0002

#define WTCR_WTCLR 			    ((uint32_t)(1<<0))  // Watch Timer counter and divider clear bit	

	
	/* Public Types --------------------------------------------------------------- */
/**********************************************************************
 * @brief WDT structure definitions
 **********************************************************************/
enum {
		WT_DIV_2_7=0,
		WT_DIV_2_13,
		WT_DIV_2_14,
		WT_DIV_2_14_MUL_WTDR	
};


typedef struct Wt_Config
{
	uint32_t wtClkDiv;		/* wtClkDiv */
	uint32_t wtTmrConst;		/**< Set Watch Timer Data Register	*/
} WT_CFG_Type;

/* Public Functions ----------------------------------------------------------- */
void WT_Init(WT_CFG_Type wtCfg);
void WT_DeInit(void);
void WT_Start(FunctionalState ctrl);
uint32_t WT_GetCurrentCount(void);
uint32_t WT_GetStatus(void);
void WT_ClearStatus(void);
void WT_IntConfig(FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /* _WT_H_ */

/* --------------------------------- End Of File ------------------------------ */
