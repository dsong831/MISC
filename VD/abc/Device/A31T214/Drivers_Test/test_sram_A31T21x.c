/**********************************************************************
* @file		test_sram_A31T21x.c
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
#define SRAM_START_ADDR 0x20003700 //0x20003000 // 0x20000000
#define SRAM_TEST_SIZE  0x1000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t gu32_errcount;
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_sram_main(void);
static uint32_t SRAM_Test(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_sram_main(void)
{
	gu32_errcount = SRAM_Test();
	if(gu32_errcount)
	{
		while(1);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static uint32_t SRAM_Test(void)
{
	int i;
	unsigned char initval;
	unsigned char *p = (unsigned char *) SRAM_START_ADDR;
	uint32_t errorcnt_ff=0;
	uint32_t errorcnt_00=0;
	uint32_t errorcnt_55=0;
	uint32_t errorcnt_aa=0;

///////////////////////////////////////////////////////////////////	
	initval = 0xff;
	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		p[i] = ((unsigned char) initval) & 0xff;
	}

	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		if (p[i] != initval){
			errorcnt_ff++;
		}
	}
	
///////////////////////////////////////////////////////////////////	
	initval = 0x00;
	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		p[i] = ((unsigned char) initval) & 0xff;
	}

	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		if (p[i] != initval){
			errorcnt_00++;
		}
	}

///////////////////////////////////////////////////////////////////	
	initval = 0x55;
	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		p[i] = ((unsigned char) initval) & 0xff;
	}

	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		if (p[i] != initval){
			errorcnt_55++;
		}
	}

///////////////////////////////////////////////////////////////////	
	initval = 0xaa;
	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		p[i] = ((unsigned char) initval) & 0xff;
	}

	for (i = 0; i < SRAM_TEST_SIZE; i++) {
		if (p[i] != initval){
			errorcnt_aa++;
		}
	}	
	
	return (errorcnt_ff);
}

