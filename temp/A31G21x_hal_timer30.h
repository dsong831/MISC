/**********************************************************************//**
* @file		A31G21x_hal_timer30.h
* @brief	       Contains all macro definitions and function prototypes support
*                     Timer 30 function header file.
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


#ifndef _TIMER30_H_
#define _TIMER30_H_

//------------------------------- Includes ----------------------------
#include "A31G21x.h"
#include "A31G21x_hal_aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif
//************* T30CR******************************************************
//*************************************************************************

/**  Timer 30 Enable/Disable Definition */
#define TIMER30_DISABLE						(0x0uL<<15)
#define TIMER30_ENABLE						(0x1uL<<15)

/**   Timer 30 Clock Selection Definition */
#define TIMER30_CLKINT						(0x0uL<<14)
#define TIMER30_CLKEXT						(0x1uL<<14)

/**  Timer 30 Mode Selection Definition */
#define TIMER30_INVM						(0x0uL<<12)
#define TIMER30_CAPM						(0x1uL<<12)
#define TIMER30_BTOB						(0x2uL<<12)

/**   Timer 30 External Clock Edge Selection Definition */
#define TIMER30_FEDGE						(0x0uL<<11)
#define TIMER30_REDGE						(0x1uL<<11)

/**   Timer 30 Output Mode Selection Definition */
#define TIMER30_6CHMOD						(0x0uL<<10)
#define TIMER30_FORAMOD						(0x1uL<<10)

/**   Timer 30 Delay Time Insert En/Disable Definition */
#define TIMER30_DLYINSDIS					(0x0uL<<9)
#define TIMER30_DLYINSEN					(0x1uL<<9)

/**  Timer 30 Delay Timer Insertion Position Definition */
#define TIMER30_INSFRONT					(0x0uL<<8)
#define TIMER30_INSBACK						(0x1uL<<8)

/**   Timer30 Capture Polarity Selection Definition */
#define TIMER30_CAPFALL						(0x0uL<<6)
#define TIMER30_CAPRISE						(0x1uL<<6)
#define TIMER30_CAPBOTH						(0x2uL<<6)

/**   Timer 30 Data Reload Time Selection Definition */
#define TIMER30_UPWRITE						(0x0uL<<4)
#define TIMER30_UPMATCH						(0x1uL<<4)
#define TIMER30_UPBOTTOM					(0x2uL<<4)

/**   Timer 30 Period Match Interrupt Occurrence Selection Definition */
#define TIMER30_E1PERIOD					(0x00uL<<0)
#define TIMER30_E2PERIOD					(0x01uL<<0)
#define TIMER30_E3PERIOD					(0x02uL<<0)
#define TIMER30_E4PERIOD					(0x03uL<<0)
#define TIMER30_E5PERIOD					(0x04uL<<0)
#define TIMER30_E6PERIOD					(0x05uL<<0)
#define TIMER30_E7PERIOD					(0x06uL<<0)
#define TIMER30_E8PERIOD					(0x07uL<<0)

//************* T30OUTCR ************************************************
//****************************************************************************

/**  Timer 30 T30OUTCR Write Key Definition */
#define TIMER30O_WRITEKEY					(0xE06CuL<<16)

/**   Timer 30 PWM30xB Output Polarity Selection Definition  */
#define TIMER30O_BPOLOW						(0x0uL<<15)
#define TIMER30O_BPOHIGH					(0x1uL<<15)

/**   Timer 30 PWM30xA Output Polarity Selection Definition  */
#define TIMER30O_APOLOW						(0x0uL<<14)
#define TIMER30O_APOHIGH					(0x1uL<<14)

/**   Timer 30 PWM30AB Output En/Disable Definition  */
#define TIMER30O_PWMABDIS					(0x0uL<<13)
#define TIMER30O_PWMABEN					(0x1uL<<13)

/**   Timer 30 PWM30BB Output En/Disable Definition  */
#define TIMER30O_PWMBBDIS					(0x0uL<<12)
#define TIMER30O_PWMBBEN					(0x1uL<<12)

/**   Timer 30 PWM30CB Output En/Disable Definition  */
#define TIMER30O_PWMCBDIS					(0x0uL<<11)
#define TIMER30O_PWMCBEN					(0x1uL<<11)

/**   Timer 30 PWM30AA Output En/Disable Definition */
#define TIMER30O_PWMAADIS					(0x0uL<<10)
#define TIMER30O_PWMAAEN					(0x1uL<<10)

/**  Timer 30 PWM30BA Output En/Disable Definition  */
#define TIMER30O_PWMBADIS					(0x0uL<<9)
#define TIMER30O_PWMBAEN					(0x1uL<<9)

/**   Timer 30 PWM30CA Output En/Disable Definition  */
#define TIMER30O_PWMCADIS					(0x0uL<<8)
#define TIMER30O_PWMCAEN					(0x1uL<<8)

/**   Timer 30 PWM30AB Output When Disable  */
#define TIMER30O_ABLOW						(0x0uL<<6)
#define TIMER30O_ABHIGH						(0x1uL<<6)

/**   Timer 30 PWM30BB Output When Disable  */
#define TIMER30O_BBLOW						(0x0uL<<5)
#define TIMER30O_BBHIGH						(0x1uL<<5)

/**   Timer 30 PWM30CB Output When Disable  */
#define TIMER30O_CBLOW						(0x0uL<<4)
#define TIMER30O_CBHIGH						(0x1uL<<4)

/**   Timer 30 PWM30AA Output When Disable   */
#define TIMER30O_AALOW						(0x0uL<<2)
#define TIMER30O_AAHIGH						(0x1uL<<2)

/**   Timer 30 PWM30BA Output When Disable  */
#define TIMER30O_BALOW						(0x0uL<<1)
#define TIMER30O_BAHIGH						(0x1uL<<1)

/**  Timer 30 PWM30CA Output When Disable  */
#define TIMER30O_CALOW						(0x0uL<<0)
#define TIMER30O_CAHIGH						(0x1uL<<0)

//************* T30INTCR *************************************************
//*************************************************************************
/**  Timer 30 High-Impedance Interrupt EN/Disable Definition */
#define TIMER30INT_HIZDIS						(0x0uL<<6)
#define TIMER30INT_HIZEN						(0x1uL<<6)

/**   Timer 30 Capture Interrupt EN/Disable Definition  */
#define TIMER30INT_CAPDIS						(0x0uL<<5)
#define TIMER30INT_CAPEN						(0x1uL<<5)

/**   Timer 30 Bottom Interrupt EN/Disable Definition  */
#define TIMER30INT_BOTDIS						(0x0uL<<4)
#define TIMER30INT_BOTEN						(0x1uL<<4)

/**  Timer 30 Period Match Interrupt EN/Disable Definition  */
#define TIMER30INT_PMATDIS					(0x0uL<<3)
#define TIMER30INT_PMATEN						(0x1uL<<3)

/**  Timer 30 A Match Interrupt EN/Disable Definition  */
#define TIMER30INT_AMATDIS					(0x0uL<<2)
#define TIMER30INT_AMATEN						(0x1uL<<2)

/**   Timer 30 B Match Interrupt EN/Disable Definition  */
#define TIMER30INT_BMATDIS					(0x0uL<<1)
#define TIMER30INT_BMATEN						(0x1uL<<1)

/**   Timer 30 C Match Interrupt EN/Disable Definition  */
#define TIMER30INT_CMATDIS					(0x0uL<<0)
#define TIMER30INT_CMATEN						(0x1uL<<0)


//************* T30HIZCR *************************************************
//****************************************************************************
/**   Timer 30 PWM Output High-Impedance En/Disable Definition  */
#define TIMER30HIZ_DISABLE					(0x0uL<<7)
#define TIMER30HIZ_ENABLE					(0x1uL<<7)

/**   Timer 30 High-Impedance(BLNK) Edge Definition  */
#define TIMER30HIZ_BLNKFALL					(0x0uL<<2)
#define TIMER30HIZ_BLNKRISE					(0x1uL<<2)


//************* T30ADTCR *************************************************
//****************************************************************************
/**   Timer 30 Bottom for A/DC Trigger Signal Generator EN/Disable Definition   */
#define TIMER30ADT_BTTGDIS					(0x0uL<<4)
#define TIMER30ADT_BTTGEN					(0x1uL<<4)

/**   Timer 30 Period Match for A/DC Trigger Signal Generator EN/Disable Definition  */
#define TIMER30ADT_PMTGDIS					(0x0uL<<3)
#define TIMER30ADT_PMTGEN					(0x1uL<<3)

/**   Timer 30 A-ch Match for A/DC Trigger Signal Generator EN/Disable Definition  */
#define TIMER30ADT_AMTGDIS					(0x0uL<<2)
#define TIMER30ADT_AMTGEN					(0x1uL<<2)
 
/**   Timer 30 B-ch Match for A/DC Trigger Signal Generator EN/Disable Definition  */
#define TIMER30ADT_BMTGDIS					(0x0uL<<1)
#define TIMER30ADT_BMTGEN					(0x1uL<<1)

/**   Timer 30 C-ch Match for A/DC Trigger Signal Generator EN/Disable Definition  */
#define TIMER30ADT_CMTGDIS					(0x0uL<<0)
#define TIMER30ADT_CMTGEN					(0x1uL<<0)







/** ----------------------------------------------------------------------------
   @brief        Timer 30 Enable/Disable
   @param[in]    Timer_Type           T30
   @explain      This macro Enable Timer 30 Block
 *----------------------------------------------------------------------------*/
#define TIMER30_EN()							(T30->T30CR_b.T30EN = 1)
#define TIMER30_DIS()							(T30->T30CR_b.T30EN = 0)

/** ----------------------------------------------------------------------------
   @brief         Timer 30 Counter and Prescaler clear
   @param[in]    Timer_Type           T30
   @explain      This macro clears Counter and Prescalrer of Timer 30
 *----------------------------------------------------------------------------*/
#define TIMER30_ClrCnt()						(T30->T30CR_b.T30CLR = 1)

/** ----------------------------------------------------------------------------
   @brief         Get Timer 30 Counter Register
   @param[in]    Timer_Type           T30
   @explain      This macro gets Timer 30 Counter Register
 *----------------------------------------------------------------------------*/
#define TIMER30_GetCnt()						(T30->T30CNT)

/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 Period Match Interrupt Occurrence
   @param[in]    Timer_Type           T30
   @explain      This macro sets Period Match Interrupt Occurrence
 *----------------------------------------------------------------------------*/
#define TIMER30_SetPMOC(u32PMOC)				(T30->T30CR_b.PMOC = u32PMOC)

/** ----------------------------------------------------------------------------
   @brief        Timer 30 PWM Output High-Impedance Enable/Disable
   @param[in]    Timer_Type           T30
   @explain      This macro Enable Timer 30 PWM Output High-Impedance
 *----------------------------------------------------------------------------*/
#define TIMER30_HIZEN()							(T30->T30HIZCR_b.T30EN = 1)
#define TIMER30_HIZDIS()						(T30->T30HIZCR_b.T30EN = 0)

/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 PWM Output High-Impedance by Software
   @param[in]    Timer_Type           T30
   @explain      This macro sets Timer 30 PWM Output High-Impedance by Software
 *----------------------------------------------------------------------------*/
#define TIMER30_SetHIZSW()						(T30->T30HIZCR_b.HIZSW = 1)

/** ----------------------------------------------------------------------------
   @brief         Timer 30 High-Impedance Output Clear
   @param[in]    Timer_Type           T30
   @explain      This macro clears Timer 30 High-Impedance Output 
 *----------------------------------------------------------------------------*/
#define TIMER30_ClrHIZ()						(T30->T30HIZCR_b.HIZCLR = 1)

/** ----------------------------------------------------------------------------
   @brief         Get Timer 30 High-Impedance Status
   @param[in]    Timer_Type           T30
   @explain      This macro gets Timer 30 High-Impedance Status
 *----------------------------------------------------------------------------*/
#define TIMER30_GetHIZStaus()					(T30->T30HIZCR_b.HIZSTA)





// Set & Get Data Register
/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 Period Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32AData         Period Data of Timer 30
   @explain      This macro sets Timer 30 Period Data Register
 *----------------------------------------------------------------------------*/
#define TIMER30_SetPeData(u32PData)							(T30->T30PDR = u32PData)

/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 A Data Register
   @param[in]    Timer_Type           T20, T21
   @param[in]    u32AData         A Data of Timer 30
   @explain      This macro sets Timer 30 A Data Register
 *----------------------------------------------------------------------------*/
#define TIMER30_SetAData(u32AData)							(T30->T30ADR = u32AData)

/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 B Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32BData         B Data of Timer 30
   @explain      This macro sets Timer 30 B Data Register
 *----------------------------------------------------------------------------*/
#define TIMER30_SetBData(u32BData)							(T30->T30BDR = u32BData)

/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 C Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32CData         C Data of Timer 30
   @explain      This macro sets Timer 30 C Data Register
 *----------------------------------------------------------------------------*/
#define TIMER30_SetCData(u32CData)							(T30->T30CDR = u32CData)

/** ----------------------------------------------------------------------------
   @brief         Get Timer 30 Capture Data Register
   @param[in]    Timer_Type           T30
   @explain      This macro gets Timer 30 Capture Data Register
 *----------------------------------------------------------------------------*/
#define TIMER30_GetCapData()								(T30->T30CAPDR)

/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 PWM Output Delay Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32DelayData         PWM Output Delay Data of Timer 30
   @explain      This macro sets Timer 30 PWM Output Delay Data Register
 *----------------------------------------------------------------------------*/
#define TIMER30_SetDelayData(u32DelayData)					(T30->T30DLY = u32DelayData)

/** ----------------------------------------------------------------------------
   @brief         Set Timer 30 A/DC Trigger Generator Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32ADTData         A/DC Trigger Generator Data of Timer 30
   @explain      This macro sets Timer 30 A/DC Trigger Generator Data Register
 *----------------------------------------------------------------------------*/
#define TIMER30_SetADTData(u32ADTData)						(T30->T30ADTDR = u32ADTData)





// Get & Clear Interrupt Flag
/** ----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 All Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 All Interrupt
 *----------------------------------------------------------------------------*/
#define T30AllInt_GetFg()						(T30->T30INTFLAG)

/** ----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 C-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 C-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30CMaInt_GetFg()						(T30->T30INTFLAG_b.T30CMIFLAG)

/** ----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 B-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 B-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30BMaInt_GetFg()						(T30->T30INTFLAG_b.T30BMIFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 A-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 A-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30AchMaInt_GetFg()						(T30->T30INTFLAG_b.T30AMIFLAG)

/** ----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 Period Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 Period Match Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30PeMaInt_GetFg()						(T30->T30INTFLAG_b.T30PMIFLAG)

/** ----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 Bottom Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 Bottom Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30BotMaInt_GetFg()						(T30->T30INTFLAG_b.T30BTIFLAG)

/** ----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 Capture Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 Capture Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30CapInt_GetFg()                         (T30->T30INTFLAG_b.T30CIFLAG)

/** ----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 High-Impedance Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 High-Impedance Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30HIZInt_GetFg()                         (T30->T30INTFLAG_b.HIZIFLAG)

/** ----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 All Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro Clears interrupt flag of Timer 30 All Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30AllInt_ClrFg()						(T30->T30INTFLAG = 0x7F)

/** ----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 C-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 C-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30CchMaInt_ClrFg()						(T30->T30INTFLAG_b.T30CMIFLAG = 1)

/** ----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 B-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 B-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30BchMaInt_ClrFg()						(T30->T30INTFLAG_b.T30BMIFLAG = 1)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 A-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 A-ch Match Interrupt
 ** ----------------------------------------------------------------------------*/
#define TIMER30AchMaInt_ClrFg()						(T30->T30INTFLAG_b.T30AMIFLAG = 1)

/** ----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 Period Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 Period Match Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30PeMaInt_ClrFg()						(T30->T30INTFLAG_b.T30PMIFLAG = 1)

/**----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 Bottom Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 Bottom Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30BotMaInt_ClrFg()						(T30->T30INTFLAG_b.T30BTIFLAG = 1)

/**----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 Capture Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 Capture Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30CapInt_ClrFg()                       (T30->T30INTFLAG_b.T30CIFLAG = 1)

/**----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 High-Impedance Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 High-Impedance Interrupt
 *----------------------------------------------------------------------------*/
#define TIMER30HIZInt_ClrFg()                       (T30->T30INTFLAG_b.HIZIFLAG = 1)





//------------------------- Internal Functions ------------------------
HAL_Status_Type HAL_TIMER3n_SetPeriod(TIMER3n_Type* TIMER3n, uint32_t prescale);
HAL_Status_Type HAL_TIMER3n_SetPeriod(TIMER3n_Type* TIMER3n, uint32_t period);
HAL_Status_Type HAL_TIMER3n_SetADuty(TIMER3n_Type* TIMER3n, uint32_t period);
HAL_Status_Type HAL_TIMER3n_SetBDuty(TIMER3n_Type* TIMER3n, uint32_t period);
HAL_Status_Type HAL_TIMER3n_SetCDuty(TIMER3n_Type* TIMER3n, uint32_t period);
HAL_Status_Type HAL_TIMER3n_SetDelayTime(TIMER3n_Type* TIMER3n, uint32_t dten, uint32_t dtpos, uint32_t clkdata);
HAL_Status_Type HAL_TIMER3n_Start(TIMER3n_Type* TIMER3n, uint32_t NewState);
HAL_Status_Type HAL_TIMER3n_OutputCtrl(TIMER3n_Type* TIMER3n,  uint32_t NewState, uint32_t pwmApol, uint32_t pwmBpol);
HAL_Status_Type HAL_TIMER3n_ConfigInterrupt(TIMER3n_Type* TIMER3n,uint32_t NewState,uint32_t T30IntCfg);
HAL_Status_Type HAL_TIMER3n_ClearStatus_IT(TIMER3n_Type* TIMER3n,uint32_t T30IntCfg);
uint32_t HAL_TIMER3n_GetStatus_IT(TIMER3n_Type* TIMER3n);
HAL_Status_Type HAL_TIMER3n_MPWMCmd(TIMER3n_Type* TIMER3n, uint32_t updatedata, uint32_t intcount);
HAL_Status_Type HAL_TIMER3n_SetHizReg(TIMER3n_Type* TIMER3n, uint32_t u32T30HizSet);
HAL_Status_Type HAL_TIMER3n_SetADCTrigger(TIMER3n_Type* TIMER3n, uint32_t u32triggerpoint, uint32_t u32triggertime);
HAL_Status_Type HAL_TIMER3n_Init(TIMER3n_Type *TIMER3n);
#ifdef __cplusplus
}
#endif

#endif /* _T30_H_ */

