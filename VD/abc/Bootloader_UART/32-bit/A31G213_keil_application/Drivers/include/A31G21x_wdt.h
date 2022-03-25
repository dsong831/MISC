/**********************************************************************
* @file		A31G21x_wdt.h
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
#ifndef _WDT_H_
#define _WDT_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/** WDTCR interrupt enable bit */
#define WDTCR_UNFIEN			    ((uint32_t)(1<<2))  // Watch-dog Timer Underflow Interrupt Enable bit	
#define WDTCR_WINMIEN		    ((uint32_t)(1<<3))  // Watch-dog Timer Window Match Interrupt Enable bit
	
#define WDT_INTERRUPT_BITMASK   0x000c

/** WDTSR interrupt enable bit */
#define WDTSR_UNFIFLAG			    ((uint32_t)(1<<0))  // Watch-dog Timer Underflow Interrupt Flag bit	
#define WDTSR_WINMIFLAG		    ((uint32_t)(1<<1))  // Watch-dog Timer Window Match Interrupt Flag bit

#define WDT_STATUS_BITMASK   0x0003

	
	/* Public Types --------------------------------------------------------------- */
/**********************************************************************
 * @brief WDT structure definitions
 **********************************************************************/
enum {
		WDT_DIV_4=0,
		WDT_DIV_16,
		WDT_DIV_64,
		WDT_DIV_256	
};

typedef enum {
	WDT_INTCFG_UNFIEN = 0,		/*!< UNFIEN Interrupt enable*/
	WDT_INTCFG_WINMIEN,			/*!< WINMIEN Interrupt enable*/
} WDT_INT_Type;

typedef struct Wdt_Config
{
	uint8_t wdtResetEn;			/**< if ENABLE -> the Reset bit is enabled				*/
	uint16_t wdtClkDiv;		/* wdtClkDiv */
	uint32_t wdtTmrConst;		/**< Set Watch-dog Timer Data Register	*/
	uint32_t wdtWTmrConst;		/**< Set Watch-dog Timer Window Data Register	*/	
} WDT_CFG_Type;

/* Public Functions ----------------------------------------------------------- */
void WDT_Init(WDT_CFG_Type wdtCfg);
void WDT_ReloadTimeCounter(void);
void WDT_Start(FunctionalState ctrl);
uint32_t WDT_GetCurrentCount(void);
uint32_t WDT_GetStatus(void);
void WDT_ClearStatus(uint32_t clrbit);
void WDT_IntConfig(WDT_INT_Type WDTIntCfg, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /* _WDT_H_ */

/* --------------------------------- End Of File ------------------------------ */
