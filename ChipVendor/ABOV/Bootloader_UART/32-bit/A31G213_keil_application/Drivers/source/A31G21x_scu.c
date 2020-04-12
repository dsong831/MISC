/**********************************************************************
* @file		A31G21x_scu.c
* @brief	Contains all functions support for SCU firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2017, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "A31G21x_scu.h"

/**********************************************************************
 * @brief 		Configure external sub clock  
 *             Before exteranl crystal enable setting, you have to set clock ports as SXIN,SXOUT in MOD,AFSRx reg
 * @param[in]	esosccon 	Function mode, should be:
 * 					- EXSOSC_STOP	   : 0
 * 					- EXSOSC_EN    	   : 8
 * 					- EXSOSC_EN_DIV2 : 9
  * 					- EXSOSC_EN_DIV2 : 10 
 * @return		None
 **********************************************************************/
void SCU_SetSOSC(uint32_t esosccon)
{
	uint32_t reg_val32;
	
// enable external sub clock  
// Before exteranl crystal enable setting, you have to set clock ports as SXIN,SXOUT in MOD,AFSRx reg
	reg_val32=SCU->CSCR;
	reg_val32&=~(0x0f<<12); // 
	reg_val32|=(esosccon<<12); // 0:Stop SOSC, 8:Enable SOSC/1, 9:Enable SOSC/2, 10:Enable SOSC/4
	SCU->CSCR=reg_val32 | (0xa507UL<<16);		
}
 
/**********************************************************************
 * @brief 	  monitoring External sub oscillator  
 * @param  none
 * @return	status enumeration value:
 *              - ERROR
 *              - SUCCESS
 **********************************************************************/
Status SCU_WaitForSOSCStartUp(void)
{
	__IO uint32_t StartUpCounter = 0;
	__IO uint16_t status;
	uint32_t i;
		
	// monitoring External sub oscillator 
	SCU->CMR|=(1<<11); // 0:External sub oscillator monitoring disabled, 1:External sub oscillator monitoring enable
	/* Wait till SOSC is ready and if timeout is reached exit */
//	do 
//	{
//		status = (SCU->CMR & SCU_CMR_SOSCSTS_Msk);
//		StartUpCounter++;  
//	} while((StartUpCounter != SOSC_STARTUP_TIMEOUT) && (status != SCU_CMR_SOSCSTS_Msk));	
	
	/* need more Wait for SOSC stable, check SXOUT pin oscillation  */	
	for (i=0;i<0x100;i++)
	{
          NOP();	
	}
	
//	status = (SCU->CMR & SCU_CMR_SOSCSTS_Msk);
//	if (status != SCU_CMR_SOSCSTS_Msk){
//		return ERROR;
//	}
//	else {
		return SUCCESS;
//	}
}

/**********************************************************************
 * @brief 		Configure low speed internal osc clock  
 * @param[in]	ringosc Function mode, should be:
 * 					- LSIOSC_STOP        : 0
 * 					- LSIOSC_EN             : 8
 * 					- LSIOSC_EN_DIV2     : 9
 * 					- LSIOSC_EN_DIV4     : 10
 * @return		None
 **********************************************************************/
void SCU_SetLSIOSC(uint32_t ringosc)
{
	uint32_t reg_val32;
	
// enable internal clock  
	reg_val32=SCU->CSCR;
	reg_val32&=~(0x0f<<8); // 
	reg_val32|=(ringosc<<8); // 0:Stop LSI, 8:Enable LSI/1, 9:Enable LSI/2, 10:Enable LSI/4
	SCU->CSCR=reg_val32 | (0xa507UL<<16);		
}

/**********************************************************************
 * @brief 		Configure high speed internal osc clock  
 * @param[in]	hsiosc Function mode, should be:
 * 					- HSIOSC_STOP        : 0
 * 					- HSIOSC_EN             : 8
 * 					- HSIOSC_EN_DIV2     : 9
 * 					- HSIOSC_EN_DIV4     : 10
 * 					- HSIOSC_EN_DIV8     : 11
 * 					- HSIOSC_EN_DIV16    : 12
 * 					- HSIOSC_EN_DIV32    : 13
 * @return		None
 **********************************************************************/
void SCU_SetHSIOSC(uint32_t hsiosc)
{
	uint32_t reg_val32;
	
// enable internal clock  
	reg_val32=SCU->CSCR;
	reg_val32&=~(0x0f<<4); // 
	reg_val32|=(hsiosc<<4); // 0:Stop LSI, 8:Enable LSI/1, 9:Enable LSI/2, 10:Enable LSI/4, 11:Enable LSI/8, 12:Enable LSI/16, 13:Enable LSI/32
	SCU->CSCR=reg_val32 | (0xa507UL<<16);		
}

/**********************************************************************
 * @brief 		Configure external clock  
 *             Before exteranl crystal enable setting, you have to set clock ports as XIN,XOUT in MR.
 * @param[in]	eosccon 	Function mode, should be:
 * 					- EXOSC_STOP        : 0
 * 					- EXOSC_EN             : 8
 * 					- EXOSC_EN_DIV2     : 9
 * 					- EXOSC_EN_DIV4     : 10
 * @return		None
 **********************************************************************/
void SCU_SetMOSC(uint32_t eosccon)
{
	uint32_t reg_val32;
	
// enable external clock  
// Before exteranl crystal enable setting, you have to set clock ports as XIN,XOUT in MOD,AFSRx reg
	reg_val32=SCU->CSCR;
	reg_val32&=~(0x0f<<0); // 
	reg_val32|=(eosccon<<0); // 0:Stop MOSC, 8:Enable MOSC/1, 9:Enable MOSC/2, 10:Enable MOSC/4
	SCU->CSCR=reg_val32 | (0xa507UL<<16);	
}

/**********************************************************************
 * @brief 	  monitoring External oscillator  
 * @param  none
 * @return	status enumeration value:
 *              - ERROR
 *              - SUCCESS
 **********************************************************************/
Status SCU_WaitForMOSCStartUp(void)
{
	__IO uint32_t StartUpCounter = 0;
	__IO uint16_t status;
	uint32_t i;

	// monitoring External oscillator 
	SCU->CMR|=(1<<3); // 0:External oscillator monitoring disabled, 1:External oscillator monitoring enable
	
	/* Wait till MOSC is ready and if timeout is reached exit */
	do 
	{
		status = (SCU->CMR & SCU_CMR_MOSCSTS_Msk);
		StartUpCounter++;  
	} while((StartUpCounter != MOSC_STARTUP_TIMEOUT) && (status != SCU_CMR_MOSCSTS_Msk));	
	
	/* need more Wait for MOSC stable, check XOUT pin oscillation  */	
	for (i=0;i<0x800;i++)
	{

           __NOP();
		
	}
	
	status = (SCU->CMR & SCU_CMR_MOSCSTS_Msk);
	if (status != SCU_CMR_MOSCSTS_Msk){
		return ERROR;
	}
	else {
		return SUCCESS;
	}
}

/**********************************************************************
 * @brief 		change system clock control   
 * @param[in]	sysclkcon mode, should be:
 * 					- SC_LSIOSC		         : 0
 * 					- SC_SOSC  	               : 1 
 * 					- SC_HSIOSC  	               : 2
 * 					- SC_HSIOSCPLL            : 3
 * 					- SC_EXOSC                   : 6
 * 					- SC_EXOSCPLL             : 7
 * @return		None
 **********************************************************************/
void SCU_ChangeSysClk(uint32_t sysclkcon)
{ 
	uint32_t reg_val32;
	
	reg_val32=SCU->SCCR;
	reg_val32&=~(7<<0); //
	reg_val32|=sysclkcon;  
	SCU->SCCR=reg_val32 | (0x570aUL<<16);		
}


/**********************************************************************
 * @brief 		Configure clock out  on PF4
 *       Before this function setting, you have to set PF4 as CLKO in PF.MODand PF.AFSR0.
 * @param[in]	divval(=CLKODIV) : 0~15  // CLKO=MCLK/(2*(CLKODIV+1))
 *                   ex) MCLK=20Mh, CLKODIV=4,    20Mhz/ (2*(4+1)) = 2Mhz
 * @param[in]	FunctionalState, should be:
 * 					- DISABLE		   : 0
 * 					- ENABLE    	   : 1
 * @return		None
 **********************************************************************/
void SCU_SetCOR(uint8_t divval, FunctionalState endis)
{
	SCU->COR=(endis<<4)|(divval&0xf);      
}

/**********************************************************************
 * @brief 	  monitoring External oscillator  
 * @param  none
 * @return	status enumeration value:
 *              - ERROR
 *              - SUCCESS
 **********************************************************************/
Status SCU_SetPLLandWaitForPLLStartUp(FunctionalState pllsetstate, uint8_t selbypass, uint8_t selfin,  uint8_t PREDIV, uint8_t POSTDIV1, uint8_t POSTDIV2, uint8_t OUTDIV)
{
	__IO uint32_t StartUpCounter = 0;
	__IO uint32_t status;
//	uint32_t i;
	
	if (pllsetstate == DISABLE){
		SCU->PLLCON=0;
		return SUCCESS;
	}
	else {
// PLLenable    PLL freq = (FIN or FIN/2) * M / N
		SCU->PLLCON=0
		|(1<<23)   // PLL reset is negated
		|(1<<22)  // PLLEn		
		|((selbypass&1)<<21)     //BYPASS 0:FOUT is bypassed as FIN, 1:FOUT is PLL output
		|((selfin&1)<<20)        //PLL VCO mode  0:VCO is the same with FOUT, 1:VCO frequency is x2 of FOUT   D
		|((PREDIV&0x7)<<16)      //PREDIV        R
		|((POSTDIV1&0xff)<<8)    //POSTDIV1   N1			
		|((POSTDIV2&0xf)<<4)     //POSTDIV2   N2
		|((OUTDIV&0xf)<<0)       //OUTDIV	     P 
			;

//		SCU->PLLCON&=~(1<<23);   // PLL reset is asserted
//		for (i=0;i<1000;i++);
//		SCU->PLLCON|=(1<<23);     // PLL reset is negated
		
	  /* Wait till PLL LOCK is ready and if timeout is reached exit */
		do
		{
			status = (SCU->PLLCON & (1UL<<31));
			StartUpCounter++;  
		} while((StartUpCounter != PLL_STARTUP_TIMEOUT) && (status != (1UL<<31)));	
		
		status = (SCU->PLLCON & (1UL<<31));
		if (status != (1UL<<31)){
			return ERROR;
		}
		else {
			return SUCCESS;
		}
	}
}

/*----------------------------------------------------------------------------
   @brief        Set Timer1n Clock
   @param[in]    t1nclk                           T1NCLK_MCCR1, T1NCLK_PCLK
   @explain      This macro sets timer1n clock
 *----------------------------------------------------------------------------*/
void SCU_SetTimer1nClk(uint32_t t1nclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<22);
	temp |= (t1nclk<<22);
	SCU->PPCLKSR = temp;	      
}

/*----------------------------------------------------------------------------
   @brief        Set Timer20 Clock
   @param[in]    t20clk                           T20CLK_MCCR2, T20CLK_PCLK
   @explain      This macro sets timer20 clock
 *----------------------------------------------------------------------------*/
void SCU_SetTimer20Clk(uint32_t t20clk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<20);
	temp |= (t20clk<<20);
	SCU->PPCLKSR = temp;	      
}

/*----------------------------------------------------------------------------
   @brief        Set Timer30 Clock
   @param[in]    t30clk                           T30CLK_MCCR2, T30CLK_PCLK
   @explain      This macro sets timer30 clock
 *----------------------------------------------------------------------------*/
void SCU_SetTimer30Clk(uint32_t t30clk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<17);
	temp |= (t30clk<<17);
	SCU->PPCLKSR = temp;	      
}

/*----------------------------------------------------------------------------
   @brief        Set LED Clock
   @param[in]    ledclk                           LEDCLK_MCCR5, LEDCLK_PCLK
   @explain      This macro sets LED clock
 *----------------------------------------------------------------------------*/
void SCU_SetLedClk(uint32_t ledclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<10);
	temp |= (ledclk<<10);
	SCU->PPCLKSR = temp;	      
}

/*----------------------------------------------------------------------------
   @brief        Set USB Clock
   @param[in]    usbclk                           USBCLK_MCCR6, USBCLK_PCLK
   @explain      This macro sets USB clock
 *----------------------------------------------------------------------------*/
void SCU_SetUSBClk(uint32_t usbclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<9);
	temp |= (usbclk<<9);
	SCU->PPCLKSR = temp;	      
}

/*----------------------------------------------------------------------------
   @brief        Set LCD Timer Clock
   @param[in]    lcdclk                           LCDCLK_MCCR5, LCDCLK_SOSC, LCDCLK_WDTRC
   @explain      This macro sets watch timer clock
 *----------------------------------------------------------------------------*/
void SCU_SetLcdClk(uint32_t lcdclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(3<<6);
	temp |= (lcdclk<<6);
	SCU->PPCLKSR = temp;	      
}


/*----------------------------------------------------------------------------
   @brief        Set Watch Timer Clock
   @param[in]    wtclk                           WTCLK_MCCR3, WTCLK_SOSC, WTCLK_WDTRC
   @explain      This macro sets watch timer clock
 *----------------------------------------------------------------------------*/
void SCU_SetWtClk(uint32_t wtclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(3<<3);
	temp |= (wtclk<<3);
	SCU->PPCLKSR = temp;	      
}

/*----------------------------------------------------------------------------
   @brief        Set WatchDog Timer Clock
   @param[in]    wdtclk                           WDTCLK_WDTRC, WDTCLK_MCCR3
   @explain      This macro sets watchdog timer clock
 *----------------------------------------------------------------------------*/
void SCU_SetWdtClk(uint32_t wdtclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<0);
	temp |= (wdtclk<<0);
	SCU->PPCLKSR = temp;	      
}

/**********************************************************************
 * @brief 		Configure peri clock setting
 * @param[in]	mccrnum: select REG number
 *                     - MCCR1 : 1, * - MCCR2 : 2
 *                     - MCCR3 : 3, * - MCCR4 : 4
 *                     - MCCR5 : 5, * - MCCR6 : 6 
 * @param[in]	type : 
 *	                     SYSTICK_TYPE, TIMER10_TYPE, 
 *	                     TIMER20_TYPE, TIMER30_TYPE,
 *						 WDT_TYPE, WT_TYPE, 
 *						 PD0_TYPE, PD1_TYPE,
 *						 LED_TYPE, LCD_TYPE,
 *						 USB_TYPE
 * @param[in]	clksrc : 
 *						- CLKSRC_LSI=0,
 *						- CLKSRC_SOSC=3,
 *						- CLKSRC_MCLK=4,	
 *						- CLKSRC_HSI=5,
 *						- CLKSRC_MOSC=6,
 *                     - CLKSRC_PLL=7	
 * @param[in]	clkdiv : 
 * @return		None
 **********************************************************************/
void SCU_SetMCCRx(uint8_t mccrnum, uint8_t type, uint8_t clksrc, uint8_t clkdiv)
{
	uint32_t tmp;
	
	switch(mccrnum){
		case 1:
			tmp=SCU->MCCR1;
			if (type==SYSTICK_TYPE){
				tmp&=0xFFFFF800;
				tmp|= (((clksrc&7)<<8)|((clkdiv&0xFF)<<0));		
			}
			else if (type==TIMER10_TYPE){
				tmp&=0xF800FFFF;
				tmp|= (((clksrc&7)<<24)|((clkdiv&0xFF)<<16));					
			}			
			SCU->MCCR1=tmp;
			break;
			
		case 2:	
			tmp=SCU->MCCR2;
			if (type==TIMER20_TYPE){
				tmp&=0xFFFFF800;
				tmp|= (((clksrc&7)<<8)|((clkdiv&0xFF)<<0));		
			}
			else if (type==TIMER30_TYPE){
				tmp&=0xF800FFFF;
				tmp|= (((clksrc&7)<<24)|((clkdiv&0xFF)<<16));					
			}
			SCU->MCCR2=tmp;			
			break;
			
		case 3:
			tmp=SCU->MCCR3;
			if (type==WDT_TYPE){
				tmp&=0xFFFFF800;
				tmp|= (((clksrc&7)<<8)|((clkdiv&0xFF)<<0));		
			}
			else if (type==WT_TYPE){
				tmp&=0xF800FFFF;
				tmp|= (((clksrc&7)<<24)|((clkdiv&0xFF)<<16));					
			}
			SCU->MCCR3=tmp;			
			break;
			
		case 4:
			tmp=SCU->MCCR4;
			if (type==PD0_TYPE){
				tmp&=0xFFFFF800;
				tmp|= (((clksrc&7)<<8)|((clkdiv&0xFF)<<0));		
			}
			else if (type==PD1_TYPE){
				tmp&=0xF800FFFF;
				tmp|= (((clksrc&7)<<24)|((clkdiv&0xFF)<<16));					
			}
			SCU->MCCR4=tmp;			
			break;
			
		case 5:
			tmp=SCU->MCCR5;
			if (type==LED_TYPE){
				tmp&=0xFFFFF800;
				tmp|= (((clksrc&7)<<8)|((clkdiv&0xFF)<<0));		
			}
			else if (type==LCD_TYPE){
				tmp&=0xF800FFFF;
				tmp|= (((clksrc&7)<<24)|((clkdiv&0xFF)<<16));					
			}
			SCU->MCCR5=tmp;			
			break;		

		case 6:
			tmp=SCU->MCCR6;
			if (type==LCD_TYPE){
				tmp&=0xF800FFFF;
				tmp|= (((clksrc&7)<<24)|((clkdiv&0x1)<<16));					
			}
			SCU->MCCR6=tmp;			
			break;				
	}
}

/**********************************************************************
 * @brief 		set LVD level   
 * @param[in]	bodlevel, should be:
 * 					- LVD_LEVEL_1_65 : 0
 * 					- LVD_LEVEL_2_5 : 1
 * 					- LVD_LEVEL_3_6 : 2
 * 					- LVD_LEVEL_4_2 : 3
 * @return		None
 **********************************************************************/
void SCU_SetLVDLevel(uint32_t lvdlevel)
{
////	uint32_t tmp;
	
////	tmp=SCU->LVDCON;
////	tmp&=~(3<<8); //
////	tmp|=(1<<15) | (lvdlevel <<8); 
////	SCU->LVDCON=tmp;	
}

/**********************************************************************
 * @brief	 	enable/disable LVD
 * @param[in]	NewState
 * 					- ENABLE  	:Set LVD enable
 * 					- DISABLE 	:Disable LVD
 * @return 		None
 **********************************************************************/
void SCU_LVDCmd(FunctionalState NewState)
{
		
	if (NewState == ENABLE)
	{
		SCULV->LVRCR = 0x00;
		SCULV->LVRCNFIG = ((0x72A5<<16) | (0x00<<8));
		
	}
	else
	{
		SCULV->LVRCR = 0x55;
		SCULV->LVRCNFIG = ((0x72A5<<16) | (0xAA<<8));
	}
}

uint32_t SCULV_GetLviFlag(void)
{
	return (SCULV->LVICR & (1<<4));	
}

void SCULV_ClrLviFlag(void)
{
	SCULV->LVICR |= (1<<4);
}

/**********************************************************************
 * @brief	 	enable/disable WakeUp Source
 * @param[in]	WakeUpSrc, oring is possible:
 * 					- WAKEUP_GPIOD :    (1UL<<11)
 * 					- WAKEUP_GPIOC :    (1UL<<10)
 * 					- WAKEUP_GPIOB :    (1UL<<9)
 * 					- WAKEUP_GPIOA :   (1UL<<8)
*						- WAKEUP_FRT     :   (1UL<<2)
 * 					- WAKEUP_WDT    :    (1UL<<1)
 * 					- WAKEUP_LVD    :    (1UL<<0)
 * @param[in]	NewState
 * 					- ENABLE  	:Set WakeUp Source enable
 * 					- DISABLE 	:Disable WakeUp Source
 * @return 		None
 **********************************************************************/
void SCU_WakeUpSRCCmd(uint32_t WakeUpSrc, FunctionalState NewState)
{
	if (NewState == ENABLE)
	{
		SCU->WUER |=  (WakeUpSrc);
	}
	else
	{
		SCU->WUER &= ~(WakeUpSrc);
	}
}

/*********************************************************************
 * @brief 		Get current value of WakeUpSRC Status.
 * @param[in]	None.
 * @return		Current value of WakeUpSRC Status register.
 *********************************************************************/
uint32_t SCU_GetWakeUpSRCStatus(void)
{
	return (SCU->WUSR);
}

/**********************************************************************
 * @brief	 	enable/disable WakeUp Source
 * @param[in]	WakeUpSrc
 * @return 		None
 **********************************************************************/
void SCU_ClearWakeUpSRC(uint32_t WakeUpStatus)
{
	SCU->WUSR = WakeUpStatus;
}

/*********************************************************************
 * @brief 		Get current value of NMI Status.
 * @param[in]	None.
 * @return		Current value of NMI Status register.
 *********************************************************************/
uint32_t SCU_GetNMIStatus(void)
{
	return (SCU->NMIR);
}

/**********************************************************************
 * @brief	 	enable/disable NMI Source
 * @param[in]	nmisrc
 * @return 		None
 **********************************************************************/
void SCU_SetNMI(uint32_t nmisrc)
{
	SCU->NMIR = (0xA32CUL<<16) | (nmisrc & 0x1F);
}


/**********************************************************************
 * @brief	 	enable/disable Reset Source
 * @param[in]	RstSrc, oring is possible:
 * 					- RST_PINRST :    (1UL<<11)
 * 					- RST_CPURST :    (1UL<<10)
 * 					- RST_SWRST :    (1UL<<9)
 * 					- RST_WDTRST :   (1UL<<8)
 *						- RST_MCKFRST     :   (1UL<<2)
 * 					- RST_MOFRST    :    (1UL<<1)
 * 					- RST_LVDRST    :    (1UL<<0)
 * @param[in]	NewState
 * 					- ENABLE  	:Set Reset Source enable
 * 					- DISABLE 	:Disable Reset Source
 * @return 		None
 **********************************************************************/
void SCU_SetResetSrc(uint32_t RstSrc, FunctionalState NewState)
{
	if (NewState == ENABLE)
	{
		SCU->RSER |=  (RstSrc);
	}
	else
	{
		SCU->RSER &= ~(RstSrc);
	}
}

/*********************************************************************
 * @brief 		Get current value of RSSR Status.
 * @param[in]	None.
 * @return		Current value of RSSR Status register.
 *********************************************************************/
uint32_t SCU_GetResetSrc(void)
{
	return (SCU->RSSR);
}

/**********************************************************************
 * @brief	 	clear Reset Status
 * @param[in]	rststatus
 * @return 		None
 **********************************************************************/
void SCU_ClearResetStatus(uint32_t rststatus)
{
	SCU->RSSR = rststatus;
}


/* --------------------------------- End Of File ------------------------------ */
