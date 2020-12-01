/**********************************************************************
* @file		test_crc_A31T21x.c
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
static DMAC_CFG_Type DMAConfigStruct;
/* Private define ------------------------------------------------------------*/
#define CRC16_BYPASS						0x8005								//	CRC-16
#define CRC16_CCITT								0x1021								//	CRC-CCITT
#define POLYNOM											CRC16_BYPASS	// Select mode

#define WORD_LENGTH							0
#define BUF_LENGTH									1

#define CRC_MSB_FIRST						0
#define CRC_LSB_FIRST						1
#define NUM_OF_CRC_DUMMY	10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_crc_main(void);
static void gen_crc_table(void);
static unsigned short crc_gen(unsigned short crc_init, unsigned long crc_in, unsigned char ind1, unsigned char ind2);
static void CRC_CHECKSUM_DMA_TEST(void);
static void CRC_CHECKSUM_TEST(void);
static void CRC_16_DMA_TEST(void);
static void CRC_16_TEST(void);
static void CRC_CCITT_DMA_TEST(void);
static void CRC_CCITT_TEST(void);
static void PC5_Toggle_RUN(void);
/* Private variables ---------------------------------------------------------*/
static unsigned short crc_table[256];
static unsigned short polynominal = POLYNOM;
static unsigned long crc_start_adr;
static unsigned long crc_end_adr;
static uint32_t crc_in;
static uint32_t crc_dummy_data[NUM_OF_CRC_DUMMY] = {0x12345678, 0x9ABCDEF1, 0x23456789, 0xABCDEF12, 0x3456789A, 0xBCDEF123, 0x456789AB, 0xCDEF1234, 0x56789ABC, 0xDEF12345};


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_crc_main(void)
{

}

/**********************************************************************
 * @brief		gen_crc_table
 * @param[in]	None
 * @return 		None
 **********************************************************************/
static void gen_crc_table(void) 
{
	unsigned short i, j;
	unsigned short crc_accum;
	
	for( i = 0; i < 256; i++ ) {
		crc_accum = i<<8;
		for (j=0; j<8; j++) {
			if(crc_accum & 0x8000) crc_accum = (crc_accum<<1 ) ^ polynominal;
			else crc_accum <<= 1;
		}
		crc_table[i] = crc_accum;
	}
}

/**********************************************************************
 * @brief		crc_gen
 * @param[in]	None
 * @return 		None
 **********************************************************************/
static unsigned short crc_gen(unsigned short crc_init, unsigned long crc_in, unsigned char ind1, unsigned char ind2) 
{
	unsigned char i;
	unsigned long tmp=0, *st_adr, *ed_adr;
	
	gen_crc_table();
	if(ind1 == WORD_LENGTH) {
		if(ind2 != CRC_MSB_FIRST) {
			for(i=0 ; i<32 ; i++) {
				tmp <<= 1;
				if(crc_in & 0x00000001ul) tmp |= 0x00000001;
				crc_in >>= 1;
			}
			crc_in = tmp;
		}
		for(i=0 ; i<4 ; i++) crc_init = (crc_init<<8) ^ crc_table[((crc_init>>8) ^ (unsigned short)(crc_in>>(24-i*8)))&0x00FF];
	}
	else {
		st_adr = (unsigned long *)crc_start_adr;
		ed_adr = (unsigned long *)crc_end_adr;
		while(st_adr <= ed_adr) {
			crc_in = *st_adr++;
			if(ind2 != CRC_MSB_FIRST) {
				for(i=0 ; i<32 ; i++) {
					tmp <<= 1;
					if(crc_in & 0x00000001ul) tmp |= 0x00000001;
					crc_in >>= 1;
				}
				crc_in = tmp;
			}
			for(i=0 ; i<4 ; i++) crc_init = (crc_init<<8) ^ crc_table[((crc_init>>8) ^ (unsigned short)(crc_in>>(24-i*8)))&0x00FF];
		}
	}
	return crc_init;
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void CRC_CHECKSUM_DMA_TEST(void)
{
	uint16_t chk_sum_init;
	uint16_t chk_sum_manual;
	uint16_t chk_sum_crc;
	uint32_t i, j;

	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = NUM_OF_CRC_DUMMY;
	DMAConfigStruct.TransferSize = DMAC_SIZE_WORD;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_CRC;
	DMAConfigStruct.Dir = DMAC_DIR_TX;
	DMAConfigStruct.PeriAddr = 0x40000304UL;	// CRC_IN Register
	DMAConfigStruct.MemAddr = (uint32_t)(&crc_dummy_data);
	HAL_DMAC_Init(DMA1, &DMAConfigStruct, ENABLE);

	HAL_CRC_Init();
	chk_sum_init = 0;								// CRC_INIT Value

	/* Manual SW Calculation */
	chk_sum_manual = 0;
	for(i=0;i<NUM_OF_CRC_DUMMY;i++){
		for (j=0; j<4; j++) {
			chk_sum_manual += (uint16_t) (crc_dummy_data[i] >> (24-j*8)) & 0x00FF;
		}
	}

	/* CRC Block Calculation */
	HAL_CRC_SetUserInput(chk_sum_init);
	HAL_CRC_ConfigUserMode(CRC_CHECKSUM, CRC_CCITT, CRC_MSB);
	HAL_CRC_ConfigInterrupt(ENABLE);
	NVIC_SetPriority(CRC_IRQn, 3);
  NVIC_EnableIRQ(CRC_IRQn);
	__enable_irq();
	
  CRCRun();
	HAL_DMAC_Cmd(DMA1, SET);
	while(HAL_DMAC_GetStatus(DMA1) == RESET);
	HAL_DMAC_Cmd(DMA1, RESET);
	chk_sum_crc = HAL_CRC_GetData();
	CRCStop();
	
	// Check Result
	if(chk_sum_manual == chk_sum_crc)
	{
		PC5_Toggle_RUN();
	}
	else
	{
		while(1);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void CRC_CHECKSUM_TEST(void)
{
  uint16_t chk_sum_init;	
	uint16_t chk_sum_manual;
	uint16_t chk_sum_crc;
	uint32_t i;
	
	HAL_CRC_Init();

	chk_sum_init = 0;								/* CRC_INIT Value*/
	crc_in = 0x12345678;					/* CRC_IN Value*/
	
	// Manual SW Calculation
	chk_sum_manual = 0;
	for(i=0; i<4; i++)
	{
		chk_sum_manual += (uint16_t)(crc_in >> (24-i*8)) & 0x00FF;
	}

	// CRC Block Calculation
	HAL_CRC_SetUserInput(chk_sum_init);
	HAL_CRC_ConfigUserMode(CRC_CHECKSUM, CRC_CCITT, CRC_MSB);
	CRCRun();
	HAL_CRC_UserInput(crc_in);
	chk_sum_crc = 0;
	chk_sum_crc = HAL_CRC_GetData();
	CRCStop();

	// Check Result
	if(chk_sum_manual == chk_sum_crc)
	{
		PC5_Toggle_RUN();
	}
	else
	{
		while(1);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void CRC_16_DMA_TEST(void)
{
	uint16_t crc16_sum_init;
	uint16_t crc16_sum_manual;
	uint16_t crc16_sum_crc;
	uint32_t i;

	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = NUM_OF_CRC_DUMMY;
	DMAConfigStruct.TransferSize = DMAC_SIZE_WORD;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_CRC;
	DMAConfigStruct.Dir = DMAC_DIR_TX;
	DMAConfigStruct.PeriAddr = 0x40000304UL;	// CRC_IN Register
	DMAConfigStruct.MemAddr = (uint32_t)(&crc_dummy_data);
	HAL_DMAC_Init(DMA1, &DMAConfigStruct, ENABLE);

	HAL_CRC_Init();
	crc16_sum_init = 0;								// CRC_INIT Value

	/* Manual SW Calculation */
	crc16_sum_manual = 0;
	for(i=0;i<NUM_OF_CRC_DUMMY;i++)
	{
		crc16_sum_manual = crc_gen(crc16_sum_manual, crc_dummy_data[i], WORD_LENGTH, CRC_MSB_FIRST);
	}

	/* CRC Block Calculation */
	HAL_CRC_SetUserInput(crc16_sum_init);
	HAL_CRC_ConfigUserMode(CRC_CRC, CRC_16, CRC_MSB);
	HAL_CRC_ConfigInterrupt(ENABLE);
	NVIC_SetPriority(CRC_IRQn, 3);
  NVIC_EnableIRQ(CRC_IRQn);
	__enable_irq();
	
  CRCRun();
	HAL_DMAC_Cmd(DMA1, SET);
	while(HAL_DMAC_GetStatus(DMA1) == RESET);
	HAL_DMAC_Cmd(DMA1, RESET);
	crc16_sum_crc = HAL_CRC_GetData();
	CRCStop();
	
	// Check Result
	if(crc16_sum_manual == crc16_sum_crc)
	{
		PC5_Toggle_RUN();
	}
	else
	{
		while(1);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void CRC_16_TEST(void)
{
  uint16_t crc16_sum_init;	
	uint16_t crc16_sum_manual;
	uint16_t crc16_sum_crc;
	
	HAL_CRC_Init();

	crc16_sum_init = 0;								/* CRC_INIT Value*/
	crc_in = 0x98765432;							/* CRC_IN Value*/
	
	// Manual SW Calculation
	crc16_sum_manual = 0;
	crc16_sum_manual = crc_gen(crc16_sum_init, crc_in, WORD_LENGTH, CRC_MSB_FIRST);

	// CRC Block Calculation
	HAL_CRC_SetUserInput(crc16_sum_init);
	HAL_CRC_ConfigUserMode(CRC_CRC, CRC_16, CRC_MSB);
	CRCRun();
	HAL_CRC_UserInput(crc_in);
	crc16_sum_crc = 0;
	crc16_sum_crc = HAL_CRC_GetData();
	CRCStop();

	// Check Result
	if(crc16_sum_manual == crc16_sum_crc)
	{
		PC5_Toggle_RUN();
	}
	else
	{
		while(1);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void CRC_CCITT_DMA_TEST(void)
{
	uint16_t ccitt_sum_init;
	uint16_t ccitt_sum_manual;
	uint16_t ccitt_sum_crc;
	uint32_t i;

	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = NUM_OF_CRC_DUMMY;
	DMAConfigStruct.TransferSize = DMAC_SIZE_WORD;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_CRC;
	DMAConfigStruct.Dir = DMAC_DIR_TX;
	DMAConfigStruct.PeriAddr = 0x40000304UL;	// CRC_IN Register
	DMAConfigStruct.MemAddr = (uint32_t)(&crc_dummy_data);
	HAL_DMAC_Init(DMA1, &DMAConfigStruct, ENABLE);

	HAL_CRC_Init();
	ccitt_sum_init = 0;								// CRC_INIT Value

	/* Manual SW Calculation */
	ccitt_sum_manual = 0;
	for(i=0;i<NUM_OF_CRC_DUMMY;i++)
	{
		ccitt_sum_manual = crc_gen(ccitt_sum_manual, crc_dummy_data[i], WORD_LENGTH, CRC_MSB_FIRST);
	}

	/* CRC Block Calculation */
	HAL_CRC_SetUserInput(ccitt_sum_init);
	HAL_CRC_ConfigUserMode(CRC_CRC, CRC_CCITT, CRC_MSB);
	HAL_CRC_ConfigInterrupt(ENABLE);
	NVIC_SetPriority(CRC_IRQn, 3);
  NVIC_EnableIRQ(CRC_IRQn);
	__enable_irq();
	
  CRCRun();
	HAL_DMAC_Cmd(DMA1, SET);
	while(HAL_DMAC_GetStatus(DMA1) == RESET);
	HAL_DMAC_Cmd(DMA1, RESET);
	ccitt_sum_crc = HAL_CRC_GetData();
	CRCStop();
	
	// Check Result
	if(ccitt_sum_manual == ccitt_sum_crc)
	{
		PC5_Toggle_RUN();
	}
	else
	{
		while(1);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void CRC_CCITT_TEST(void)
{
  uint16_t ccitt_sum_init;	
	uint16_t ccitt_sum_manual;
	uint16_t ccitt_sum_crc;
	
	HAL_CRC_Init();

	ccitt_sum_init = 0;								/* CRC_INIT Value*/
	crc_in = 0xABCDEF00;				/* CRC_IN Value*/
	
	// Manual SW Calculation
	ccitt_sum_manual = 0;
	ccitt_sum_manual = crc_gen(ccitt_sum_init, crc_in, WORD_LENGTH, CRC_MSB_FIRST);

	// CRC Block Calculation
	HAL_CRC_SetUserInput(ccitt_sum_init);
	HAL_CRC_ConfigUserMode(CRC_CRC, CRC_CCITT, CRC_MSB);
	CRCRun();
	HAL_CRC_UserInput(crc_in);
	ccitt_sum_crc = 0;
	ccitt_sum_crc = HAL_CRC_GetData();
	CRCStop();

	// Check Result
	if(ccitt_sum_manual == ccitt_sum_crc)
	{
		PC5_Toggle_RUN();
	}
	else
	{
		while(1);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void PC5_Toggle_RUN(void)
{
	unsigned int i;

   // Test Pin setting PC5
	HAL_GPIO_ModeConfig(PC, 5, PUSH_PULL_OUTPUT);
	HAL_GPIO_PUPDConfig(PC, 5, PUPD_DISABLE);

	while(1)
	{
		for(i=0; i<0x5FFFF; i++);
		HAL_GPIO_SetPin(PC, 5);
		for(i=0; i<0x5FFFF; i++);
		HAL_GPIO_ClearPin(PC, 5);
	}
}
