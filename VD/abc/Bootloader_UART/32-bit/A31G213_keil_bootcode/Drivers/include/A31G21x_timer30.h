/**********************************************************************
* @file		A31G21x_timer30.h
* @brief	      Timer 30 function header file
* @version	1.0
* @date:       2017-3-3
* @author	ABOV Application Team
*
* Copyright(C) 2017, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#ifndef _TIMER30_H_
#define _TIMER30_H_

//------------------------------- Includes ----------------------------
#include "A31G21x.h"
#include "aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif
//************* T30CR******************************************************
//****************************************************************************

//-------- Timer 30 Enable/Disable Definition ------------
#define T30_DISABLE						(0x0uL<<15)
#define T30_ENABLE						(0x1uL<<15)

//-------- Timer 30 Clock Selection Definition ------------
#define T30_CLKINT						(0x0uL<<14)
#define T30_CLKEXT						(0x1uL<<14)

//-------- Timer 30 Mode Selection Definition ------------
#define T30_INVM						(0x0uL<<12)
#define T30_CAPM						(0x1uL<<12)
#define T30_BTOB						(0x2uL<<12)

//-------- Timer 30 External Clock Edge Selection Definition ------------
#define T30_FEDGE						(0x0uL<<11)
#define T30_REDGE						(0x1uL<<11)

//-------- Timer 30 Output Mode Selection Definition ------------
#define T30_6CHMOD						(0x0uL<<10)
#define T30_FORAMOD						(0x1uL<<10)

//-------- Timer 30 Delay Time Insert En/Disable Definition ------------
#define T30_DLYINSDIS					(0x0uL<<9)
#define T30_DLYINSEN					(0x1uL<<9)

//-------- Timer 30 Delay Timer Insertion Position Definition ------------
#define T30_INSFRONT					(0x0uL<<8)
#define T30_INSBACK						(0x1uL<<8)

//-------- Timer30 Capture Polarity Selection Definition ------------
#define T30_CAPFALL						(0x0uL<<6)
#define T30_CAPRISE						(0x1uL<<6)
#define T30_CAPBOTH						(0x2uL<<6)

//-------- Timer 30 Data Reload Time Selection Definition ------------
#define T30_UPWRITE						(0x0uL<<4)
#define T30_UPMATCH						(0x1uL<<4)
#define T30_UPBOTTOM					(0x2uL<<4)

//-------- Timer 30 Period Match Interrupt Occurrence Selection Definition ------------
#define T30_E1PERIOD					(0x00uL<<0)
#define T30_E2PERIOD					(0x01uL<<0)
#define T30_E3PERIOD					(0x02uL<<0)
#define T30_E4PERIOD					(0x03uL<<0)
#define T30_E5PERIOD					(0x04uL<<0)
#define T30_E6PERIOD					(0x05uL<<0)
#define T30_E7PERIOD					(0x06uL<<0)
#define T30_E8PERIOD					(0x07uL<<0)

//************* T30OUTCR ************************************************
//****************************************************************************

//-------- Timer 30 T30OUTCR Write Key Definition ------------
#define T30O_WRITEKEY					(0xE06CuL<<16)

//-------- Timer 30 PWM30xB Output Polarity Selection Definition ------------
#define T30O_BPOLOW						(0x0uL<<15)
#define T30O_BPOHIGH					(0x1uL<<15)

//-------- Timer 30 PWM30xA Output Polarity Selection Definition ------------
#define T30O_APOLOW						(0x0uL<<14)
#define T30O_APOHIGH					(0x1uL<<14)

//-------- Timer 30 PWM30AB Output En/Disable Definition ------------
#define T30O_PWMABDIS					(0x0uL<<13)
#define T30O_PWMABEN					(0x1uL<<13)

//-------- Timer 30 PWM30BB Output En/Disable Definition ------------
#define T30O_PWMBBDIS					(0x0uL<<12)
#define T30O_PWMBBEN					(0x1uL<<12)

//-------- Timer 30 PWM30CB Output En/Disable Definition ------------
#define T30O_PWMCBDIS					(0x0uL<<11)
#define T30O_PWMCBEN					(0x1uL<<11)

//-------- Timer 30 PWM30AA Output En/Disable Definition ------------
#define T30O_PWMAADIS					(0x0uL<<10)
#define T30O_PWMAAEN					(0x1uL<<10)

//-------- Timer 30 PWM30BA Output En/Disable Definition ------------
#define T30O_PWMBADIS					(0x0uL<<9)
#define T30O_PWMBAEN					(0x1uL<<9)

//-------- Timer 30 PWM30CA Output En/Disable Definition ------------
#define T30O_PWMCADIS					(0x0uL<<8)
#define T30O_PWMCAEN					(0x1uL<<8)

//-------- Timer 30 PWM30AB Output When Disable ---------
#define T30O_ABLOW						(0x0uL<<6)
#define T30O_ABHIGH						(0x1uL<<6)

//-------- Timer 30 PWM30BB Output When Disable ---------
#define T30O_BBLOW						(0x0uL<<5)
#define T30O_BBHIGH						(0x1uL<<5)

//-------- Timer 30 PWM30CB Output When Disable ---------
#define T30O_CBLOW						(0x0uL<<4)
#define T30O_CBHIGH						(0x1uL<<4)

//-------- Timer 30 PWM30AA Output When Disable ---------
#define T30O_AALOW						(0x0uL<<2)
#define T30O_AAHIGH						(0x1uL<<2)

//-------- Timer 30 PWM30BA Output When Disable ---------
#define T30O_BALOW						(0x0uL<<1)
#define T30O_BAHIGH						(0x1uL<<1)

//-------- Timer 30 PWM30CA Output When Disable ---------
#define T30O_CALOW						(0x0uL<<0)
#define T30O_CAHIGH						(0x1uL<<0)

//************* T30INTCR *************************************************
//****************************************************************************
//-------- Timer 30 High-Impedance Interrupt EN/Disable Definition------
#define T30INT_HIZDIS						(0x0uL<<6)
#define T30INT_HIZEN						(0x1uL<<6)

//-------- Timer 30 Capture Interrupt EN/Disable Definition--------
#define T30INT_CAPDIS						(0x0uL<<5)
#define T30INT_CAPEN						(0x1uL<<5)

//-------- Timer 30 Bottom Interrupt EN/Disable Definition---------
#define T30INT_BOTDIS						(0x0uL<<4)
#define T30INT_BOTEN						(0x1uL<<4)

//-------- Timer 30 Period Match Interrupt EN/Disable Definition----------
#define T30INT_PMATDIS					(0x0uL<<3)
#define T30INT_PMATEN						(0x1uL<<3)

//-------- Timer 30 A Match Interrupt EN/Disable Definition---------
#define T30INT_AMATDIS					(0x0uL<<2)
#define T30INT_AMATEN						(0x1uL<<2)

//-------- Timer 30 B Match Interrupt EN/Disable Definition---------
#define T30INT_BMATDIS					(0x0uL<<1)
#define T30INT_BMATEN						(0x1uL<<1)

//-------- Timer 30 C Match Interrupt EN/Disable Definition---------
#define T30INT_CMATDIS					(0x0uL<<0)
#define T30INT_CMATEN						(0x1uL<<0)


//************* T30HIZCR *************************************************
//****************************************************************************
//-------- Timer 30 PWM Output High-Impedance En/Disable Definition---------
#define T30HIZ_DISABLE					(0x0uL<<7)
#define T30HIZ_ENABLE					(0x1uL<<7)

//-------- Timer 30 High-Impedance(BLNK) Edge Definition---------
#define T30HIZ_BLNKFALL					(0x0uL<<2)
#define T30HIZ_BLNKRISE					(0x1uL<<2)


//************* T30ADTCR *************************************************
//****************************************************************************
//-------- Timer 30 Bottom for A/DC Trigger Signal Generator EN/Disable Definition---------
#define T30ADT_BTTGDIS					(0x0uL<<4)
#define T30ADT_BTTGEN					(0x1uL<<4)

//-------- Timer 30 Period Match for A/DC Trigger Signal Generator EN/Disable Definition---------
#define T30ADT_PMTGDIS					(0x0uL<<3)
#define T30ADT_PMTGEN					(0x1uL<<3)

//-------- Timer 30 A-ch Match for A/DC Trigger Signal Generator EN/Disable Definition---------
#define T30ADT_AMTGDIS					(0x0uL<<2)
#define T30ADT_AMTGEN					(0x1uL<<2)

//-------- Timer 30 B-ch Match for A/DC Trigger Signal Generator EN/Disable Definition---------
#define T30ADT_BMTGDIS					(0x0uL<<1)
#define T30ADT_BMTGEN					(0x1uL<<1)

//-------- Timer 30 C-ch Match for A/DC Trigger Signal Generator EN/Disable Definition---------
#define T30ADT_CMTGDIS					(0x0uL<<0)
#define T30ADT_CMTGEN					(0x1uL<<0)







/*----------------------------------------------------------------------------
   @brief        Timer 30 Enable/Disable
   @param[in]    Timer_Type           T30
   @explain      This macro Enable Timer 30 Block
 *----------------------------------------------------------------------------*/
#define T30_EN()							(T30->T30CR_b.T30EN = 1)
#define T30_DIS()							(T30->T30CR_b.T30EN = 0)

/*----------------------------------------------------------------------------
   @brief         Timer 30 Counter and Prescaler clear
   @param[in]    Timer_Type           T30
   @explain      This macro clears Counter and Prescalrer of Timer 30
 *----------------------------------------------------------------------------*/
#define T30_ClrCnt()						(T30->T30CR_b.T30CLR = 1)

/*----------------------------------------------------------------------------
   @brief         Get Timer 30 Counter Register
   @param[in]    Timer_Type           T30
   @explain      This macro gets Timer 30 Counter Register
 *----------------------------------------------------------------------------*/
#define T30_GetCnt()						(T30->T30CNT)

/*----------------------------------------------------------------------------
   @brief         Set Timer 30 Period Match Interrupt Occurrence
   @param[in]    Timer_Type           T30
   @explain      This macro sets Period Match Interrupt Occurrence
 *----------------------------------------------------------------------------*/
#define T30_SetPMOC(u32PMOC)				(T30->T30CR_b.PMOC = u32PMOC)

/*----------------------------------------------------------------------------
   @brief        Timer 30 PWM Output High-Impedance Enable/Disable
   @param[in]    Timer_Type           T30
   @explain      This macro Enable Timer 30 PWM Output High-Impedance
 *----------------------------------------------------------------------------*/
#define T30_HIZEN()							(T30->T30HIZCR_b.T30EN = 1)
#define T30_HIZDIS()						(T30->T30HIZCR_b.T30EN = 0)

/*----------------------------------------------------------------------------
   @brief         Set Timer 30 PWM Output High-Impedance by Software
   @param[in]    Timer_Type           T30
   @explain      This macro sets Timer 30 PWM Output High-Impedance by Software
 *----------------------------------------------------------------------------*/
#define T30_SetHIZSW()						(T30->T30HIZCR_b.HIZSW = 1)

/*----------------------------------------------------------------------------
   @brief         Timer 30 High-Impedance Output Clear
   @param[in]    Timer_Type           T30
   @explain      This macro clears Timer 30 High-Impedance Output 
 *----------------------------------------------------------------------------*/
#define T30_ClrHIZ()						(T30->T30HIZCR_b.HIZCLR = 1)

/*----------------------------------------------------------------------------
   @brief         Get Timer 30 High-Impedance Status
   @param[in]    Timer_Type           T30
   @explain      This macro gets Timer 30 High-Impedance Status
 *----------------------------------------------------------------------------*/
#define T30_GetHIZStaus()					(T30->T30HIZCR_b.HIZSTA)





// Set & Get Data Register
/*----------------------------------------------------------------------------
   @brief         Set Timer 30 Period Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32AData         Period Data of Timer 30
   @explain      This macro sets Timer 30 Period Data Register
 *----------------------------------------------------------------------------*/
#define T30_SetPeData(u32PData)							(T30->T30PDR = u32PData)

/*----------------------------------------------------------------------------
   @brief         Set Timer 30 A Data Register
   @param[in]    Timer_Type           T20, T21
   @param[in]    u32AData         A Data of Timer 30
   @explain      This macro sets Timer 30 A Data Register
 *----------------------------------------------------------------------------*/
#define T30_SetAData(u32AData)							(T30->T30ADR = u32AData)

/*----------------------------------------------------------------------------
   @brief         Set Timer 30 B Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32BData         B Data of Timer 30
   @explain      This macro sets Timer 30 B Data Register
 *----------------------------------------------------------------------------*/
#define T30_SetBData(u32BData)							(T30->T30BDR = u32BData)

/*----------------------------------------------------------------------------
   @brief         Set Timer 30 C Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32CData         C Data of Timer 30
   @explain      This macro sets Timer 30 C Data Register
 *----------------------------------------------------------------------------*/
#define T30_SetCData(u32CData)							(T30->T30CDR = u32CData)

/*----------------------------------------------------------------------------
   @brief         Get Timer 30 Capture Data Register
   @param[in]    Timer_Type           T30
   @explain      This macro gets Timer 30 Capture Data Register
 *----------------------------------------------------------------------------*/
#define T30_GetCapData()								(T30->T30CAPDR)

/*----------------------------------------------------------------------------
   @brief         Set Timer 30 PWM Output Delay Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32DelayData         PWM Output Delay Data of Timer 30
   @explain      This macro sets Timer 30 PWM Output Delay Data Register
 *----------------------------------------------------------------------------*/
#define T30_SetDelayData(u32DelayData)					(T30->T30DLY = u32DelayData)

/*----------------------------------------------------------------------------
   @brief         Set Timer 30 A/DC Trigger Generator Data Register
   @param[in]    Timer_Type           T30
   @param[in]    u32ADTData         A/DC Trigger Generator Data of Timer 30
   @explain      This macro sets Timer 30 A/DC Trigger Generator Data Register
 *----------------------------------------------------------------------------*/
#define T30_SetADTData(u32ADTData)						(T30->T30ADTDR = u32ADTData)





// Get & Clear Interrupt Flag
/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 All Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 All Interrupt
 *----------------------------------------------------------------------------*/
#define T30AllInt_GetFg()						(T30->T30INTFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 C-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 C-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30CMaInt_GetFg()						(T30->T30INTFLAG_b.T30CMIFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 B-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 B-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30BMaInt_GetFg()						(T30->T30INTFLAG_b.T30BMIFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 A-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 A-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30AchMaInt_GetFg()						(T30->T30INTFLAG_b.T30AMIFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 Period Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 Period Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30PeMaInt_GetFg()						(T30->T30INTFLAG_b.T30PMIFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 Bottom Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 Bottom Interrupt
 *----------------------------------------------------------------------------*/
#define T30BotMaInt_GetFg()						(T30->T30INTFLAG_b.T30BTIFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 Capture Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 Capture Interrupt
 *----------------------------------------------------------------------------*/
#define T30CapInt_GetFg()                         (T30->T30INTFLAG_b.T30CIFLAG)

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 30 High-Impedance Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro gets interrupt flag of Timer 30 High-Impedance Interrupt
 *----------------------------------------------------------------------------*/
#define T30HIZInt_GetFg()                         (T30->T30INTFLAG_b.HIZIFLAG)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 All Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro Clears interrupt flag of Timer 30 All Interrupt
 *----------------------------------------------------------------------------*/
#define T30AllInt_ClrFg()						(T30->T30INTFLAG = 0x7F)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 C-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 C-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30CchMaInt_ClrFg()						(T30->T30INTFLAG_b.T30CMIFLAG = 1)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 B-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 B-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30BchMaInt_ClrFg()						(T30->T30INTFLAG_b.T30BMIFLAG = 1)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 A-ch Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 A-ch Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30AchMaInt_ClrFg()						(T30->T30INTFLAG_b.T30AMIFLAG = 1)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 Period Match Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 Period Match Interrupt
 *----------------------------------------------------------------------------*/
#define T30PeMaInt_ClrFg()						(T30->T30INTFLAG_b.T30PMIFLAG = 1)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 Bottom Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 Bottom Interrupt
 *----------------------------------------------------------------------------*/
#define T30BotMaInt_ClrFg()						(T30->T30INTFLAG_b.T30BTIFLAG = 1)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 Capture Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 Capture Interrupt
 *----------------------------------------------------------------------------*/
#define T30CapInt_ClrFg()                       (T30->T30INTFLAG_b.T30CIFLAG = 1)

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 30 High-Impedance Interrupt
   @param[in]    Timer_Type           T30
   @explain      This macro clears interrupt flag of Timer 30 High-Impedance Interrupt
 *----------------------------------------------------------------------------*/
#define T30HIZInt_ClrFg()                       (T30->T30INTFLAG_b.HIZIFLAG = 1)





//------------------------- Internal Functions ------------------------
void T30_ClockPrescaler(TIMER3n_Type* TIMER3n, uint32_t prescale);
void T30_SetPeriod(TIMER3n_Type* TIMER3n, uint32_t period);
void T30_SetADuty(TIMER3n_Type* TIMER3n, uint32_t period);
void T30_SetBDuty(TIMER3n_Type* TIMER3n, uint32_t period);
void T30_SetCDuty(TIMER3n_Type* TIMER3n, uint32_t period);
void T30_SetDelayTime(TIMER3n_Type* TIMER3n, uint32_t dten, uint32_t dtpos, uint32_t clkdata);
void T30_Start(TIMER3n_Type* TIMER3n, uint32_t NewState);
void T30_OutputCtrl(TIMER3n_Type* TIMER3n,  uint32_t NewState, uint32_t pwmApol, uint32_t pwmBpol);
void T30_IntConfig(TIMER3n_Type* TIMER3n,uint32_t NewState,uint32_t T30IntCfg);
void T30_ClearStatus(TIMER3n_Type* TIMER3n,uint32_t T30IntCfg);
uint32_t T30_GetStatus(TIMER3n_Type* TIMER3n);
void T30_MPWMCmd(TIMER3n_Type* TIMER3n, uint32_t updatedata, uint32_t intcount);
void T30_SetHizReg(TIMER3n_Type* TIMER3n, uint32_t u32T30HizSet);
void T30_SetADCTrigger(TIMER3n_Type* TIMER3n, uint32_t u32triggerpoint, uint32_t u32triggertime);
void Timer30_Init(TIMER3n_Type *TIMER3n);
#ifdef __cplusplus
}
#endif

#endif /* _T30_H_ */

