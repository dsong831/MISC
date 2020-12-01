/**********************************************************************//**
* @file				A31T21x_hal_scu.c
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			26. JUNE. 2020
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
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


/* Includes ------------------------------------------------------------ */
#include "A31T21x_hal_scu.h"


/**********************************************************************//**
 * @brief						Configure low speed internal osc clock  
 * @param[in]	LSICON bit set
 *											- LSIOSC_STOP				: 0
 *											- LSIOSC_EN						: 8
 *											- LSIOSC_EN_DIV2		: 9
 *											- LSIOSC_EN_DIV4		: 10
 * @return				None
 **********************************************************************/
void HAL_SCU_LSI_ClockConfig(uint32_t lsicon)
{
	uint32_t reg32;

	reg32 = SCU->CSCR;
	reg32 &= ~(0x0F << 8);
	reg32 |= (lsicon << 8);		// 0:Stop LSI, 8:Enable LSI/1, 9:Enable LSI/2, 10:Enable LSI/4
	SCU->CSCR = reg32 | (0xA507UL << 16);
}

/**********************************************************************//**
 * @brief						Configure high speed internal osc clock  
 * @param[in]	HSICON bit set
 *											- HSIOSC_STOP				: 0
 *											- HSIOSC_EN							: 8
 *											- HSIOSC_EN_DIV2		: 9
 *											- HSIOSC_EN_DIV4		: 10
 *											- HSIOSC_EN_DIV8		: 11
 *											- HSIOSC_EN_DIV16		: 12
 *											- HSIOSC_EN_DIV32		: 13
 * @return				None
 **********************************************************************/
void HAL_SCU_HSI_ClockConfig(uint32_t hsicon)
{
	uint32_t reg32;

	reg32 = SCU->CSCR;
	reg32 &= ~(0x0F << 4);
	reg32 |= (hsicon <<4);		// 0:Stop LSI, 8:Enable LSI/1, 9:Enable LSI/2, 10:Enable LSI/4, 11:Enable LSI/8, 12:Enable LSI/16, 13:Enable LSI/32
	SCU->CSCR = reg32 | (0xA507UL << 16);
}

/**********************************************************************//**
 * @brief						Configure external sub clock  
 *											Before exteranl crystal enable setting, you have to set clock ports as SXIN,SXOUT in MOD,AFSRx reg
 * @param[in]	LSECON bit set
 *											- EXSOSC_STOP			: 0
 *											- EXSOSC_EN					: 8
 *											- EXSOSC_EN_DIV2	: 9
 *											- EXSOSC_EN_DIV2	: 10 
 * @return		None
 **********************************************************************/
void HAL_SCU_LSE_ClockConfig(uint32_t lsecon)
{
	uint32_t reg32;

	reg32 = SCU->CSCR;
	reg32 &= ~(0x0F << 12);
	reg32 |= (lsecon << 12);		// 0:Stop SOSC, 8:Enable SOSC/1, 9:Enable SOSC/2, 10:Enable SOSC/4
	SCU->CSCR = reg32 | (0xA507UL << 16);		
}
 
/**********************************************************************//**
 * @brief						Configure external clock  
 *											Before exteranl crystal enable setting, you have to set clock ports as XIN,XOUT in MR.
 * @param[in]	HSECON bit set
 *											- EXOSC_STOP			: 0
 *											- EXOSC_EN					: 8
 *											- EXOSC_EN_DIV2	: 9
 *											- EXOSC_EN_DIV4	: 10
 * @return				None
 **********************************************************************/
void HAL_SCU_HSE_ClockConfig(uint32_t hsecon)
{
	uint32_t reg32;

	reg32 = SCU->CSCR;
	reg32 &= ~(0x0F << 0);
	reg32 |= (hsecon << 0);		// 0:Stop MOSC, 8:Enable MOSC/1, 9:Enable MOSC/2, 10:Enable MOSC/4
	SCU->CSCR = reg32 | (0xA507UL<<16);	
}

/**********************************************************************//**
 * @brief 	  Monitoring external sub oscillator  
 * @param	None
 * @return	Status enumeration value
 *								- FAIL
 *								- SUCCESS
 **********************************************************************/
SUCCESS_FAIL_Type HAL_SCU_LSE_ClockMonitoring(void)
{
	__IO uint32_t StartUpCounter = 0;
	__IO uint16_t status;
	uint32_t i;

	SCU->CMR |= (1<<11);	// 0:External sub oscillator monitoring disabled, 1:External sub oscillator monitoring enable
	/* Wait till SOSC is ready and if timeout is reached exit */
	do
	{
		status = (SCU->CMR & (1<<8));
		StartUpCounter++;
	} while((StartUpCounter != SOSC_STARTUP_TIMEOUT) && (status != (1<<8)));

	/* Need more Wait for SOSC stable, check SXOUT pin oscillation  */
	for(i=0; i<0x100; i++)
	{
		__NOP();
	}

	status = (SCU->CMR & (1<<8));
	if(status != (1<<8)){
		return FAIL;
	}
	else{
		return SUCCESS;
	}
}

/**********************************************************************//**
 * @brief			Monitoring External oscillator
 * @param	None
 * @return	Status enumeration value
 *								- FAIL
 *								- SUCCESS
 **********************************************************************/
SUCCESS_FAIL_Type HAL_SCU_HSE_ClockMonitoring(void)
{
	__IO uint32_t StartUpCounter = 0;
	__IO uint16_t status;
	uint32_t i;

	SCU->CMR |= (1 << 3);	// 0:External oscillator monitoring disabled, 1:External oscillator monitoring enable

	/* Wait till MOSC is ready and if timeout is reached exit */
	do
	{
		status = (SCU->CMR & (1<<0));
		StartUpCounter++;
	} while((StartUpCounter != MOSC_STARTUP_TIMEOUT) && (status != (1<<0)));

	/* Need more Wait for MOSC stable, check XOUT pin oscillation  */	
	for(i=0; i<0x800; i++)
	{
		__NOP();
	}

	status = (SCU->CMR & (1<<0));
	if(status != (1<<0)){
		return FAIL;
	}
	else{
		return SUCCESS;
	}
}

/**********************************************************************//**
 * @brief						Clear Clock Source
 * @param[in]	clock_source
 *											- SCU_CLEAR_HSE	: 0
 *											- SCU_CLEAR_HSI		: 4 
 *											- SCU_CLEAR_LSI		: 8
 *											- SCU_CLEAR_LSE		: 12
 * @return				None
 **********************************************************************/
void HAL_SCU_ClearClockSource(uint8_t clock_source)
{
	uint32_t reg32;

	reg32 = SCU->CSCR;
	reg32 &= ~(0x0F << clock_source);
	SCU->CSCR = reg32 | (0xA507UL<<16);	
}

/**********************************************************************//**
 * @brief						Change system clock control   
 * @param[in]	MCLKSEL bit set
 *											- SC_LSIOSC				: 0
 *											- SC_EXSOSC			: 1 
 *											- SC_HSIOSC				: 2
 *											- SC_HSIOSCPLL	: 3
 *											- SC_EXOSC				: 6
 *											- SC_EXOSCPLL	: 7
 * @return				None
 **********************************************************************/
void HAL_SCU_SystemClockChange(uint32_t sysclkcon)
{
	uint32_t reg32;

	reg32 = SCU->SCCR;
	reg32 &= ~(0x07 << 0);
	reg32 |= sysclkcon;
	SCU->SCCR = (0x570AUL << 16) | reg32;
}

/**********************************************************************//**
 * @brief						Configure clock out on PA2
 *											Before this function setting, you have to set PA2 as CLKO in PA.MODand PA.AFSR2.
 * @param[in]	CLKODIV bit set // CLKO = MCLK / (2*(CLKODIV+1))
 * @param[in]	CLKOEN bit set
 *						 					- DISABLE		   : 0
 *											- ENABLE    	   : 1
 * @return				None
 **********************************************************************/
void HAL_SCU_ClockOutConfig(uint8_t clkodiv, EN_DIS_Type clkoen)
{
	SCU->COR = (clkoen << 4) | (clkodiv & 0x0F);
}

/**********************************************************************//**
 * @brief						Configure pll clock  
 * @param[in]	PLL Freq = (FIN*(POSTDIV1+1)) / ((POSTDIV2+1)*(OUTDIV+1)) * (PLLMODE+1)
 * @return				Status enumeration value:
 *											- FAIL
 *											- SUCCESS
 **********************************************************************/
SUCCESS_FAIL_Type HAL_SCU_PLL_ClockConfig(EN_DIS_Type PLLEN, uint8_t BYPASSB, uint8_t PLLMODE, uint8_t PREDIV, uint8_t POSTDIV1, uint8_t POSTDIV2, uint8_t OUTDIV)
{
	__IO uint32_t StartUpCounter = 0;
	__IO uint32_t status;

	if (PLLEN == DISABLE)
	{
		SCU->PLLCON = 0;
		return SUCCESS;
	}
	else
	{
		SCU->PLLCON = 0
		|((BYPASSB&0x1)<<21)	// BYPASSB		(0: FOUT is bypassed as FIN , 1:FOUT is PLL output)
		|((PLLMODE&0x1)<<20)	// PLLMODE		(D) 	// 0: VCO is the same with FOUT , 1: VCO frequency is x2 of FOUT)
		|((PREDIV&0x7)<<16)			// PREDIV			(R)		// FIN = PLLINCLK / (R+1) // Recommanded FIN = 2MHz
		|((POSTDIV1&0xff)<<8)		// POSTDIV1	(N1) // Fvco = FIN * (N1+1) // Fvco <= 200MHz
		|((POSTDIV2&0xf)<<4)		// POSTDIV2	(N2)
		|((OUTDIV&0xf)<<0)				// OUTDIV			(P)
		;
		SCU->PLLCON |= (1<<22);			// PLLEN 				(0: PLL is disabled , 1: PLL is enabled)
		SCU->PLLCON &= ~(1<<23);	// PLLRSTB 	(0: PLL reset is asserted , 1: PLL reset is negated)
		SCU->PLLCON |= (1<<23);

	  /* Wait till PLL LOCK is ready and if timeout is reached exit */
		do
		{
			status = (SCU->PLLCON & (0x01UL<<31));
			StartUpCounter++;  
		} while((StartUpCounter != PLL_STARTUP_TIMEOUT) && (status != (0x01UL<<31)));	

		status = (SCU->PLLCON & (0x01UL<<31));
		if (status != (0x01UL<<31))
		{
			return FAIL;
		}
		else
		{
			return SUCCESS;
		}
	}
}

/**********************************************************************//**
 * @brief						Enable/Disable Reset Source
 * @param[in]	reset_source, oring is possible :
 *											- SCU_RSER_PINRST
 *											- SCU_RSER_CPURST
 *											- SCU_RSER_SWRST
 *											- SCU_RSER_WDTRST
 *											- SCU_RSER_MCKFRST
 *											- SCU_RSER_MOFRST
 *											- SCU_RSER_LVDRST
 * @param[in]	reset_en
 *						 					- DISABLE		   : 0
 *											- ENABLE    	   : 1
 * @return				None
 **********************************************************************/
void HAL_SCU_ResetSourceConfig(uint32_t reset_source, EN_DIS_Type reset_en)
{
	if(reset_en == ENABLE)
	{
		SCU->RSER |= (reset_source);
	}
	else
	{
		SCU->RSER &= ~(reset_source);
	}
}

/**********************************************************************//**
 * @brief						Enable/Disable WakeUp Source
 * @param[in]	wakeup_source, oring is possible :
*											- SCU_WUER_LVDWUE					: 0
*											- SCU_WUER_WDTWUE					: 1
*											- SCU_WUER_WTWUE						: 2
*											- SCU_WUER_USART10WUE	: 3
*											- SCU_WUER_USART11WUE	: 4
*											- SCU_WUER_GPIOAWUE			: 8
*											- SCU_WUER_GPIOBWUE			: 9
*											- SCU_WUER_GPIOCWUE			: 10
*											- SCU_WUER_GPIODWUE			: 11
*											- SCU_WUER_GPIOEWUE			: 12
*											- SCU_WUER_GPIOFWUE			: 13
*											- SCU_WUER_I2C0WUE					: 14
*											- SCU_WUER_I2C1WUE					:	15
 * @param[in]	wakeup_en
 *						 					- DISABLE		   : 0
 *											- ENABLE    	   : 1
 * @return				None
 **********************************************************************/
void HAL_SCU_WakeUpSourceConfig(uint32_t wakeup_source, EN_DIS_Type wakeup_en)
{
	if(wakeup_en == ENABLE)
	{
		SCU->WUER |= (wakeup_source);
	}
	else
	{
		SCU->WUER &= ~(wakeup_source);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

/**********************************************************************//**
   @brief        Set Timer1n Clock
   @param[in]    t1nclk                           T1NCLK_MCCR1, T1NCLK_PCLK
   @explain      This macro sets timer1n clock
 **********************************************************************/
void HAL_SCU_Timer1n_ClockConfig(uint32_t t1nclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<22);
	temp |= (t1nclk<<22);
	SCU->PPCLKSR = temp;	      
}

/**********************************************************************//**
   @brief        Set Timer20 Clock
   @param[in]    t20clk                           TIMER20CLK_MCCR2, TIMER20CLK_PCLK
   @explain      This macro sets timer20 clock
 **********************************************************************/
void HAL_SCU_Timer20_ClockConfig(uint32_t t20clk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<20);
	temp |= (t20clk<<20);
	SCU->PPCLKSR = temp;	      
}

/**********************************************************************//**
   @brief        Set Timer30 Clock
   @param[in]    t30clk                           T30CLK_MCCR2, T30CLK_PCLK
   @explain      This macro sets timer30 clock
 **********************************************************************/
void HAL_SCU_Timer30_ClockConfig(uint32_t t30clk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<17);
	temp |= (t30clk<<17);
	SCU->PPCLKSR = temp;	      
}

/**********************************************************************//**
   @brief        Set LED Clock
   @param[in]    ledclk                           LEDCLK_MCCR5, LEDCLK_PCLK
   @explain      This macro sets LED clock
 **********************************************************************/
void HAL_SCU_LED_ClockConfig(uint32_t ledclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<10);
	temp |= (ledclk<<10);
	SCU->PPCLKSR = temp;	      
}

/**********************************************************************//**
   @brief        Set Watch Timer Clock
   @param[in]    wtclk                           WTCLK_MCCR3, WTCLK_SOSC, WTCLK_WDTRC
   @explain      This macro sets watch timer clock
 **********************************************************************/
void HAL_SCU_WT_ClockConfig(uint32_t wtclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(3<<3);
	temp |= (wtclk<<3);
	SCU->PPCLKSR = temp;	      
}

/**********************************************************************//**
   @brief        Set WatchDog Timer Clock
   @param[in]    wdtclk                           WDTCLK_WDTRC, WDTCLK_MCCR3
   @explain      This macro sets watchdog timer clock
 **********************************************************************/
void HAL_SCU_WDT_ClockConfig(uint32_t wdtclk)
{
	uint32_t temp;
	
	temp = SCU->PPCLKSR;
	temp &= ~(1<<0);
	temp |= (wdtclk<<0);
	SCU->PPCLKSR = temp;	      
}

/**********************************************************************//**
   @brief        Set LCD Clock
   @param[in]    lcdclk                           LCDCLK_MCCR5, LCDCLK_SOSC, LCDCLK_WDTRC
   @explain      This macro sets lcd clock
 **********************************************************************/
void HAL_SCU_LCD_ClockConfig(uint32_t lcdclk)
{
	uint32_t temp;

	temp = SCU->PPCLKSR;
	temp &= ~(1<<6);
	temp |= (lcdclk<<6);
	SCU->PPCLKSR = temp;
}


/* --------------------------------- End Of File ------------------------------ */
