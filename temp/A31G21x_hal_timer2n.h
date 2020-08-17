/**********************************************************************//**
* @file		A31G21x_hal_timer2n.h
* @brief	      Contains all macro definitions and function prototypes support
*                    Timer 2n function header file.
* @version	1.00
* @date: 
* @author	ABOV Application Team
*
* Copyright(C) 2016, ABOV Semiconductor. All rights reserved.
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


#ifndef _TIMER2n_H_
#define _TIMER2n_H_

//------------------------------- Includes ----------------------------
#include "A31G21x.h"
#include "A31G21x_hal_aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**********************************************************************//**
* Timer Control1 Register definitions
**********************************************************************/
/** counter clock select bits */
#define TIMER2n_CR_CLK_MASK			(0x01<<14)
#define TIMER2n_CR_CLK_SET(n)		(n<<14)

/** Counter/timer mode bits */
#define TIMER2n_CR_MODE_MASK			(0x03<<12)
#define TIMER2n_CR_MODE_SET(n)			(n<<12)
	
/** External Clock Edge Selection bit */
#define TIMER2n_CR_ECE_MASK			(0x01<<11)
#define TIMER2n_CR_ECE_SET(n)		(n<<11)

/** T2nCAP signal selection bit */
#define TIMER2n_CR_CAPSEL_MASK			(0x03<<9)
#define TIMER2n_CR_CAPSEL_SET(n)		(n<<9)

	
/** T2nOUT Polarity Selection bit */
#define TIMER2n_CR_OPOL_MASK			(0x01<<8)
#define TIMER2n_CR_OPOL_SET(n)		(n<<8)

/** T2n Capture Polarity Selection bit */
#define TIMER2n_CR_CPOL_MASK			(0x03<<6)
#define TIMER2n_CR_CPOL_SET(n)		(n<<6)

/** T2n Match Interrupt Enable bit */
#define TIMER2n_CR_MIEN_MASK			(0x01<<5)
#define TIMER2n_CR_MIEN_SET(n)		(n<<5)

/** T2n Capture Interrupt Enable bit */
#define TIMER2n_CR_CIEN_MASK			(0x01<<4)
#define TIMER2n_CR_CIEN_SET(n)		(n<<4)

/** T2n Counter Temporary Pause Control bit */
#define TIMER2n_CR_PAU_MASK				(0x01<<1)
#define TIMER2n_CR_PAU_SET(n)			(n<<1)

/** T2n Match Interrupt Flag */
#define TIMER2n_CR_MATCH_FLAG			(1<<3)
#define TIMER2n_CR_CAPTURE_FLAG		(1<<2)


/** Clear Flag */
#define TIMER2n_CR_CLEAR_MATCHINT		(0x01<<3)
#define TIMER2n_CR_CLEAR_CAPTUREINT	(0x01<<2)
#define TIMER2n_CR_CLEAR_CNT_PRED		(0x01<<0)




/**********************************************************************//**
* Timer/Counter 2n A Data Register (ADR)
**********************************************************************/
#define TIMER2n_ADR_MASK	0xFFFFFFFF


/**********************************************************************//**
* Timer/Counter 2n B Data Register (BDR)
**********************************************************************/
#define TIMER2n_BDR_MASK	0xFFFFFFFF


/**********************************************************************//**
* Timer/Counter 2n Prescaler Data Register (PREDR)
**********************************************************************/
#define TIMER2n_PREDR_MASK					(0xFFF<<0)
#define TIMER2n_PREDR_SET(n)				(n<<0)



/**********************************************************************//**
* Timer/Counter 2n definitions
**********************************************************************/
/** Timer/counter enable bit */
#define TIMER2n_ENABLE			(1)
#define TIMER2n_DISABLE			(0)
/** Timer/counter reset bit */
#define TIMER2n_CLEAR				(1)
/** Timer control bit mask */


///* ================================================================================ */
///* ================         struct 'TGECR' Position & Mask         ================ */
///* ================================================================================ */

/* Public Types --------------------------------------------------------------- */
/***********************************************************************
 * @brief Timer device enumeration
**********************************************************************/
/* Timer n Control register 1 */
/** @brief Timer operating mode */
typedef enum
{
	TIMER2n_PERIODIC_MODE = 0,	/*!< PERIODIC mode */
	TIMER2n_CAPTURE_MODE,				/*!< PWM mode */
	TIMER2n_ONESHOT_MODE,				/*!< ONE SHOT mode */
	TIMER2n_PWM_MODE						/*!< CAPTURE mode */
} TIMER2n_MODE_OPT;

/** @brief clear select when capture mode */
typedef enum
{
	TIMER2n_FALLING_EGDE = 0,		/*!< rising edge clear mode */
	TIMER2n_RISING_EGDE,				/*!< falling edge clear mode */
	TIMER2n_BOTH_EGDE,					/*!< both edge clear  mode */
	TIMER2n_NONE								/*!< none clear mode */
} TIMER2n_CLR_MODE_OPT;

/** @brief Timer clock source select */
typedef enum
{
	TIMER2n_SOSC_CLK = 0,		/*!< clock source from Internal */
	TIMER2n_PCLK_CLK,					/*!< clock source from External */
} TIMER2n_MASTER_CKSEL_OPT;


/** @brief counter clock source select */
typedef enum
{
	TIMER2n_MCCR2PCLK = 0,		/*!< clock source from Internal */
	TIMER2n_PCLK = 0,		/*!< clock source from Internal */
	TIMER2n_EC2n=1,				/*!< clock source from External */
} TIMER2n_CKSEL_MODE_OPT;

/** @brief capture clock source select */
typedef enum
{
	TIMER2n_CAP_EXTERNAL_CLK = 0,		/*!< clock source from Externl */
	TIMER2n_CAP_SOSC_CLK,				/*!< clock source from XOSC */
	TIMER2n_CAP_WDTRC_CLK,				/*!< Clock Source from WDTRC */
} TIMER2n_CAP_CKSEL_OPT;


/** @brief start default level select: initial output value. */
typedef enum
{
	TIMER2n_START_HIGH =0,
	TIMER2n_START_LOW		
} TIMER2n_STARTLVL_OPT;

/*********************************************************************
* @brief TIMER Interrupt Type definitions
**********************************************************************/
typedef enum {
	TIMER2n_CR_MATCH_INTR = 0,		/*!< OVIE Interrupt enable*/
	TIMER2n_CR_CAPTURE_INTR,			/*!< MBIE Interrupt enable*/
} TIMER2n_INT_Type;

/***********************************************************************
 * @brief Timer structure definitions
**********************************************************************/
/** @brief Configuration structure in TIMER mode */
typedef struct
{
	uint32_t ADR;	
	uint32_t BDR;	
	uint16_t Prescaler;
	uint8_t StartLevel;		
	uint8_t CkSel;
	uint8_t ECE;
} TIMER2n_PERIODICCFG_Type;

/** @brief Configuration structure in COUNTER mode */
typedef struct {
	uint32_t ADR;
	uint32_t BDR;	
	uint16_t Prescaler;
	uint8_t StartLevel;
	uint8_t CkSel;
	uint8_t ECE;
} TIMER2n_PWMCFG_Type,TIMER2n_ONESHOTCFG_Type;

/** @brief Capture Input configuration structure */
typedef struct {
	uint32_t ADR;
	uint16_t Prescaler;
	uint8_t ClrMode;
	uint8_t CkSel;
	uint8_t ECE;	
	uint8_t CAPCkSel;	
} TIMER2n_CAPTURECFG_Type;


/* Public Functions ----------------------------------------------------------- */
/* Init/DeInit TIM functions -----------*/
HAL_Status_Type HAL_TIMER2n_Init(TIMER2n_Type *TIMER2n, TIMER2n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct);
HAL_Status_Type HAL_TIMER2n_DeInit (TIMER2n_Type *TIMER2n);

/* TIM configuration functions --------*/
HAL_Status_Type HAL_TIMER2n_Cmd(TIMER2n_Type *TIMER2n, FunctionalState NewState);
HAL_Status_Type HAL_TIMER2n_ConfigInterrupt(TIMER2n_Type *TIMER2n, TIMER2n_INT_Type TIMERIntCfg, FunctionalState NewState);
HAL_Status_Type HAL_TIMER2n_ClearCounter(TIMER2n_Type *TIMER2n);
uint32_t HAL_TIMER2n_GetMatchInterrupt(TIMER2n_Type *TIMER2n);
HAL_Status_Type HAL_TIMER2n_ClearMatchInterrupt(TIMER2n_Type *TIMER2n);
uint32_t HAL_TIMER2n_GetCaptureData(TIMER2n_Type *TIMER2n);
uint32_t HAL_TIMER2n_GetCaptureInterrupt(TIMER2n_Type *TIMER2n);
HAL_Status_Type HAL_TIMER2n_ClearCaptureInterrupt(TIMER2n_Type *TIMER2n);



#ifdef __cplusplus
}
#endif

#endif /* _TIMER2n_H_ */

/* --------------------------------- End Of File ------------------------------ */
