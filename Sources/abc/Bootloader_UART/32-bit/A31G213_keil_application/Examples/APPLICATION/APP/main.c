#include "A31G21x_pcu.h"
#include "A31G21x_scu.h"


//-------------------------
#define USED_CLKO
//-------------------------
#define USED_HSI
//#define USED_LSI
//#define USED_MOSC
//#define USED_SOSC
//#define USED_MOSCPLL
//#define USED_HSIPLL

volatile uint32_t msec;

void PCU_Init(void);
void SCU_ClockInit(void);

/**********************************************************************
 * @brief		SysTick handler sub-routine (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Handler (void) 					// SysTick Interrupt Handler @ 1000Hz
{
	if(msec)msec--;
}

void mainloop(void)
{
	uint32_t i;
	
	// Test Pin setting PE0
	PCU_SetDirection(PE, 0, PUSH_PULL_OUTPUT);
	PCU_ConfigurePullupdown(PE, 0, DISPUPD);	
	GPIO_SetValue(PE, _BIT(0));

	// Test Pin setting PE1
	PCU_SetDirection(PE, 1, PUSH_PULL_OUTPUT);
	PCU_ConfigurePullupdown(PE, 1, DISPUPD);
	GPIO_SetValue(PE, _BIT(1));

	// Test Pin setting PE2
	PCU_SetDirection(PE, 2, PUSH_PULL_OUTPUT);
	PCU_ConfigurePullupdown(PE, 2, DISPUPD);
	GPIO_SetValue(PE, _BIT(2));

	// Test Pin setting PE3
	PCU_SetDirection(PE, 3, PUSH_PULL_OUTPUT);
	PCU_ConfigurePullupdown(PE, 3, DISPUPD);	
	GPIO_SetValue(PE, _BIT(3));

	// Test Pin setting PE4
	PCU_SetDirection(PE, 4, PUSH_PULL_OUTPUT);
	PCU_ConfigurePullupdown(PE, 4, DISPUPD);
	GPIO_SetValue(PE, _BIT(4));

	// Test Pin setting PE5
	PCU_SetDirection(PE, 5, PUSH_PULL_OUTPUT);
	PCU_ConfigurePullupdown(PE, 5, DISPUPD);
	GPIO_SetValue(PE, _BIT(5));


	SysTick_Config(SystemCoreClock/1000);  //1msec interrupt 	

	__enable_irq();	
	
	while(1){
	#if 1
		msec = 100;
		while(msec);
		
		for(i=0;i<6;i++){
			msec = 50;
			while(msec);
			GPIO_ClearValue(PE,_BIT(i));
		}
		
		for(i=0;i<6;i++){
			msec = 50;
			while(msec);
			GPIO_SetValue(PE,_BIT(i));
		}
	#else
		msec = 200;
		while(msec);
		GPIO_SetValue(PE,_BIT(0));
		GPIO_SetValue(PE,_BIT(1));
		
		msec = 200;
		while(msec);
		GPIO_ClearValue(PE,_BIT(0));
		GPIO_SetValue(PE,_BIT(1));
		
		msec = 200;
		while(msec);
		GPIO_SetValue(PE,_BIT(0));
		GPIO_ClearValue(PE,_BIT(1));
		
		msec = 200;
		while(msec);
		GPIO_ClearValue(PE,_BIT(0));
		GPIO_ClearValue(PE,_BIT(1));
	#endif
		
	}
}

int main(void)
{
  __disable_irq();	
	
	SCB->VTOR = 0x00001000; //Vector Table Offset Register
	
	PCU_Init(); // all port control setting  !!You have to set PORT function!!
	SCU_ClockInit(); 

	mainloop();
	
	return (0);
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
	| (0x0<<2)              	// P1
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

