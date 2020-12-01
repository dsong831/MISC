/**********************************************************************
* @file		test_fmc_A31T21x.c
* @brief	Contains all macro definitions and function prototypes
* 			support for A31T21x firmware library
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/
#include "main_conf.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static unsigned char gau8Buffer[512];		// For OTP Write
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_fmc_main(void);
static void Flash_OTP_EraseWrite_RUN(unsigned char otp_area);
static void OTP_ARRAY_SET(void);
static void WaitTime_Test(void);
static void FMC_Self_Test(void);
static void FMCTest(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_fmc_main(void)
{
	
}

/**********************************************************************
 * @brief		Test Function
* @param[in]	otp_area (1: User_OTP / 2: PROT)
 * @return 	None
 **********************************************************************/
static void Flash_OTP_EraseWrite_RUN(unsigned char otp_area)
{
	// For set test mode
	OTP_ARRAY_SET();

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
	
	// Erase Process
	__disable_irq();
	if(otp_area == 1)
	{
		FMC->MR = 0x5A;
		FMC->MR = 0xA5;
		FMCR_IFEN_SET;
		FMCR_SECT4K_CLR;
		FMCR_SECT1K_CLR;
	}
	else if(otp_area == 2)
	{
		FMC->MR = 0x5A;
		FMC->MR = 0xA5;
		FMCR_LOCKSEL_SET;
		FMCR_SECT4K_CLR;
		FMCR_SECT1K_CLR;
	}
	ErasePage(0);	// Erase
	FMC->MR = 0;
	__enable_irq();

	// Write Process
	__disable_irq();
	if(otp_area == 1)
	{
		FMC->MR = 0x5A;
		FMC->MR = 0xA5;
		FMCR_IFEN_SET;
		FMCR_SECT4K_CLR;
		FMCR_SECT1K_CLR;
	}
	else if(otp_area == 2)
	{
		FMC->MR = 0x5A;
		FMC->MR = 0xA5;
		FMCR_LOCKSEL_SET;
		FMCR_SECT4K_CLR;
		FMCR_SECT1K_CLR;
	}
	ProgramPage(0, 512, gau8Buffer);	// Write
	FMC->MR = 0;
	__enable_irq();

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
}

/**********************************************************************
 * @brief		Test Function (For BootROM Timing Check)
* @param[in]	None
 * @return 	None
 **********************************************************************/
static void OTP_ARRAY_SET(void)
{
	unsigned int i, j = 0;

#if 1		// Best Time
	unsigned int au32Buffer[128] = {0x55555555, 0xAAAAAAAA,
		0x000000FF,	//origin : 0x000000FF,	
		0xEA2A4000,												// Base Address 0x4000_0000
		0xED730104, 0x00000081,		// AMBA mode
		0xED1A0104, 0x00000028,		// AMBA mode
		0xEDF00130, 0x78580200,		// 2-wait (3-cycle)

		0xED250028, 0x0000010F,		// SCU->PER1 = 0x10F (GPIOA Peri Enable)
		0xED2D0030, 0x0000010F,		// SCU->PCER1 = 0x10F (GPIOA Clock Enable)

		0xED111FF0, 0x00000015, 	// PORTEN ENABLE
		0xED4D1FF0, 0x00000051,		// PORTEN ENABLE

		0xED051008, 0x00000000,		// PA->AFSR1 = 0 (PA2 GPIO)
		0xEDD91000, 0xFFFFFFDF,	// PA->MOD = 0xFFFFFFDF (PA2 Output)
		0xED051004, 0x00000000,		// PA->TYP = 0 (PA2 Push-Pull Output)
		0xED0D1010, 0x00000000,		// PA->PUPD = 0 (PA2 PUPD Disable)
		0xED1D101C, 0x00000004,	// PA->BSR = (1<<2) (PA2 OUTPUT HIGH)	//	[27]

		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
#else		// Worst Time
	unsigned int au32Buffer[128] = {0x55555555, 0xAAAAAAAA,
		0x000000FF,	
		0xEA2A4000,												// Base Address 0x4000_0000
		0xED730104, 0x00000081,		// AMBA mode
		0xED1A0104, 0x00000028,		// AMBA mode
		0xEDF00130, 0x78580200,		// 2-wait (3-cycle)
		
		0xED250028, 0x0000010F,		// SCU->PER1 = 0x10F (GPIOA Peri Enable)
		0xED2D0030, 0x0000010F,		// SCU->PCER1 = 0x10F (GPIOA Clock Enable)
		
		0xED111FF0, 0x00000015, 	// PORTEN ENABLE
		0xED4D1FF0, 0x00000051,		// PORTEN ENABLE
		
		0xED051008, 0x00000000,		// PA->AFSR1 = 0 (PA2/3 GPIO)
		0xED591000, 0xFFFFFF5F,		// PA->MOD = 0xFFFFFF5F (PA2/3 Output)
		0xED051004, 0x00000000,		// PA->TYP = 0 (PA2/3 Push-Pull Output)
		0xED0D1010, 0x00000000,		// PA->PUPD = 0 (PA2/3 PUPD Disable)
		0xED25101C, 0x0000000C,	// PA->BSR = (1<<2) (PA2/3 OUTPUT HIGH)	// [27]

		// PA3 Toggle
		0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008,
		0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008,
		0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008,
		0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008,
		0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008,
		0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008,
		0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008,
		0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008,
		0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008,
		0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008, 0xED251020, 0x00000008, 0xED21101C, 0x00000008};
#endif

	for(i=0; i<512; i+=4)
	{
		gau8Buffer[i+0] = (unsigned char)(au32Buffer[j] >> 0);
		gau8Buffer[i+1] = (unsigned char)(au32Buffer[j] >> 8);
		gau8Buffer[i+2] = (unsigned char)(au32Buffer[j] >> 16);
		gau8Buffer[i+3] = (unsigned char)(au32Buffer[j] >> 24);
		j++;
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void WaitTime_Test(void)
{
	/* Set Flach Access Time */
	FMC->MR = 0x81;
	FMC->MR = 0x28;
	FMC->CFG = (0x7858 << 16) | (3 << 8);		// Flash Access in 4 cycles (3-wait)
	FMC->MR = 0;
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void FMC_Self_Test(void)
{
	unsigned int i;
	unsigned char au8Buffer[512];
	
	for(i=0; i<512; i++)
	{
		au8Buffer[i] = 0x5A;
	}

#if 1	// Page Erase
	__disable_irq();
	SelfErasePage(0x10000);
	__enable_irq();
#else	// Sector Erase
	__disable_irq();
	EraseSector4KB(0x1F000);
	__enable_irq();
#endif


	__disable_irq();
	SelfProgramPage(0x10000, 512, au8Buffer);
	__enable_irq();

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void FMCTest(void)
{
	unsigned int i;
	unsigned char au8Buffer[512];
	
	for(i=0; i<512; i++)
	{
		au8Buffer[i] = 0x5A;
	}

#if 1	// Page Erase
	__disable_irq();
	ErasePage(0xF000);
	__enable_irq();
#else	// Sector Erase
	__disable_irq();
	EraseSector4KB(0x1F000);
	__enable_irq();
#endif


	__disable_irq();
	ProgramPage(0xF000, 512, au8Buffer);
	__enable_irq();

}

