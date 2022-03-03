/**********************************************************************
* @file		A31G21x_timer2n.h
* @brief	      Timer 2n function header file
* @version	1.0
* @date: 2017-3-3
* @author	ABOV Application Team
*
* Copyright(C) 2016, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#ifndef _TIMER2n_H_
#define _TIMER2n_H_

//------------------------------- Includes ----------------------------
#include "A31G21x.h"
#include "aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**********************************************************************
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




/**********************************************************************
* Timer/Counter 2n A Data Register (ADR)
**********************************************************************/
#define TIMER2n_ADR_MASK	0xFFFFFFFF


/**********************************************************************
* Timer/Counter 2n B Data Register (BDR)
**********************************************************************/
#define TIMER2n_BDR_MASK	0xFFFFFFFF


/**********************************************************************
* Timer/Counter 2n Prescaler Data Register (PREDR)
**********************************************************************/
#define TIMER2n_PREDR_MASK					(0xFFF<<0)
#define TIMER2n_PREDR_SET(n)				(n<<0)



/**********************************************************************
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
	PERIODIC_MODE = 0,	/*!< PERIODIC mode */
	CAPTURE_MODE,				/*!< PWM mode */
	ONESHOT_MODE,				/*!< ONE SHOT mode */
	PWM_MODE						/*!< CAPTURE mode */
} TIMER2n_MODE_OPT;

/** @brief clear select when capture mode */
typedef enum
{
	FALLING_EGDE = 0,		/*!< rising edge clear mode */
	RISING_EGDE,				/*!< falling edge clear mode */
	BOTH_EGDE,					/*!< both edge clear  mode */
	NONE								/*!< none clear mode */
} TIMER2n_CLR_MODE_OPT;

/** @brief Timer clock source select */
typedef enum
{
	SOSC_CLK = 0,		/*!< clock source from Internal */
	PCLK_CLK,					/*!< clock source from External */
} TIMER2n_MASTER_CKSEL_OPT;


/** @brief counter clock source select */
typedef enum
{
	MCCR2PCLK = 0,		/*!< clock source from Internal */
	PCLK = 0,		/*!< clock source from Internal */
	EC2n=1,				/*!< clock source from External */
} TIMER2n_CKSEL_MODE_OPT;

/** @brief capture clock source select */
typedef enum
{
	CAP_EXTERNAL_CLK = 0,		/*!< clock source from Externl */
	CAP_SOSC_CLK,				/*!< clock source from XOSC */
	CAP_WDTRC_CLK,				/*!< Clock Source from WDTRC */
} TIMER2n_CAP_CKSEL_OPT;


/** @brief start default level select: initial output value. */
typedef enum
{
	START_LOW = 0,		/*!< clock source from pclk div 2 */
	START_HIGH			/*!< clock source from pclk div 4 */
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
void TIMER2n_Init(TIMER2n_Type *TIMER2n, TIMER2n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct);
void TIMER2n_DeInit (TIMER2n_Type *TIMER2n);

/* TIM configuration functions --------*/
void TIMER2n_Cmd(TIMER2n_Type *TIMER2n, FunctionalState NewState);
void TIMER2n_IntConfig(TIMER2n_Type *TIMER2n, TIMER2n_INT_Type TIMERIntCfg, FunctionalState NewState);
void TIMER2n_ClrCnt(TIMER2n_Type *TIMER2n);
uint32_t TIMER2nMaInt_GetFg(TIMER2n_Type *TIMER2n);
void TIMER2nMaInt_ClrFg(TIMER2n_Type *TIMER2n);
uint32_t TIMER2n_GetCapData(TIMER2n_Type *TIMER2n);
uint32_t TIMER2nCapInt_GetFg(TIMER2n_Type *TIMER2n);
void TIMER2nCapInt_ClrFg(TIMER2n_Type *TIMER2n);



#ifdef __cplusplus
}
#endif

#endif /* _TIMER2n_H_ */

/* --------------------------------- End Of File ------------------------------ */
