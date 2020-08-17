/**********************************************************************//**
* @file		A31G21x_hal_timer1n.h
* @brief	       Contains all macro definitions and function prototypes support
* @version	1.00
* @date: 
* @author	ABOV Application Team
*
* Copyright(C) 2019, ABOV Semiconductor. All rights reserved.
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


#ifndef _TIMER1n_H_
#define _TIMER1n_H_

//------------------------------- Includes ----------------------------
#include "A31G21x.h"
#include "A31G21x_hal_aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** Timer 1n Enable/Disable Definition  */
#define TIMER1n_DISABLE                    	(0x0uL<<15)
#define TIMER1n_ENABLE                    	(0x1uL<<15)

/** Timer 1n Clock Selection Definition  */
#define TIMER1n_CLKINT                    	(0x0uL<<14)
#define TIMER1n_CLKEXT                    	(0x1uL<<14)

/**  Timer 1n Mode Selection Definition */
#define TIMER1n_CNTM                    		(0x0uL<<12)
#define TIMER1n_CAPM                    		(0x1uL<<12)
#define TIMER1n_PPGONEM                 		(0x2uL<<12)
#define TIMER1n_PPGREM                  		(0x3uL<<12)

/** Timer 1n External Clock Edge Selection Definition  */
#define TIMER1n_FEDGE                    		(0x0uL<<11)
#define TIMER1n_REDGE                    		(0x1uL<<11)

/**  Timer 1n OUT Polarity Selection Definition  */
#define TIMER1n_STHIGH                    	(0x0uL<<8)
#define TIMER1n_STLOW                    		(0x1uL<<8)

/** Timer 1n Capture Polarity Selection Definition  */
#define TIMER1n_CAPFALL                    	(0x0uL<<6)
#define TIMER1n_CAPRISE                    	(0x1uL<<6)
#define TIMER1n_CAPBOTH                    	(0x2uL<<6)

/**  Timer 1n Match Interrupt Definition  */ 
#define TIMER1n_MATINTEN                    (0x1uL<<5)
#define TIMER1n_MATINTDIS                  	(0x0uL<<5)

/** Timer 1n Capture Interrupt Definition  */ 
#define TIMER1n_CAPINTEN                   	(0x1uL<<4)
#define TIMER1n_CAPINTDIS                   (0x0uL<<4)


#define TIMER_INT_MATCH                  (0x01<<3)

//------------------------- Internal Functions ------------------------
extern void TIMER1n_SetReg(TIMER1n_Type *, uint32_t, uint32_t);



#define TIMER1n_PRS_MASK	0x0FFF

/***********************************************************************//**
 * @brief Timer device enumeration
**********************************************************************/
/* Timer n Control register  */
#define TIMER1n_CR_CKSEL_MASK			(0x01<<14)
#define TIMER1n_CR_CKSEL_SET(n)			(n<<14)

#define TIMER1n_CR_MODE_MASK			(0x03<<12)
#define TIMER1n_CR_MODE_SET(n)			(n<<12)

#define TIMER1n_CR_ECE_MASK			(0x01<<11)
#define TIMER1n_CR_ECE_SET(n)			(n<<11)

#define TIMER1n_CR_STARTLVL_MASK			(0x01<<8)
#define TIMER1n_CR_STARTLVL_SET(n)			(n<<8)

#define TIMER1n_CR_CPOL_MASK			(0x03<<6)
#define TIMER1n_CR_CPOL_SET(n)			(n<<6)

/** @brief counter clock source select */
typedef enum
{
	TIMER1n_MCCR1PCLK = 0,		/*!< clock source from pclk or mccr1 */
	TIMER1n_ECn = 1			/*!< clock source from ECn pin input. before setting, have to set ECn pin mode */	
} TIMER1n_CKSEL_MODE_OPT;

/**
 * @brief  Timer1n mode selection enumerated definition
 */
typedef enum
{
	TIMER1n_PERIODIC_MODE = 0,	/*!< PERIODIC mode */
	TIMER1n_CAPTURE_MODE,			/*!< CAPTURE mode */
	TIMER1n_ONESHOT_MODE,		/*!< ONE SHOT mode */
	TIMER1n_PWM_MODE				/*!< PWM mode */
} TIMER1n_MODE_OPT;

/**
 * @brief  Timer1n clr mode definition
 */
typedef enum
{
	TIMER1n_FALLING_EGDE=0,		/*!< falling edge clear mode */
	TIMER1n_RISING_EGDE,	/*!< rising edge clear mode */
	TIMER1n_BOTH_EGDE,			/*!< both edge clear  mode */
	TIMER1n_NONE						/*!< none clear mode */
} TIMER1n_CLR_MODE_OPT;

/** @brief start default level select: initial output value. */
typedef enum
{
	TIMER1n_START_HIGH=0,
	TIMER1n_START_LOW
} TIMER1n_STARTLVL_OPT;

/*********************************************************************
* @brief TIMER Interrupt Type definitions
**********************************************************************/
typedef enum {
	TIMER1n_INTCFG_MIE = 0,		/*!< Match Interrupt enable*/
	TIMER1n_INTCFG_CIE,			/*!< Capture Interrupt enable*/
} TIMER1n_INT_Type;

/***********************************************************************
 * @brief Timer structure definitions
**********************************************************************/
/** @brief Configuration structure in TIMER mode */
typedef struct
{
	uint16_t ADR;	
	uint16_t BDR;	
	uint16_t Prescaler;		
	uint8_t StartLevel;		/**< set initial output value, should be:	START_LOW or START_HIGH */		
	uint8_t CkSel;			/**< Counter clock source select, should be: PCLK or  ECn */
	uint8_t ECE;
} TIMER1n_PERIODICCFG_Type;

/** @brief Configuration structure in COUNTER mode */
typedef struct {
	uint16_t ADR;		
	uint16_t BDR;			
	uint16_t Prescaler;		
	uint8_t StartLevel;		/**< set initial output value, should be:	START_LOW or START_HIGH */	
	uint8_t CkSel;			/**< Counter clock source select, should be: PCLK or  ECn */	
	uint8_t ECE;
} TIMER1n_PWMCFG_Type,TIMER1n_ONESHOTCFG_Type;

/** @brief Capture Input configuration structure */
typedef struct {	
	uint16_t ADR;	
	uint16_t Prescaler;		
	uint8_t ClrMode;		
	uint8_t CkSel;
	uint8_t ECE;
} TIMER1n_CAPTURECFG_Type;


/* Public Functions ----------------------------------------------------------- */
/* Init/DeInit TIMER1n functions -----------*/
HAL_Status_Type HAL_TIMER1n_Init(TIMER1n_Type *TIMER1n, TIMER1n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct);
HAL_Status_Type HAL_TIMER1n_DeInit (TIMER1n_Type *TIMER1n);

/* TIMER1n configuration functions --------*/
HAL_Status_Type HAL_TIMER1n_Cmd(TIMER1n_Type *TIMER1n, FunctionalState NewState);
HAL_Status_Type HAL_TIMER1n_ConfigInterrupt(TIMER1n_Type *TIMER1n, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState);
HAL_Status_Type HAL_TIMER1n_ClearCounter(TIMER1n_Type *TIMER1n);
uint32_t HAL_TIMER1n_GetMatchInterrupt(TIMER1n_Type *TIMER1n);
HAL_Status_Type HAL_TIMER1n_ClearMatchInterrupt(TIMER1n_Type *TIMER1n);
uint32_t HAL_TIMER1n_GetCaptureData(TIMER1n_Type *TIMER1n);
uint32_t HAL_TIMER1n_GetCaptureInterrupt(TIMER1n_Type *TIMER1n);
HAL_Status_Type HAL_TIMER1n_ClearCaptureInterrupt(TIMER1n_Type *TIMER1n);

#ifdef __cplusplus
}
#endif

#endif /*_TIMER1n_H_ */

