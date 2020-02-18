#include "A31G21x_pcu.h"
#include "A31G21x_scu.h"
#include "A31G21x_uartn.h"
#include "A31G21x_usart1n.h"
#include "defboot.h"
#include "flash.h"

/**********************************************************************
// Main Clock Selection
 **********************************************************************/
//-------------------------
//#define USED_CLKO
//-------------------------
#define USED_HSI
//#define USED_LSI
//#define USED_MOSC
//#define USED_SOSC
//#define USED_MOSCPLL
//#define USED_HSIPLL


//#define USED_UART   // 

// Assembly jump code
extern void _fjump(void);

void BootMain(void);

// system default I/O setting
void PCU_Init(void);
void SCU_ClockInit(void);

DEV_INFO wrt;

int main(void)
{
	__disable_irq();
	
	BootMain();
	
	return (0);
}

void DelayMS(uint32_t t)			// Delay Time 
{
	uint32_t i;
	
	do
	{
		for (i=0; i<4000; i++); // 
	} while(t--);
}

#ifdef USED_UART
void uputch(uint8_t ch)
{
	while ((UART1->LSR & UART_LSR_TEMT) == 0)		
	{
		
	}

	UART1->THR = ch;
}

uint8_t ugetch(void)
{
	uint8_t ch;
	
	while ((UART1->LSR & UART_LSR_RDR)==0)
	{
		
	}
	ch =  UART1->RBR;
	
	return ch;
}

uint8_t ugetchar(uint8_t *ch)
{
	if( (UART1->LSR & UART_LSR_RDR) == UART_LSR_RDR)
	{
		*ch = UART1->RBR;
		return 1;
	}
	*ch=0;

	return 0;
}
#else // USART10
void uputch(uint8_t ch)
{
	while(!(USART10->ST & USART_SR_DRE)){ }
	USART10->DR = ch;
}

uint8_t ugetch(void)
{
	uint8_t ch;

	while(!(USART10->ST & USART_SR_RXC)){ }	
	ch =  USART10->DR;
	
	return ch;
}

uint8_t ugetchar(uint8_t *ch)
{
	if( (USART10->ST & USART_SR_RXC) == USART_SR_RXC)
	{
		*ch = USART10->DR;
		return 1;
	}
	*ch=0;

	return 0;
}
#endif

uint32_t PageAddress;
uint32_t RealPageAddress;

void GetPageNumber(void)
{
	uint8_t PageAddressHigh;
	uint8_t PageAddressLow;
	
	PageAddressHigh = ugetch();
	PageAddressLow = ugetch();

	RealPageAddress = ((uint16_t)PageAddressHigh)*0x100;
	RealPageAddress += PageAddressLow;

	PageAddress = ((uint32_t)RealPageAddress) << NSHIFTPAGE;
	
	uputch('!');
}

/*****************************************************************************/
char WriteFlashPage(void)
{
	uint16_t i;
	uint8_t CheckSum;
	uint8_t tt;
	
	uint8_t rxdata[PageByte];

	CheckSum = 0;
	
	for (i=0;i<PageByte;i++)
	{ 
		rxdata[i] = ugetch();
	}	
	tt=ugetch();

	for (i=0;i<PageByte;i++)
	{ 
		CheckSum += rxdata[i];
	}
	
	if (CheckSum != tt)
	{
		return 0;
	}

	if (PageAddress<0x1000) // 0x0000~0x1000 is bootcode area, so we do not erase and write 
		return 1;
		
	if (flash_erase_sector(PageAddress))
		return 0; //fail
	if (flash_write_sector(PageAddress,(unsigned long *)rxdata))
		return 0; //fail		
	
	return 1;
}

void BootMain(void)
{
	uint8_t ch;
	uint8_t cnt;
#ifdef USED_UART
	UART_CFG_Type UARTConfigStruct;
#else
	USART_CFG_Type USARTConfigStruct;	
#endif
	
	PCU_Init(); // all port control setting  !!You have to set PORT function!!
	SCU_ClockInit();
	

#ifdef USED_UART
	UART_ConfigStructInit(&UARTConfigStruct); //38400-8-N-1	
	UART_Init(UART1,&UARTConfigStruct);
#else 
	USART_UART_Mode_ConfigStructInit(&USARTConfigStruct);	
	USART_Init(USART10, &USARTConfigStruct);
  USART_Enable(USART10, ENABLE);
#endif

	uputch('>'); //start protocol
	for (cnt=0;cnt<100;cnt++)
	{
		ugetchar(&ch);
		if (ch=='<') break;

		DelayMS(1); // about 1msec		
	}
	
	if (cnt<100) // program update start
	{
		wrt.pagesize=PageSize;
		wrt.deviceid.word=DeviceID;
		wrt.flashsize=FlashSize;
		wrt.bootsize=BootSize;	
		
		uputch(wrt.pagesize);
		uputch(wrt.deviceid.byte[0]);
		uputch(wrt.deviceid.byte[1]);
		uputch(wrt.flashsize);
		uputch(wrt.bootsize);
		
		uputch('!');

		while (1)
		{
			GetPageNumber();

			if (RealPageAddress==(uint32_t)(0xffff)) 
			{
				__nop();
				goto EXIT;
			}

			if (WriteFlashPage())
			{
				uputch('!');
			}
			else 
			{
				uputch('@');
			}
		}
		
EXIT:
		;										
	}
	
	_fjump();										// jump to application	
}

/**********************************************************************
 * @brief		SCU_ClockInit
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SCU_ClockInit(void)
{
	uint32_t i;


/*  LSI Clock Setting  500kHz */	
#ifdef USED_LSI			
	SCU_SetLSIOSC(LSIOSC_EN_DIV2); //LSIOSC_EN. LSIOSC_EN_DIV2, LSIOSC_EN_DIV4
	SystemCoreClock=500000; 	//500khz
	SystemPeriClock=500000; 	//500khz	
	
	for (i=0;i<10;i++);	
	
	SCU_ChangeSysClk(SC_LSIOSC);
#endif 

/*  SOSC Clock Setting  32.768khz */	
#ifdef USED_SOSC  
	SCU_SetSOSC(EXSOSC_EN);
	SystemCoreClock=32768; //32.768khz
	SystemPeriClock=32768; //32.768khz	

	for (i=0;i<100;i++);	
	
	SCU_WaitForSOSCStartUp();
	SCU_ChangeSysClk(SC_SOSC);
#endif 	

/*  HSI Clock Setting  32kHz */
#ifdef USED_HSI		
	SCU_SetHSIOSC(HSIOSC_EN);
	SystemCoreClock=32000000uL; //32MHz
	SystemPeriClock=32000000uL; //32MHz	
	
	for (i=0;i<10;i++);	

	SCU_ChangeSysClk(SC_HSIOSC);
#endif 	

/*  HSE Clock Setting  8Mhz */
#ifdef USED_MOSC	
	SCU_SetMOSC(EXOSC_EN);
	SystemCoreClock=16000000uL; //8MHz
	SystemPeriClock=16000000uL; //8MHz	

	for (i=0;i<100;i++);	

	SCU_WaitForMOSCStartUp();
	SCU_ChangeSysClk(SC_EXOSC);
#endif


/*  HSE PLL Clock Setting  48Mhz */
#ifdef USED_MOSCPLL

	if (SCU_SetPLLandWaitForPLLStartUp(ENABLE,  
		PLLCON_BYPASS_PLL,    								//PLLCON_BYPASS_FIN:0, PLLCON_BYPASS_PLL:1
		0,                                   	//0:FOUT==VCO, 1:FOUT==2xVCO,  D=0
		1,                                    //PREDIV, R=1
		47,                                  	//POSTDIV1, N1=47  
		1,                                    //POSTDIV2, N2=1
		0)==ERROR)                    				//OUTDIV P=0
	{
		while(1);
	}


	SCU_ChangeSysClk(SC_EXOSCPLL); //EOSC -->  EOSCPLL
	
	SystemCoreClock=48000000uL; 	//48MHz
	SystemPeriClock=48000000uL;  	//48MHz
#endif




/*  HSI PLL Clock Setting  48Mhz */	
#ifdef USED_HSIPLL
/**
************************************************************************************************
* PLL setting 
* FIN=PLLINCLK/(R+1)                                 			// R: Pre Divider   
* FOUT=(FIN*(N1+1)*(D+1))  / ((N2+1)*(P+1))          			// N1: Post Divider1,N2:Post Div2, P:Out Div,      
*             = FVCO *(D+1)                         		 	// D:Frequency Doubler
*
*ex)    FIN=PLLINCLK/(R+1) = 8M/(3+1) = 2M            		// R:3, PLLINCLK:8MHz(MOSC)
*         FOUT=(2M*(47+1)*(0+1)) / ((1+1)*(0+1) = 48MHz   // N1:47, D:0, N2:1, P:0
*
************************************************************************************************
*/
	if (SCU_SetPLLandWaitForPLLStartUp(ENABLE,  
		PLLCON_BYPASS_PLL,    //PLLCON_BYPASS_FIN:0, PLLCON_BYPASS_PLL:1
		0,                    //0:FOUT==VCO, 1:FOUT==2xVCO,  D=0
		1,                    //PREDIV, R=1
		2,                   //POSTDIV1, N1=47  
		0,                    //POSTDIV2, N2=1
		0)==ERROR)            //OUTDIV P=0
	{
		while(1);
	}
	SCU_ChangeSysClk(SC_HSIOSCPLL); //EOSC -->  EOSCPLL
	
	SystemCoreClock=48000000uL; 	//48MHz 
	SystemPeriClock=48000000uL; 	//48MHz
#endif

/* mclk monitoring enable */
	SCU->CMR&=~(1<<7); //mclk monitoring disable
	SCU->CMR|=(1<<7); 

	
/* flash memory controller */
	FMC->MR = 0x81;       							// after changing 0x81 -> 0x28 in MR reg, flash access timing will be able to be set.
	FMC->MR = 0x28;       							// enter flash access timing changing mode
	//FMC->CFG = (0x7858<<16) | (3<<8);  	//WAIT is 011, flash access in 4 cycles (3-wait) 		
	FMC->CFG = (0x7858<<16) | (2<<8);  	//WAIT is 011, flash access in 4 cycles (3-wait) 		
	FMC->MR = 0;	      								// exit flash access timing --> normal mode				
	
/*	CLKO function setting. check PORT setting (PF4) */
#ifdef USED_CLKO
	SCU_SetCOR(4,ENABLE); //ENABLE
#else
	SCU_SetCOR(4,DISABLE);  //DISABLE
#endif
}


/**********************************************************************
 * @brief		PCU_Init
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void PCU_Init(void)
{
//Peripheral Enable Register 1  0:Disable, 1:Enable	
	SCU->PER1=SCU->PER1  
	| (1<<13)   // GPIOF
	| (1<<12)   // GPIOE
	| (1<<11)   // GPIOD
	| (1<<10)   // GPIOC
	| (1<<9)  	// GPIOB
	| (1<<8)   	// GPIOA
		; 		
//Peripheral Clock Enable Register 1 0:Disable, 1:Enable	
	SCU->PCER1=SCU->PCER1
	| (1<<13)   // GPIOF
	| (1<<12)   // GPIOE
	| (1<<11)   // GPIOD
	| (1<<10)   // GPIOC
	| (1<<9)   	// GPIOB
	| (1<<8)   	// GPIOA
		; 	

 	PORT_ACCESS_EN();  // enable writing permittion of ALL PCU register

	//--------------------------------------------------------------
	//	PORT INIT
	//	PA
	//--------------------------------------------------------------
	// PORT - A
	PA->MOD = 0              // 0 : Input Mode,	1 : Output Mode,	2 : Alternative function mode
	| (0x01<<14)             // P7
	| (0x01<<12)             // P6
	| (0x01<<10)             // P5
	| (0x01<<8)              // P4
	| (0x01<<6)              // P3
	| (0x01<<4)              // P2
	| (0x01<<2)              // P1
	| (0x01<<0)              // P0
	;
	
	PA->TYP = 0              // 0 : Push-pull Output,	1 : Open-drain Output
	| (0x00<<7)              // P7
	| (0x00<<6)              // P6
	| (0x00<<5)              // P5
	| (0x00<<4)              // P4
	| (0x00<<3)              // P3
	| (0x00<<2)              // P2
	| (0x00<<1)              // P1
	| (0x00<<0)              // P0
	;
	
	PA->AFSR1 = 0
	| (0x00<<28)              // P7	  0 : 			,	1 :T13O 		, 2 :	T13C	, 3 : AN7/DAO			, 4 : CS2/COM10	
	| (0x00<<24)              // P6		0 : 			,	1 :T11O 		, 2 :	T11C	, 3 : AN6					, 4 : CS1/COM11
	| (0x00<<20)              // P5		0 :				,	1 :T12O	  	, 2 : T12C	, 3 : AN5					, 4 : CS0/COM12
	| (0x00<<16)              // P4		0 :				,	1 : 				, 2 :				, 3 : AN4					, 4 :	CS7/COM13
	| (0x00<<12)              // P3		0 : 			,	1 : 				, 2 :				, 3 : AN3					, 4 :	CS6/COM14
	| (0x00<<8)               // P2		0 : 			,	1 : EC12		, 2 :				, 3 : AN2/AVREF		, 4 :	CS5/COM15
	| (0x00<<4)               // P1		0 : 			,	1 : SCL1		, 2 :				, 3 : AN1					, 4 :	CS4
	| (0x00<<0)               // P0		0 : 			,	1 : SDA1		, 2 :				, 3 : AN0					, 4 :	CS3
	;

	
	PA->PUPD = 0            	// 0 : Disable Pull-up/down,	1 : Enable Pull-up,	2 : Enable Pull-down
	| (0x0<<14)             	// P7
	| (0x0<<12)             	// P6
	| (0x0<<10)             	// P5
	| (0x0<<8)              	// P4
	| (0x0<<6)              	// P3
	| (0x0<<4)              	// P2
	| (0x0<<2)              	// P1
	| (0x0<<0)              	// P0
	;
	
	PA->OUTDR = 0            	// 0 : Output Low,	1 : Output High
	| (0x00<<7)              	// P7
	| (0x00<<6)              	// P6
	| (0x00<<5)              	// P5
	| (0x00<<4)              	// P4
	| (0x00<<3)              	// P3
	| (0x00<<2)              	// P2
	| (0x00<<1)              	// P1
	| (0x00<<0)              	// P0
	;

	//--------------------------------------------------------------
	//	PORT INIT
	//	PB
	//--------------------------------------------------------------
	PB->MOD = 0              // 0 : Input Mode,	1 : Output Mode,	2 : Alternative function mode
	| (0x01<<14)             // P7
	| (0x01<<12)             // P6
	| (0x02<<10)             // P5   SWDIO
	| (0x02<<8)              // P4   SWCLK
	| (0x01<<6)              // P3
	| (0x01<<4)              // P2
	| (0x02<<2)              // P1
	| (0x02<<0)              // P0
	;
	
	PB->TYP = 0              // 0 : Push-pull Output,	1 : Open-drain Output
	| (0x00<<7)              // P7
	| (0x00<<6)              // P6
	| (0x00<<5)              // P5
	| (0x00<<4)              // P4
	| (0x00<<3)              // P3
	| (0x00<<2)              // P2
	| (0x00<<1)              // P1
	| (0x00<<0)              // P0
	;
	
	PB->AFSR1 = 0
	| (0x01<<28)              // P7		0 :	 	,	1 : RXD1	, 2 : 				, 3 : AN12			, 4 : CS18
	| (0x01<<24)              // P6		0 : 	,	1 : TXD1	, 2 :	EC11		, 3 :	AN11			, 4 : CS17
	| (0x02<<20)              // P5		0 : 	,	1 : RXD0	, 2 : SWDIO		, 3 :						, 4 :  
	| (0x02<<16)              // P4		0 : 	,	1 : TXD0	, 2 : SWCLK		, 3 :						, 4 :  
	| (0x01<<12)              // P3		0 :	 	,	1 : BOOT	, 2 : SS10		, 3 :						, 4 : 
	| (0x00<<8)               // P2		0 : 	,	1 : EC13	, 2 : SCK10		, 3 : AN10			, 4 : CS10/COM7
	| (0x01<<4)               // P1		0 : 	,	1 : RXD10	, 2 : MISO10	, 3 : AN9				, 4 : CS9/COM8
	| (0x01<<0)               // P0		0 : 	,	1 : TXD10	, 2 : MOSI10	, 3 : AN8				, 4 : CS8/COM9
	;

	
	PB->PUPD = 0              // 0 : Disable Pull-up/down,	1 : Enable Pull-up,	2 : Enable Pull-down
	| (0x0<<14)              	// P7
	| (0x0<<12)              	// P6
	| (0x0<<10)              	// P5
	| (0x0<<8)              	// P4
	| (0x0<<6)              	// P3
	| (0x0<<4)              	// P2
	| (0x1<<2)              	// P1
	| (0x0<<0)              	// P0
	;
	
	PB->OUTDR = 0             // 0 : Output Low,	1 : Output High	
	| (0x00<<7)              	// P7
	| (0x00<<6)              	// P6
	| (0x00<<5)              	// P5
	| (0x00<<4)              	// P4
	| (0x00<<3)              	// P3
	| (0x00<<2)              	// P2
	| (0x00<<1)              	// P1
	| (0x00<<0)              	// P0
	;

	//--------------------------------------------------------------
	//	PORT INIT
	//	PC
	//--------------------------------------------------------------
	PC->MOD = 0              // 0 : Input Mode,	1 : Output Mode,	2 : Alternative function mode
	| (0x02<<10)             // P5
	| (0x01<<8)              // P4
	| (0x01<<6)              // P3
	| (0x01<<4)              // P2
	| (0x01<<2)              // P1
	| (0x01<<0)              // P0
	;
	
	PC->TYP = 0              // 0 : Push-pull Output,	1 : Open-drain Output
	| (0x00<<5)              // P5
	| (0x00<<4)              // P4
	| (0x00<<3)              // P3
	| (0x00<<2)              // P2
	| (0x00<<1)              // P1
	| (0x00<<0)              // P0
	;
	
	PC->AFSR1 = 0			
	| (0x0<<20)              // P5		0 : nRESET	, 1 : 				, 2 :					, 3 :   		, 4 :				
	| (0x0<<16)              // P4		0 : 				, 1 :					, 2 :	SCK20		, 3 :  			, 4 : CS23/COM2  
	| (0x0<<12)              // P3		0 : 				, 1 : EC21		, 2 :	MISO20	, 3 :  			, 4 : CS22/COM3  
	| (0x0<<8)               // P2		0 : 				, 1 : EC20		, 2 :	MOSI20	, 3 :  			, 4 : CS21/COM4  
	| (0x0<<4)               // P1		0 : 				, 1 : T21O		, 2 : T21C		, 3 :  			, 4 : CS20/COM5  
	| (0x0<<0)               // P0		0 : 				, 1 : T20O		, 2 : T20C		, 3 :  AN13	, 4 : CS19/COM6  
	;

	
	PC->PUPD = 0             // 0 : Disable Pull-up/down,	1 : Enable Pull-up,	2 : Enable Pull-down
	| (0x0<<10)              // P5
	| (0x0<<8)               // P4
	| (0x0<<6)               // P3
	| (0x0<<4)               // P2
	| (0x0<<2)               // P1
	| (0x0<<0)               // P0
	;
	
	PC->OUTDR = 0           	// 0 : Output Low,	1 : Output High
	| (0x0<<5)              	// P5
	| (0x0<<4)              	// P4
	| (0x0<<3)              	// P3
	| (0x0<<2)              	// P2
	| (0x0<<1)              	// P1
	| (0x0<<0)              	// P0
	;

	//--------------------------------------------------------------
	//	PORT INIT
	//	PD
	//--------------------------------------------------------------
	PD->MOD = 0              // 0 : Input Mode,	1 : Output Mode,	2 : Alternative function mode
	| (0x01<<10)             // P5
	| (0x01<<8)              // P4
	| (0x01<<6)              // P3
	| (0x01<<4)              // P2
	| (0x01<<2)              // P1
	| (0x01<<0)              // P0
	;
	
	PD->TYP = 0              // 0 : Push-pull Output,	1 : Open-drain Output
	| (0x00<<5)              // P5
	| (0x00<<4)              // P4
	| (0x00<<3)              // P3
	| (0x00<<2)              // P2
	| (0x00<<1)              // P1
	| (0x00<<0)              // P0
	;
	
	PD->AFSR1 = 0
	| (0x00<<20)              // P5		0 : 	,	1 : 			, 2 : SS11			, 3 : 		, 4 : SEG6				
	| (0x00<<16)              // P4		0 : 	,	1 :	BLNK	, 2 : SCK11			, 3 : 		, 4 : SEG7
	| (0x00<<12)              // P3		0 : 	,	1 : RXD11	, 2 : MISO11		, 3 : 		, 4 : SEG8
	| (0x00<<8)               // P2		0 : 	,	1 : TXD11	, 2 : MOSI11		, 3 : 		, 4 : SEG9			
	| (0x00<<4)               // P1		0 : 	,	1 : SDA0	, 2 :	EC10			, 3 : 		, 4 : COM0			 
	| (0x00<<0)               // P0		0 : 	,	1 : SCL0	, 2 :	SS20			, 3 : 		, 4 : COM1			 
	;
	
	PD->PUPD = 0           		// 0 : Disable Pull-up/down,	1 : Enable Pull-up,	2 : Enable Pull-down
	| (0x0<<10)             	// P5
	| (0x0<<8)              	// P4
	| (0x0<<6)              	// P3
	| (0x0<<4)              	// P2
	| (0x0<<2)              	// P1
	| (0x0<<0)              	// P0
	;
	
	PD->OUTDR = 0             // 0 : Output Low,	1 : Output High
	| (0x00<<5)              	// P5
	| (0x00<<4)              	// P4
	| (0x00<<3)              	// P3
	| (0x00<<2)              	// P2
	| (0x00<<1)              	// P1
	| (0x00<<0)              	// P0
	;

	//--------------------------------------------------------------
	//	PORT INIT
	//	PE
	//--------------------------------------------------------------
	PE->MOD = 0              // 0 : Input Mode,	1 : Output Mode,	2 : Alternative function mode
	| (0x01<<14)             // P7
	| (0x01<<12)             // P6
	| (0x01<<10)             // P5
	| (0x01<<8)              // P4
	| (0x01<<6)              // P3
	| (0x01<<4)              // P2
	| (0x01<<2)              // P1
	| (0x01<<0)              // P0
	;
	
	PE->TYP = 0              // 0 : Push-pull Output,	1 : Open-drain Output
	| (0x00<<7)              // P7
	| (0x00<<6)              // P6
	| (0x00<<5)              // P5
	| (0x00<<4)              // P4
	| (0x00<<3)              // P3
	| (0x00<<2)              // P2
	| (0x00<<1)              // P1
	| (0x00<<0)              // P0
	;
	
	PE->AFSR1 = 0
	| (0x00<<28)             // P7		0 : 		, 1 : T11O			, 2 : T11C		, 3 :		, 4 : CS11/SEG5
	| (0x00<<24)             // P6		0 : 		, 1 : T10O			, 2 : T10C		, 3 :		, 4 : CS12/SEG4
	| (0x00<<20)             // P5		0 : 		, 1 : PWM30CB		, 2 :	MOSI21	, 3 :		, 4 : CS13/SEG3
	| (0x00<<16)             // P4		0 : 		, 1 : PWM30CA		, 2 :	MISO21	, 3 :		, 4 : CS14/SEG2	
	| (0x00<<12)             // P3		0 : 		, 1 : PWM30BB		, 2 :	SCK21		, 3 :		, 4 : CS15/SEG1	
	| (0x00<<8)              // P2		0 : 		, 1 : PWM30BA		, 2 :	SS21		, 3 :		, 4 : CS16/SEG0
	| (0x00<<4)              // P1		0 : 		, 1 : PWM30AB		, 2 :					, 3 :		, 4 : 	
	| (0x00<<0)              // P0		0 : 		, 1 : PWM30AA		, 2 :	SS11		, 3 :		, 4 : 
	;

	
	PE->PUPD = 0            // 0 : Disable Pull-up/down,	1 : Enable Pull-up,	2 : Enable Pull-down
	| (0x0<<14)             // P7
	| (0x0<<12)             // P6
	| (0x0<<10)             // P5
	| (0x0<<8)              // P4
	| (0x0<<6)              // P3
	| (0x0<<4)              // P2
	| (0x0<<2)              // P1
	| (0x0<<0)              // P0
	;
	
	PE->OUTDR = 0           // 0 : Output Low,	1 : Output High
	| (0x0<<7)              // P7
	| (0x0<<6)              // P6
	| (0x0<<5)              // P5
	| (0x0<<4)              // P4
	| (0x0<<3)              // P3
	| (0x0<<2)              // P2
	| (0x0<<1)              // P1
	| (0x0<<0)              // P0
	;
	
	//--------------------------------------------------------------
	//	PORT INIT
	//	PF
	//--------------------------------------------------------------
	PF->MOD = 0              // 0 : Input Mode,	1 : Output Mode,	2 : Alternative function mode
	| (0x01<<14)             // P7
	| (0x01<<12)             // P6
	| (0x01<<10)             // P5
	| (0x02<<8)              // P4     clko
	| (0x02<<6)              // P3
	| (0x02<<4)              // P2
	| (0x02<<2)              // P1
	| (0x02<<0)              // P0
	;
	
	PF->TYP = 0							// 0 : Push-pull Output,	1 : Open-drain Output
	| (0x0<<7)              // P7
	| (0x0<<6)              // P6
	| (0x0<<5)              // P5
	| (0x0<<4)              // P4
	| (0x0<<3)              // P3
	| (0x0<<2)              // P2
	| (0x0<<1)              // P1
	| (0x0<<0)              // P0
	;
	
	PF->AFSR1 = 0
	| (0x0<<28)              // P7		0 :				, 1 : T30C		, 2 : SDA0		, 3 :					, 4 :				 
	| (0x0<<24)              // P6		0 :				, 1 : EC30		, 2 : SCL0		, 3 :					, 4 :				 
	| (0x0<<20)              // P5		0 :				, 1 : BLNK		, 2 : 			, 3 :					, 4 :				 
	| (0x1<<16)              // P4		0 :				, 1 : CLKO		, 2 :					, 3 : 				, 4 :				 
	| (0x3<<12)              // P3		0 :				, 1 : RXD1		, 2 :	T30C		, 3 : SXOUT		, 4 :				 
	| (0x3<<8)               // P2		0 :				, 1 : TXD1		, 2 :	EC30		, 3 : SXIN		, 4 :				 
	| (0x3<<4)               // P1		0 :				, 1 : SDA1		, 2 :					, 3 : XIN			, 4 :				 
	| (0x3<<0)               // P0		0 :				, 1 : SCL1		, 2 :					, 3 : XOUT		, 4 : 	 
	;

	PF->PUPD = 0             // 0 : Disable Pull-up/down,	1 : Enable Pull-up,	2 : Enable Pull-down
	| (0x0<<14)              // P7
	| (0x0<<12)              // P6
	| (0x0<<10)              // P5
	| (0x0<<8)               // P4
	| (0x0<<6)               // P3
	| (0x0<<4)               // P2
	| (0x0<<2)               // P1
	| (0x0<<0)               // P0
	;
	
	PF->OUTDR = 0              // 0 : Output Low,	1 : Output High	
	| (0x00<<7)              // P7
	| (0x00<<6)              // P6
	| (0x00<<5)              // P5
	| (0x00<<4)              // P4
	| (0x00<<3)              // P3
	| (0x00<<2)              // P2
	| (0x00<<1)              // P1
	| (0x00<<0)              // P0
	;
	 /* disable writing permittion of ALL PCU register */ 
	//	PORT_ACCESS_DIS(); 

}

