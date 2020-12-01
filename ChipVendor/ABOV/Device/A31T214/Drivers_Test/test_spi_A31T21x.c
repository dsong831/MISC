/**********************************************************************
* @file		test_uart_A31T21x.c
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
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_spi_main(void);
static void SPI_SLAVE_INTERRUPT_TEST(void);
static void SPI_SLAVE_POLLING_TEST(void);
static void SPI_MASTER_INTERRUPT_TEST(void);
static void SPI_MASTER_POLLING_TEST(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_spi_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void SPI_SLAVE_INTERRUPT_TEST(void)
{
	SPI_CFG_Type	SPIConfigStruct;

	/* SPI20 Pin Setting */
	// MOSI20 (PC2)	 
	HAL_GPIO_ModeConfig(PC, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PC, 2, ALTERN_FUNC2);
	// MISO20 (PC3)	
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC2);
	//SCK20 (PC4)   
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC2);
	//SS20 (PD0) : slave set as SSx 
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC2);

	/* SPI21 Pin Setting */
	// MOSI21 (PE5)	 
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC2);
	// MISO21 (PE4)	
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC2);
	//SCK21 (PE3)   
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC2);
	//SS21 (PE2) : slave set as SSx 
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC2);

	SPIConfigStruct.tBitsize = SPI_CR_BITSZ_8;
	SPIConfigStruct.tCPOL = SPI_CPOL_ActiveHigh;
	SPIConfigStruct.tCPHA = SPI_CPHA_ODD;
	SPIConfigStruct.tMSBF = SPI_MSBF_MSB;
	SPIConfigStruct.tMS = SPI_MS_SLAVE;
	SPIConfigStruct.tBaudrate = 1000000;		// 1MHz //  Baudrate = PCLK / (BR+1);
	HAL_SPI_Init(SPI20, &SPIConfigStruct);
	HAL_SPI_ConfigInterrupt(SPI20, SPI_INT_SLAVE, ENABLE);
	NVIC_SetPriority(SPI20_IRQn, 3);
	NVIC_EnableIRQ(SPI20_IRQn);
	HAL_SPI_Command(SPI20, ENABLE);
	__enable_irq();

	while(1)
	{
		//if(spi_rx20_Buffer.Buffer[5] == 0x05)
		{
			//PC5_Toggle_RUN();
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void SPI_SLAVE_POLLING_TEST(void)
{
	uint8_t i;
	uint32_t received_data[6];
	SPI_CFG_Type	SPIConfigStruct;

	/* SPI20 Pin Setting */
	// MOSI20 (PC2)	 
	HAL_GPIO_ModeConfig(PC, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PC, 2, ALTERN_FUNC2);
	// MISO20 (PC3)	
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC2);
	//SCK20 (PC4)   
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC2);
	//SS20 (PD0) : slave set as SSx 
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC2);

	/* SPI21 Pin Setting */
	// MOSI21 (PE5)	 
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC2);
	// MISO21 (PE4)	
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC2);
	//SCK21 (PE3)   
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC2);
	//SS21 (PE2) : slave set as SSx 
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC2);

	for(i=0; i<6; i++)
	{
		received_data[i] = 0xFF;
	}
	
	SPIConfigStruct.tBitsize = SPI_CR_BITSZ_8;
	SPIConfigStruct.tCPOL = SPI_CPOL_ActiveHigh;
	SPIConfigStruct.tCPHA = 1;
	SPIConfigStruct.tMSBF = 1;
	SPIConfigStruct.tMS = SPI_MS_SLAVE;
	SPIConfigStruct.tBaudrate = 1000000;		// 1MHz //  Baudrate = PCLK / (BR+1);
	HAL_SPI_Init(SPI20, &SPIConfigStruct);
	HAL_SPI_ConfigInterrupt(SPI20, SPI_INT_POLLING, ENABLE);
	HAL_SPI_Command(SPI20, ENABLE);

	i = 0;
	while(1)
	{
		while(!(SPI20->SR & SPI_SR_RRDY));
		received_data[i] = (SPI20->RDR_TDR);
		i++;
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void SPI_MASTER_POLLING_TEST(void)
{
	uint8_t i;
	uint32_t received_data[6];
	SPI_CFG_Type	SPIConfigStruct;

	/* SPI20 Pin Setting */
	// MOSI20 (PC2)	 
	HAL_GPIO_ModeConfig(PC, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PC, 2, ALTERN_FUNC2);
	// MISO20 (PC3)	
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC2);
	//SCK20 (PC4)   
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC2);
	//SS20 (PD0) : slave set as SSx 
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC2);

	/* SPI21 Pin Setting */
	// MOSI21 (PE5)	 
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC2);
	// MISO21 (PE4)	
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC2);
	//SCK21 (PE3)   
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC2);
	//SS21 (PE2) : slave set as SSx 
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC2);
	
	for(i=0; i<6; i++)
	{
		received_data[i] = 0xFF;
	}
	
	SPIConfigStruct.tBitsize = SPI_CR_BITSZ_8;
	SPIConfigStruct.tCPOL = SPI_CPOL_ActiveHigh;
	SPIConfigStruct.tCPHA = SPI_CPHA_ODD;
	SPIConfigStruct.tMSBF = SPI_MSBF_MSB;
	SPIConfigStruct.tMS = SPI_MS_MASTER;
	SPIConfigStruct.tBaudrate = 1000000;		// 1MHz //  Baudrate = PCLK / (BR+1);
	HAL_SPI_Init(SPI20, &SPIConfigStruct);
	HAL_SPI_ConfigInterrupt(SPI20, SPI_INT_POLLING, ENABLE);
	HAL_SPI_Command(SPI20, ENABLE);

#if 0	// External Device
	/* SPI Master Write 1-byte / Read 5-byte */
	HAL_SPI_TransmitData(SPI20, 0x9F);		// Transmit : 1

	for(i=0; i<6; i++)
	{
		received_data[i] = HAL_SPI_ReceiveData(SPI20);		// Receive : Null, 1st, 2nd, 3rd, 4th, 5th
	}
#else	// LoopBack (For Slave Check)
	for(i=0; i<6; i++)
	{
		HAL_SPI_TransmitData(SPI20, i);
	}
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void SPI_MASTER_INTERRUPT_TEST(void)
{
	uint32_t i;
	uint32_t transmit_data[6];	
	uint32_t received_data[6];	
	SPI_CFG_Type	SPIConfigStruct;

	/* SPI20 Pin Setting */
	// MOSI20 (PC2)	 
	HAL_GPIO_ModeConfig(PC, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PC, 2, ALTERN_FUNC2);
	// MISO20 (PC3)	
	HAL_GPIO_ModeConfig(PC, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 3, ALTERN_FUNC2);
	//SCK20 (PC4)   
	HAL_GPIO_ModeConfig(PC, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PC, 4, ALTERN_FUNC2);
	//SS20 (PD0) : slave set as SSx 
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC2);

	/* SPI21 Pin Setting */
	// MOSI21 (PE5)	 
	HAL_GPIO_ModeConfig(PE, 5, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PE, 5, ALTERN_FUNC2);
	// MISO21 (PE4)	
	HAL_GPIO_ModeConfig(PE, 4, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 4, ALTERN_FUNC2);
	//SCK21 (PE3)   
	HAL_GPIO_ModeConfig(PE, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 3, ALTERN_FUNC2);
	//SS21 (PE2) : slave set as SSx 
	HAL_GPIO_ModeConfig(PE, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PE, 2, ALTERN_FUNC2);
	
	for(i=0; i<6; i++)
	{
		received_data[i] = 0xFF;
	}
	transmit_data[0] = 0x9F;

	SPIConfigStruct.tBitsize = SPI_CR_BITSZ_8;
	SPIConfigStruct.tCPOL = SPI_CPOL_ActiveHigh;
	SPIConfigStruct.tCPHA = SPI_CPHA_ODD;
	SPIConfigStruct.tMSBF = SPI_MSBF_MSB;
	SPIConfigStruct.tMS = SPI_MS_MASTER;
	SPIConfigStruct.tBaudrate = 1000000;		// 1MHz //  Baudrate = PCLK / (BR+1);
	HAL_SPI_Init(SPI20, &SPIConfigStruct);
	HAL_SPI_ConfigInterrupt(SPI20, SPI_INT_MASTER, ENABLE);
	NVIC_SetPriority(SPI20_IRQn, 3);
	NVIC_EnableIRQ(SPI20_IRQn);
	HAL_SPI_Command(SPI20, ENABLE);
	__enable_irq();

	HAL_SPI_TransmitReceiveData_INT(SPI20, transmit_data, 1, received_data, 5);
}
