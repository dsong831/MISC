/**********************************************************************
* @file		A31G21x_timer1n.h
* @brief	      Timer 1n function header file
* @version	1.0
* @date: 2017-08-10
* @author	ABOV Application Team
*
* Copyright(C) 2017, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#ifndef _TIMER1n_H_
#define _TIMER1n_H_

//------------------------------- Includes ----------------------------
#include "A31G21x.h"
#include "aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------- Timer 1n Enable/Disable Definition ------------
#define T1n_DISABLE                    	(0x0uL<<15)
#define T1n_ENABLE                    	(0x1uL<<15)

//-------- Timer 1n Clock Selection Definition ------------
#define T1n_CLKINT                    	(0x0uL<<14)
#define T1n_CLKEXT                    	(0x1uL<<14)

//-------- Timer 1n Mode Selection Definition ------------
#define T1n_CNTM                    		(0x0uL<<12)
#define T1n_CAPM                    		(0x1uL<<12)
#define T1n_PPGONEM                 		(0x2uL<<12)
#define T1n_PPGREM                  		(0x3uL<<12)

//-------- Timer 1n External Clock Edge Selection Definition ------------
#define T1n_FEDGE                    		(0x0uL<<11)
#define T1n_REDGE                    		(0x1uL<<11)

//-------- Timer 1n OUT Polarity Selection Definition ------------
#define T1n_STHIGH                    	(0x0uL<<8)
#define T1n_STLOW                    		(0x1uL<<8)

//-------- Timer 1n Capture Polarity Selection Definition ------------
#define T1n_CAPFALL                    	(0x0uL<<6)
#define T1n_CAPRISE                    	(0x1uL<<6)
#define T1n_CAPBOTH                    	(0x2uL<<6)

//-------- Timer 1n Match Interrupt Definition ------------
#define T1n_MATINTEN                    (0x1uL<<5)
#define T1n_MATINTDIS                  	(0x0uL<<5)

//-------- Timer 1n Capture Interrupt Definition ------------
#define T1n_CAPINTEN                   	(0x1uL<<4)
#define T1n_CAPINTDIS                   (0x0uL<<4)


//------------------------- Internal Functions ------------------------
extern void T1n_SetReg(TIMER1n_Type *, uint32_t, uint32_t);



#define T1n_PRS_MASK	0x0FFF

/***********************************************************************
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
	MCCR1PCLK = 0,		/*!< clock source from pclk or mccr1 */
	ECn = 1			/*!< clock source from ECn pin input. before setting, have to set ECn pin mode */	
} TIMER1n_CKSEL_MODE_OPT;

typedef enum
{
	PERIODIC_MODE = 0,	/*!< PERIODIC mode */
	CAPTURE_MODE,			/*!< CAPTURE mode */
	ONESHOT_MODE,		/*!< ONE SHOT mode */
	PWM_MODE				/*!< PWM mode */
} TIMER1n_MODE_OPT;

typedef enum
{
	FALLING_EGDE=0,		/*!< falling edge clear mode */
	RISING_EGDE,	/*!< rising edge clear mode */
	BOTH_EGDE,			/*!< both edge clear  mode */
	NONE						/*!< none clear mode */
} TIMER1n_CLR_MODE_OPT;

/** @brief start default level select: initial output value. */
typedef enum
{
	START_HIGH=0,
	START_LOW
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
void TIMER1n_Init(TIMER1n_Type *TIMER1n, TIMER1n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct);
void TIMER1n_DeInit (TIMER1n_Type *TIMER1n);

/* TIMER1n configuration functions --------*/
void TIMER1n_Cmd(TIMER1n_Type *TIMER1n, FunctionalState NewState);
void TIMER1n_IntConfig(TIMER1n_Type *TIMER1n, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState);
void TIMER1n_ClrCnt(TIMER1n_Type *TIMER1n);
uint32_t TIMER1nMaInt_GetFg(TIMER1n_Type *TIMER1n);
void TIMER1nMaInt_ClrFg(TIMER1n_Type *TIMER1n);
uint32_t TIMER1n_GetCapData(TIMER1n_Type *TIMER1n);
uint32_t TIMER1nCapInt_GetFg(TIMER1n_Type *TIMER1n);
void TIMER1nCapInt_ClrFg(TIMER1n_Type *TIMER1n);

#ifdef __cplusplus
}
#endif

#endif /*_TIMER1n_H_ */

