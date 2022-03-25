/**********************************************************************
* @file		A31G21x_timer30.h
* @brief	  Timer 30 functions
* @version	1.0
* @date:       2017-3-3
* @author	ABOV Application Team
*
* Copyright(C) 2017, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#include "A31G21x_timer30.h"

/********************************************************************
 * @brief		Initializes the Timer30 peripheral according to the specified
 *               parameters.
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @return 		None
 *********************************************************************/
void Timer30_Init(TIMER3n_Type *TIMER3n)
{
	//timer30 peri enable 
	SCU->PER1 &= ~(1<<24);
	SCU->PCER1 &= ~(1<<24);
		
	SCU->PER1 |= (1<<24);
	SCU->PCER1 |= (1<<24);	
	
	TIMER3n->CR = 0;
	TIMER3n->PDR = 0;
	TIMER3n->ADR = 0;
	TIMER3n->BDR = 0;
	TIMER3n->PREDR = 0;
	TIMER3n->OUTCR = 0;
	TIMER3n->DLY = 0;
	TIMER3n->INTCR = 0;
	TIMER3n->INTFLAG = 0;
	TIMER3n->HIZCR = 0;
	TIMER3n->ADTCR = 0;
	TIMER3n->ADTDR = 0;
}
/*********************************************************************
 * @brief		Interrupt Control Register
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	NewState New State of Timer30 peripheral's operation, should be:
 * 					- ENABLE
 * 					- DISABLE
 * @param[in]	T30IntCfg Interrupt Sourtce Setting
 * @return none
 **********************************************************************/
void T30_IntConfig(TIMER3n_Type* TIMER3n,uint32_t NewState,uint32_t T30IntCfg)
{
	if (NewState == ENABLE) {
		TIMER3n->INTCR |= T30IntCfg;
	}
	else 	{
		TIMER3n->INTCR &= (~T30IntCfg) & 0x7f;
	}
}

/*********************************************************************
 * @brief		Interrupt Flag Clear
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral 					
 * @return none
 **********************************************************************/
uint32_t T30_GetStatus(TIMER3n_Type* TIMER3n)
{
	return TIMER3n->INTFLAG;	
}

/*********************************************************************
 * @brief		Interrupt Flag Clear
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	T30IntCfg Select clear interrupt :
 * 					
 * @return none
 **********************************************************************/
void T30_ClearStatus(TIMER3n_Type* TIMER3n,uint32_t T30IntCfg)
{
	TIMER3n->INTFLAG = T30IntCfg;	
}

/*********************************************************************
* @brief		T30 PWM Mode Setting (Initial : Back to Back Mode, Internal Clock, 6channel Mode) 
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	updatedata PWM Duty Update Timing
 * @param[in]	intcount	  Period Match Interrupt Occurence Seletion  
 * @return none
 **********************************************************************/
void T30_MPWMCmd(TIMER3n_Type* TIMER3n, uint32_t updatedata, uint32_t intcount)
{
	TIMER3n->CR = 0
	| T30_CLKINT
	| T30_BTOB
	| T30_6CHMOD
	| updatedata
	| intcount
	| 1 				//Clear T30 & Precaler
	;
}

/*********************************************************************
 * @brief		Enable or Disable PWM start
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	NewState New State of T30 peripheral's operation, should be:
 * 					- ENABLE
 * 					- DISABLE
 * @return none
 **********************************************************************/
void T30_Start(TIMER3n_Type* TIMER3n, uint32_t NewState)
{
	if (NewState == T30_ENABLE) {
		TIMER3n->CR |= T30_ENABLE;
	}
	else 	{
		TIMER3n->CR &= ~T30_ENABLE;
	}
}

/*********************************************************************
* @brief		PWM Output Port Control Register (Initial : 6channel enable, output low)
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	NewState New State of MPWMx peripheral's operation, should be:
 * 					- ENABLE
 * 					- DISABLE
 * @param[in]	pwmApol	Timer 30xA Output Polarity Selection
 *
 * @param[in]	pwmBpol	Timer 30xB Output Polarity Selection
 * 				
 * @return none
 **********************************************************************/
void T30_OutputCtrl(TIMER3n_Type* TIMER3n,  uint32_t NewState, uint32_t pwmApol, uint32_t pwmBpol)
{
	uint32_t temp;
	temp = 0
	| (0xE06CuL<<TIMER30_OUTCR_WTIDKY_Pos) 
	| (pwmApol | pwmBpol)
	;	
	if(NewState == ENABLE){
		temp |= (0x3f<<8);
	}
	
	TIMER3n->OUTCR = temp;
}

/********************************************************************
 * @brief 		Set Prescaler data .
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	prescale			Timer30 Prescaler Value								
 * @return 	None
 *********************************************************************/
void T30_ClockPrescaler(TIMER3n_Type* TIMER3n, uint32_t prescale)
{
	TIMER3n->PREDR = (prescale&0x0fff);  // period. it sould be larger than 0x10		
}


/********************************************************************
 * @brief 		Set period data .
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	period			MPWM period data									
 * @return 	None
 *********************************************************************/
void T30_SetPeriod(TIMER3n_Type* TIMER3n, uint32_t period)
{
	TIMER3n->PDR = (period&0xffff);  // period. it sould be larger than 0x10		
}

/********************************************************************
 * @brief 		Set duty A data .
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	aduty			Timer30 Aduty data	 							
 * @return 	None
 *********************************************************************/
void T30_SetADuty(TIMER3n_Type* TIMER3n, uint32_t aduty)
{
	TIMER3n->ADR = ((aduty) &0xffff);  // if using I/O control function, set period data	
}

/********************************************************************
 * @brief 		Set duty B data .
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	bduty			Timer30 Bduty data	 							
 * @return 	None
 *********************************************************************/
void T30_SetBDuty(TIMER3n_Type* TIMER3n, uint32_t bduty)
{
	TIMER3n->BDR = ((bduty) &0xffff);  // if using I/O control function, set period data	
}

/********************************************************************
 * @brief 		Set duty C data .
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	cduty			Timer30 Cduty data	 							
 * @return 	None
 *********************************************************************/
void T30_SetCDuty(TIMER3n_Type* TIMER3n, uint32_t cduty)
{
	TIMER3n->CDR = ((cduty) &0xffff);  // if using I/O control function, set period data	
}

/********************************************************************
 * @brief 		Set dead time (delay time)
 * @param[in]	TIMER30	Timer 30 peripheral selected, should be:
 * 					- TIMER30	:Timer30 peripheral
 * @param[in]	dten			dead time enable 
 * @param[in]	dtpos		dead timer position (front or back) 
 * @param[in]	clkdata		dead time 							
 * @return 	None
 *********************************************************************/
void T30_SetDelayTime(TIMER3n_Type* TIMER3n, uint32_t dten, uint32_t dtpos, uint32_t clkdata)
{	
	uint32_t temp;
	
	temp = TIMER30->CR;
	temp &= ~(0x03<<8);
	temp |= dten | dtpos;	
 	TIMER3n->CR = temp;
	
	TIMER3n->DLY =	clkdata&0x03ff;                  
}

/*----------------------------------------------------------------------------
   @brief        Set Timer 30 T30HIZCR Register
   @param[in]    u32T30HizSet 			Timer 30 Output High-Impedance Setting Data 
 * @return       None
 *
 * @explain      This function sets the Timer 30 HIZ/ADT Control Register
 *----------------------------------------------------------------------------*/
void T30_SetHizReg(TIMER3n_Type* TIMER3n, uint32_t u32T30HizSet)
{
	TIMER3n->HIZCR = u32T30HizSet;  //Setting Timer 30 High-Impedance Control Register
}

/*----------------------------------------------------------------------------
   @brief        Set Timer30 ADC Tirgger Source & Timing 
   @param[in]    u32triggerpoint 		Timer 30 Output High-Impedance Setting Data 
   @param[in]    u32triggertime 		Timer 30 A/DC Trigger Setting Data 
 * @return       None
 *
 * @explain      This function sets the Timer 30 HIZ/ADT Control Register
 *----------------------------------------------------------------------------*/
void T30_SetADCTrigger(TIMER3n_Type* TIMER3n, uint32_t u32triggerpoint, uint32_t u32triggertime)
{
	TIMER3n->ADTCR = u32triggerpoint;  //Setting Timer 30 A/DC Trigger Control Register
	TIMER3n->ADTDR = u32triggertime;
}
