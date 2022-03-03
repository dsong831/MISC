/**********************************************************************
* @file		test_led_A31T21x.c
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
static LED_CFG_Type	LED_Config;
/* Private define ------------------------------------------------------------*/
#define BIT(n)	((unsigned long)1 << (n))
	/*************************
	M-Type LED
*************************/
#define SEG7_A	BIT(8)					// Parameter = SEG Number
#define SEG7_B	BIT(9)
#define SEG7_C	BIT(10)
#define SEG7_D	BIT(11) 
#define SEG7_E	BIT(12)
#define SEG7_F	BIT(13) 
#define SEG7_G	BIT(14)
#define DIGIT_0 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_F)
#define DIGIT_1 (SEG7_B|SEG7_C)
#define DIGIT_2 (SEG7_A|SEG7_B|SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_3 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_G)
#define DIGIT_4 (SEG7_B|SEG7_C|SEG7_F|SEG7_G)
#define DIGIT_5 (SEG7_A|SEG7_C|SEG7_D|SEG7_F|SEG7_G)
#define DIGIT_6 (SEG7_A|SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_7 (SEG7_A|SEG7_B|SEG7_C|SEG7_F)
#define DIGIT_8 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_9 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_F|SEG7_G)
#define DIGIT_A (SEG7_A|SEG7_B|SEG7_C|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_B (SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_C (SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_D (SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_E (SEG7_A|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_F (SEG7_A|SEG7_E|SEG7_F|SEG7_G)
unsigned long const DIGIT_HEX[16] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_B, DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F};
/*************************
	T-Type LED
*************************/
#if 1	// default COM0 to COM7
#define A1	BIT(0)								// Parameter = SEG Number
#define B1	BIT(0)
#define C1	BIT(0)
#define D1	BIT(0)
#define E1	BIT(0)
#define F1	BIT(0)
#define G1	BIT(1)
#define A2	BIT(1)
#define B2	BIT(1)
#define C2	BIT(1)
#define D2	BIT(2)
#define E2	BIT(2)
#define F2	BIT(2)
#define G2	BIT(2)
#define A3	BIT(3)
#define B3	BIT(3)
#define C3	BIT(4)
#define D3	BIT(4)
#define E3	BIT(6)
#define F3	BIT(3)
#define G3	BIT(3)
#define A4	BIT(3)
#define B4	BIT(3)
#define C4	BIT(3)
#define D4	BIT(7)
#define E4	BIT(7)
#define F4	BIT(7)
#define G4	BIT(7)
#else	// for test COM5 to COM12
#define A1	BIT(0+5)								// Parameter = SEG Number
#define B1	BIT(0+5)
#define C1	BIT(0+5)
#define D1	BIT(0+5)
#define E1	BIT(0+5)
#define F1	BIT(0+5)
#define G1	BIT(1+5)
#define A2	BIT(1+5)
#define B2	BIT(1+5)
#define C2	BIT(1+5)
#define D2	BIT(2+5)
#define E2	BIT(2+5)
#define F2	BIT(2+5)
#define G2	BIT(2+5)
#define A3	BIT(3+5)
#define B3	BIT(3+5)
#define C3	BIT(4+5)
#define D3	BIT(4+5)
#define E3	BIT(6+5)
#define F3	BIT(3+5)
#define G3	BIT(3+5)
#define A4	BIT(3+5)
#define B4	BIT(3+5)
#define C4	BIT(3+5)
#define D4	BIT(7+5)
#define E4	BIT(7+5)
#define F4	BIT(7+5)
#define G4	BIT(7+5)
#endif
unsigned long const coT4Digit[4][10][8] = {			// [4-Digit][0~9][COM0~7]
{
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		E1, 	F1},							// 0 : ABCDEF
	{0, 	0,		B1, 	C1, 	0,		0,		0,		0}, 										// 1 : BC
	{0, 	A1, 	B1, 	0,		D1, 	0,		E1, 	G1},								// 2 : ABDEG
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		0,		G1},								// 3 : ABCDG
	{0, 	0,		B1, 	C1, 	0,		0,		0,		F1+G1}, 							// 4 : BCFG
	{0, 	A1, 	0,		C1, 	D1, 	0,		0,		F1+G1}, 						// 5 : ACDFG
	{0, 	A1, 	0,		C1, 	D1, 	0,		E1, 	F1+G1}, 					// 6 : ACDEFG
	{0, 	A1, 	B1, 	C1, 	0,		0,		0,		F1},									// 7 : ABCF
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		E1, 	F1+G1},				// 8 : ABCDEFG
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		0,		F1+G1}						// 9 : ABCDFG
},{
	{0, 	0,		C2, 	B2+D2,	A2+E2,	0,		F2, 	0}, 				// 0 : ABCDEF
	{0, 	0,		C2, 	B2, 	0,		0,		0,		0}, 										// 1 : BC
	{0, 	0,		0,		B2+D2,	A2+E2,	0,		0,		G2},						// 2	: ABDEG
	{0, 	0,		C2, 	B2+D2,	A2, 	0,		0,		G2},							// 3 : ABCDG
	{0, 	0,		C2, 	B2, 	0,		0,		F2, 	G2},									// 4 : BCFG
	{0, 	0,		C2, 	D2, 	A2, 	0,		F2, 	G2},								// 5	: ACDFG
	{0, 	0,		C2, 	D2, 	A2+E2,	0,		F2, 	G2},						// 6 : ACDEFG
	{0, 	0,		C2, 	B2, 	A2, 	0,		F2, 	0},									// 7 : ABCF
//	{0, 	0,		C2, 	B2, 	A2, 	0,		F2, 	G2},								// 7 : ABCF
	{0, 	0,		C2, 	B2+D2,	A2+E2,	0,		F2, 	G2},				// 8 : ABCDEFG
	{0, 	0,		C2, 	B2+D2,	A2, 	0,		F2, 	G2} 						// 9 : ABCDFG
},{
	{0, 	0,		0,		0,		B3, 	0,		A3+C3,	D3+E3+F3},		// 0 : ABCDEF
	{0, 	0,		0,		0,		B3, 	0,		C3, 	0},							 				// 1 : BC
	{0, 	0,		0,		0,		B3, 	G3, 	A3, 	D3+E3}, 						// 2	: ABDEG 
	{0, 	0,		0,		0,		B3, 	G3, 	A3+C3,	D3},							// 3 : ABCDG
	{0, 	0,		0,		0,		B3, 	G3, 	C3, 	F3},									// 4 : BCFG
	{0, 	0,		0,		0,		0,		G3, 	A3+C3,	D3+F3}, 					// 5	: ACDFG
	{0, 	0,		0,		0,		0,		G3, 	A3+C3,	D3+E3+F3},		// 6 : ACDEFG
	{0, 	0,		0,		0,		B3, 	0,		A3+C3,	F3},								// 7 : ABCF
	{0, 	0,		0,		0,		B3, 	G3, 	A3+C3,	D3+E3+F3},	// 8 : ABCDEFG
	{0, 	0,		0,		0,		B3, 	G3, 	A3+C3,	D3+F3}					// 9 : ABCDFG
},{
	{C4,	B4, 	A4+F4,	E4, 	0,		D4, 	0,		0}, 						// 0 : ABCDEF
	{C4,	B4, 		0,	0,		0,		0,		0,		0}, 											// 1 : BC
	{0, 	B4+G4,	A4, 	E4, 	0,		D4, 	0,		0}, 						// 2	: ABDEG
	{C4,	B4+G4,	A4, 	0,		0,		D4, 	0,		0}, 							// 3 : ABCDG
	{C4,	B4+G4,	F4, 	0,		0,		0,		0,		0}, 								// 4 : BCFG
	{C4,	G4, 	A4+F4,	0,		0,		D4, 	0,		0}, 							// 5	: ACDFG
	{C4,	G4, 	A4+F4,	E4, 	0,		D4, 	0,		0}, 						// 6 : ACDEFG
	{C4,	B4, 	A4+F4,	0,		0,		0,		0,		0}, 								// 7 : ABCF
	{C4,	B4+G4,	A4+F4,	E4, 	0,		D4, 	0,		0}, 				// 8 : ABCDEFG
	{C4,	B4+G4,	A4+F4,	0,		0,		D4, 	0,		0}						// 9 : ABCDFG
}};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_led_main(void);
static void LED_Period_Test(void);
static void LED_COM_T_M_Type_BOTH_TEST(void);
static void LED_COM_T_Type_COM5_COM12_Test(void);
static void LED_COM_T_Type_Test(void);
static void LED_COM_M_Type_Test(void);
static void LED_SEG_GND_Test(void);
static void LED_COM_Overlaptime_Test(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_led_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LED_Period_Test(void)
{
	/* LED Port Initialization - ICOM0~ICOM12 & ISEG0~ISEG15 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC5);

	/* LED Config */
	HAL_SCU_LED_ClockConfig(LEDCLK_PCLK);

	LED_Config.tPRESD = 15;							// fLEDCLK = fLEDEXTCLK / (PRESD + 1)	@ fLEDCLK = 1MHz (1us) (at fLEDEXTCLK = 16MHz)
	LED_Config.tCOMPWID = 99;					// COM Width =  (COMPWID + 1) / fLEDCLK @ COM Width = 100us
	LED_Config.tLEDPD = 15999;					// LED Period = (LEDPD + 1) / fLEDEXTCLK @ LED Period = 1ms
	LED_Config.tOVERLAP = 8;						// 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = LED_CLK / 2
	LED_Config.tSR_INTE = DISABLE;
	LED_Config.tMODE = LED_STOPCOUNT_MODE;
	HAL_LED_Init(LED, &LED_Config);

	LED->COMER = 0x00000041;				// COM0 to COM6
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)

	LED->DISPRAM0 = 0x000000FF;		// COM0 [SEG15 to SEG0]
	LED->DISPRAM1 = 0x000000FF;
	LED->DISPRAM2 = 0x000000FF;
	LED->DISPRAM3 = 0x000000FF;
	LED->DISPRAM4 = 0x0000FFFF;
	LED->DISPRAM5 = 0x0000FFFF;
	LED->DISPRAM6 = 0x0000FFFF;
	LED->DISPRAM7 = 0x0000FFFF;
	LED->DISPRAM8 = 0x0000FFFF;
	LED->DISPRAM9 = 0x0000FFFF;
	LED->DISPRAM10 = 0x0000FFFF;
	LED->DISPRAM11 = 0x0000FFFF;
	LED->DISPRAM12 = 0x0000FFFF;	// COM12 [SEG15 to SEG0]

	/* Start LED */
#if 0
	NVIC_EnableIRQ(LED_IRQn);
	NVIC_SetPriority(LED_IRQn, 3);
	__enable_irq();
#endif
	
	HAL_LED_DISP_ON();
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LED_COM_T_M_Type_BOTH_TEST(void)
{
	unsigned int i, j, k;
	unsigned char comIndx;
	unsigned char numIndx;
	unsigned char digitIndx[4];
	volatile unsigned long *ptrDisp;
	
	ptrDisp = (unsigned long*)(&LED->DISPRAM0);
	
	/* LED Port Initialization - ICOM0~ICOM12 & ISEG0~ISEG15 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC5);

	/* LED Config */
	HAL_SCU_LED_ClockConfig(LEDCLK_PCLK);

	LED_Config.tPRESD = 15;							// LED_CLK = fLED / (PRESD+1) @ 15 -> 1MHz (1us)
	LED_Config.tCOMPWID = 99;					// COM Width = LED_CLK / (COMPWID+1) @ 99 -> 100us
	LED_Config.tLEDPD = 1000;					// LED Period = LED_CLK * LEDPD // ## LED Period > COM Width * count of COMER ##
	LED_Config.tOVERLAP = 0;						// 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = LED_CLK / 2
	LED_Config.tSR_INTE = DISABLE;
	LED_Config.tMODE = LED_AUTO_MODE;
	HAL_LED_Init(LED, &LED_Config);

	LED->COMER = 0x00001FFF;				// COM / SEG Select
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)

	LED->DISPRAM0 = 0x00000000;		// COM0 [SEG15 to SEG0]
	LED->DISPRAM1 = 0x00000000;
	LED->DISPRAM2 = 0x00000000;
	LED->DISPRAM3 = 0x00000000;
	LED->DISPRAM4 = 0x00000000;
	LED->DISPRAM5 = 0x00000000;
	LED->DISPRAM6 = 0x00000000;
	LED->DISPRAM7 = 0x00000000;
	LED->DISPRAM8 = 0x00000000;
	LED->DISPRAM9 = 0x00000000;
	LED->DISPRAM10 = 0x00000000;
	LED->DISPRAM11 = 0x00000000;
	LED->DISPRAM12 = 0x00000000;	// COM12 [SEG15 to SEG0]

	/* Start LED */
	HAL_LED_DISP_ON();

	while(1)
	{
		for(i=0; i<10; i++)
		{
			for(j=0; j<500000; j++);
			// Clear LED Data
			for(k=0; k<13; k++)
			{
				ptrDisp[k] = 0;
			}
			// T-Type Sequence Run
			for(numIndx=0; numIndx<4; numIndx++)
			{
				digitIndx[numIndx] = i;														// Number0~9
			}
			for(numIndx=0; numIndx<4; numIndx++)		// 4-DIGIT
			{
				// T-type setting
				for(comIndx=0; comIndx<8; comIndx++)	// COM0~7
				{
					ptrDisp[comIndx] |= coT4Digit[numIndx][digitIndx[numIndx]][comIndx];
				}
				// M-type setting
				LED->DISPRAM0 |= DIGIT_HEX[i];
				LED->DISPRAM1 |= DIGIT_HEX[i];
				LED->DISPRAM2 |= DIGIT_HEX[i];
				LED->DISPRAM3 |= DIGIT_HEX[i];
			}
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LED_COM_T_Type_COM5_COM12_Test(void)
{
	unsigned int i, j, k;
	unsigned char comIndx;
	unsigned char numIndx;
	unsigned char digitIndx[4];
	volatile unsigned long *ptrDisp;
	
	ptrDisp = (unsigned long*)(&LED->DISPRAM5);		// COM5 to COM12
	
	/* LED Port Initialization - ICOM0~ICOM12 & ISEG0~ISEG15 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC5);

	/* LED Config */
	HAL_SCU_LED_ClockConfig(LEDCLK_PCLK);

	LED_Config.tPRESD = 15;							// LED_CLK = fLED / (PRESD+1) @ 15 -> 1MHz (1us)
	LED_Config.tCOMPWID = 99;					// COM Width = LED_CLK / (COMPWID+1) @ 99 -> 100us
	LED_Config.tLEDPD = 1000;					// LED Period = LED_CLK * LEDPD // ## LED Period > COM Width * count of COMER ##
	LED_Config.tOVERLAP = 0;						// 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = LED_CLK / 2
	LED_Config.tSR_INTE = DISABLE;
	LED_Config.tMODE = LED_AUTO_MODE;
	HAL_LED_Init(LED, &LED_Config);

	LED->COMER = 0x00001FFF;				// COM / SEG Select
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)

	LED->DISPRAM5 = 0x00000000;		// COM5 [SEG15 to SEG0]
	LED->DISPRAM6 = 0x00000000;
	LED->DISPRAM7 = 0x00000000;
	LED->DISPRAM8 = 0x00000000;
	LED->DISPRAM9 = 0x00000000;
	LED->DISPRAM10 = 0x00000000;
	LED->DISPRAM11 = 0x00000000;
	LED->DISPRAM12 = 0x00000000;		// COM12 [SEG15 to SEG0]

	/* Start LED */
	HAL_LED_DISP_ON();

	while(1)
	{
		for(i=0; i<10; i++)
		{
			for(j=0; j<500000; j++);
			// Clear LED Data
			for(k=0; k<8; k++)
			{
				ptrDisp[k] = 0;
			}
			// T-Type Sequence Run
			for(numIndx=0; numIndx<4; numIndx++)
			{
				digitIndx[numIndx] = i;														// Number0~9
			}
			for(numIndx=0; numIndx<4; numIndx++)		// 4-DIGIT
			{
				for(comIndx=0; comIndx<8; comIndx++)	// COM0~7
				{
					ptrDisp[comIndx] |= coT4Digit[numIndx][digitIndx[numIndx]][comIndx];
				}
			}
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LED_COM_T_Type_Test(void)
{
	unsigned int i, j, k;
	unsigned char comIndx;
	unsigned char numIndx;
	unsigned char digitIndx[4];
	volatile unsigned long *ptrDisp;
	
	ptrDisp = (unsigned long*)(&LED->DISPRAM0);
	
	/* LED Port Initialization - ICOM0~ICOM12 & ISEG0~ISEG15 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC5);

	/* LED Config */
	HAL_SCU_LED_ClockConfig(LEDCLK_PCLK);

	LED_Config.tPRESD = 15;							// LED_CLK = fLED / (PRESD+1) @ 15 -> 1MHz (1us)
	LED_Config.tCOMPWID = 99;					// COM Width = LED_CLK / (COMPWID+1) @ 99 -> 100us
	LED_Config.tLEDPD = 1000;					// LED Period = LED_CLK * LEDPD // ## LED Period > COM Width * count of COMER ##
	LED_Config.tOVERLAP = 0;						// 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = LED_CLK / 2
	LED_Config.tSR_INTE = DISABLE;
	LED_Config.tMODE = LED_AUTO_MODE;
	HAL_LED_Init(LED, &LED_Config);

	LED->COMER = 0x00001FFF;				// COM / SEG Select
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)

	LED->DISPRAM0 = 0x00000000;		// COM0 [SEG15 to SEG0]
	LED->DISPRAM1 = 0x00000000;
	LED->DISPRAM2 = 0x00000000;
	LED->DISPRAM3 = 0x00000000;
	LED->DISPRAM4 = 0x00000000;
	LED->DISPRAM5 = 0x00000000;
	LED->DISPRAM6 = 0x00000000;
	LED->DISPRAM7 = 0x00000000;		// COM8 [SEG15 to SEG0]

	/* Start LED */
	HAL_LED_DISP_ON();

	while(1)
	{
		for(i=0; i<10; i++)
		{
			for(j=0; j<500000; j++);
			// Clear LED Data
			for(k=0; k<8; k++)
			{
				ptrDisp[k] = 0;
			}
			// T-Type Sequence Run
			for(numIndx=0; numIndx<4; numIndx++)
			{
				digitIndx[numIndx] = i;														// Number0~9
			}
			for(numIndx=0; numIndx<4; numIndx++)		// 4-DIGIT
			{
				for(comIndx=0; comIndx<8; comIndx++)	// COM0~7
				{
					ptrDisp[comIndx] |= coT4Digit[numIndx][digitIndx[numIndx]][comIndx];
				}
			}
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LED_COM_M_Type_Test(void)
{
	unsigned int i, j;
	
	/* LED Port Initialization - ICOM0~ICOM12 & ISEG0~ISEG15 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC5);

	/* LED Config */
	HAL_SCU_LED_ClockConfig(LEDCLK_PCLK);

	LED_Config.tPRESD = 15;							// LED_CLK = fLED / (PRESD+1) @ 15 -> 1MHz (1us)
	LED_Config.tCOMPWID = 99;					// COM Width = LED_CLK / (COMPWID+1) @ 99 -> 100us
	LED_Config.tLEDPD = 1000;					// LED Period = LED_CLK * LEDPD // ## LED Period > COM Width * count of COMER ##
	LED_Config.tOVERLAP = 0;						// 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = LED_CLK / 2
	LED_Config.tSR_INTE = DISABLE;
	LED_Config.tMODE = LED_AUTO_MODE;
	HAL_LED_Init(LED, &LED_Config);

	LED->COMER = 0x0000000F;				// COM / SEG Select
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)

	LED->DISPRAM0 = 0x0000FFFF;		// COM0 [SEG15 to SEG0]
	LED->DISPRAM1 = 0x0000FFFF;
	LED->DISPRAM2 = 0x0000FFFF;
	LED->DISPRAM3 = 0x0000FFFF;
	LED->DISPRAM4 = 0x0000FFFF;
	LED->DISPRAM5 = 0x0000FFFF;
	LED->DISPRAM6 = 0x0000FFFF;
	LED->DISPRAM7 = 0x0000FFFF;
	LED->DISPRAM8 = 0x0000FFFF;
	LED->DISPRAM9 = 0x0000FFFF;
	LED->DISPRAM10 = 0x0000FFFF;
	LED->DISPRAM11 = 0x0000FFFF;
	LED->DISPRAM12 = 0x0000FFFF;	// COM12 [SEG15 to SEG0]

	/* Start LED */
	HAL_LED_DISP_ON();
	
	while(1)
	{
		for(i=0; i<16; i++)
		{
			for(j=0; j<500000; j++);
			LED->DISPRAM0 = DIGIT_HEX[i];
			LED->DISPRAM1 = DIGIT_HEX[i];
			LED->DISPRAM2 = DIGIT_HEX[i];
			LED->DISPRAM3 = DIGIT_HEX[i];
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LED_SEG_GND_Test(void)
{
	unsigned long i,j;
	
	/* LED Port Initialization - ICOM0~ICOM12 & ISEG0~ISEG15 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC5);

	/* LED Config */
	HAL_SCU_LED_ClockConfig(LEDCLK_PCLK);

	LED_Config.tPRESD = 15;							// fLEDCLK = fLEDEXTCLK / (PRESD + 1)	@ fLEDCLK = 1MHz (1us) (at fLEDEXTCLK = 16MHz)
	LED_Config.tCOMPWID = 99;					// COM Width =  (COMPWID + 1) / fLEDCLK @ COM Width = 100us
	LED_Config.tLEDPD = 0xFFFFF;					// LED Period = (LEDPD + 1) / fLEDEXTCLK @ LED Period = 100ms
	LED_Config.tOVERLAP = 0;						// 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = 2 / fLEDEXTCLK
	LED_Config.tSR_INTE = DISABLE;
	LED_Config.tMODE = LED_AUTO_MODE;
	HAL_LED_Init(LED, &LED_Config);

#if 1
	/* SEG-GND Setting */
	LED->LEDCON3 = (0<<4)							// SET TIME (0: 1us / 1: 2us / 2: 4us / 3: 6us / 4: 8us / 5: 10us / 6: 12us / 7: 14us / 8: 16us / 9: 18us / 10: 20us / 11: 22us / 12: 24us / 13: 26us / 14: 28us / 15: 32us)
													| (15<<0);								// CLR TIME (0: 1us / 1: 2us / 2: 4us / 3: 6us / 4: 8us / 5: 10us / 6: 12us / 7: 14us / 8: 16us / 9: 18us / 10: 20us / 11: 22us / 12: 24us / 13: 26us / 14: 28us / 15: 32us)
	LED->LEDCON2 |= (1<<4);						// SEG-GND Function Enable
#endif

	/* COM/SEG Port Setting */
	LED->COMER = 0x00000007;				// COM0 / COM1 / COM2
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)

	LED->DISPRAM0 = 0x00000100;		// COM0 [SEG15 to SEG0] - SEG8 Monitoring
	LED->DISPRAM1 = 0x00000100;
	LED->DISPRAM2 = 0x00000100;
	
	LED->DISPRAM3 = 0x00000000;
	LED->DISPRAM4 = 0x00000000;
	LED->DISPRAM5 = 0x00000000;
	LED->DISPRAM6 = 0x00000000;
	LED->DISPRAM7 = 0x00000000;
	LED->DISPRAM8 = 0x00000000;
	LED->DISPRAM9 = 0x00000000;
	LED->DISPRAM10 = 0x00000000;
	LED->DISPRAM11 = 0x00000000;
	LED->DISPRAM12 = 0x00000000;	// COM12 [SEG15 to SEG0]

	/* Start LED */
	HAL_LED_DISP_ON();
	
	while(1)
	{
#if 0
		for(i=0; i<16; i++)
		{
			for(j=0; j<500000; j++);
			LED->DISPRAM0 = DIGIT_HEX[8];
			LED->DISPRAM1 = DIGIT_HEX[8];
			LED->DISPRAM2 = DIGIT_HEX[8];
			LED->DISPRAM3 = DIGIT_HEX[8];
		}
#endif
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void LED_COM_Overlaptime_Test(void)
{
	/* LED Port Initialization - ICOM0~ICOM12 & ISEG0~ISEG15 */
	HAL_GPIO_ModeConfig(PE, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 6, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PE, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 7, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC5);
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC5);

	/* LED Config */
	HAL_SCU_LED_ClockConfig(LEDCLK_PCLK);

	LED_Config.tPRESD = 15;							// LED_CLK = fLED / (PRESD+1) @ 15 -> 1MHz (1us)
	LED_Config.tCOMPWID = 99;					// COM Width = LED_CLK / (COMPWID+1) @ 99 -> 100us
	LED_Config.tLEDPD = 1000;					// LED Period = LED_CLK * LEDPD // ## LED Period > COM Width * count of COMER ##
	LED_Config.tOVERLAP = 0;						// 0: 64us, 1: 32us, 2: 24us, 3: 21us, 4: 12us, 5: 9us, 6: 6us, 7: 3us // if(tOverLap >= 8) Overlap time = LED_CLK / 2
	LED_Config.tSR_INTE = DISABLE;
	LED_Config.tMODE = LED_AUTO_MODE;
	HAL_LED_Init(LED, &LED_Config);

	LED->COMER = 0x00000041;				// COM0, COM6
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)
#if 0	// default value
	LED->COMER = 0x00001FFF;				// All COM Enable
	LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
	LED->SEGOE = 0x0000FFFF;					// All SET Output Enable (SEG0~SEG15)
#endif
	LED->DISPRAM0 = 0x0000FFFF;		// COM0 [SEG15 to SEG0]
	LED->DISPRAM1 = 0x0000FFFF;
	LED->DISPRAM2 = 0x0000FFFF;
	LED->DISPRAM3 = 0x0000FFFF;
	LED->DISPRAM4 = 0x0000FFFF;
	LED->DISPRAM5 = 0x0000FFFF;
	LED->DISPRAM6 = 0x0000FFFF;
	LED->DISPRAM7 = 0x0000FFFF;
	LED->DISPRAM8 = 0x0000FFFF;
	LED->DISPRAM9 = 0x0000FFFF;
	LED->DISPRAM10 = 0x0000FFFF;
	LED->DISPRAM11 = 0x0000FFFF;
	LED->DISPRAM12 = 0x0000FFFF;	// COM12 [SEG15 to SEG0]

	/* Start LED */
#if 0
	NVIC_EnableIRQ(LED_IRQn);
	NVIC_SetPriority(LED_IRQn, 3);
	__enable_irq();
#endif
	
	HAL_LED_DISP_ON();
}
