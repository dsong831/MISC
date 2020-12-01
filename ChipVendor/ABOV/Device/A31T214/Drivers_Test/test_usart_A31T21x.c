/**********************************************************************
* @file		test_usart_A31T21x.c
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
void test_usart_main(void);
static void USART_SPI_SLAVE_INTERRUPT_TEST(void);
static void USART_SPI_SLAVE_POLLING_TEST(void);
static void USART_SPI_MASTER_INTERRUPT_TEST(void);
static void USART_SPI_MASTER_POLLING_TEST(void);
static void USART_USART_INTERRUPT_TEST(void);
static void USART_USART_POLLING_TEST(void);
static void USART_UART_INTERRUPT_Test(void);
static void USART_UART_POLLING_RUN(void);
static void PC5_Toggle_RUN(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_usart_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_SPI_SLAVE_INTERRUPT_TEST(void)
{
	USART_CFG_Type	USARTConfigStruct;

	/* Initialize SPI10 pin connect */
	// MOSI10 (PB0)
	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC2);
	// MISO10 (PB1)	
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC2);
	//SCK10 (PB2)   
	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC2);
	//SS10 (PB3) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PB, 3, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PB, 3);

	/* Initialize SPI11 pin connect */
	// MOSI11 (PD2)
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC2);
	// MISO11 (PD3)	
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC2);
	//SCK11 (PD5)   
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC2);
	//SS11 (PD4) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PD, 4, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PD, 4);

	USARTConfigStruct.tOpMode = USART_SPI_MODE;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tCPOL = 1;
	USARTConfigStruct.tCPHA = 1;
	USARTConfigStruct.tMSBF = USART_MSBF_MSB;
	USARTConfigStruct.tMS = USART_MS_SLAVE;
	USARTConfigStruct.tBaudRate = 1000000;		// 1MHz
	HAL_USART_Init(USART11, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART11, USART_INT_RXCIE_TXCIE, ENABLE);
	NVIC_SetPriority(USART11_IRQn, 3);
	NVIC_EnableIRQ(USART11_IRQn);
	__enable_irq();
	HAL_USART_Command(USART11, ENABLE);

	while(1)
	{
//		if(spi_rx10_Buffer.Buffer[5] == 0x05)
		{
			PC5_Toggle_RUN();
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_SPI_SLAVE_POLLING_TEST(void)
{
	uint8_t i;
	uint32_t received_data[6];
	USART_CFG_Type	USARTConfigStruct;

	/* Initialize SPI10 pin connect */
	// MOSI10 (PB0)
	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC2);
	// MISO10 (PB1)
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC2);
	//SCK10 (PB2)
	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC2);
	//SS10 (PB3) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PB, 3, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PB, 3);

	/* Initialize SPI11 pin connect */
	// MOSI11 (PD2)
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC2);
	// MISO11 (PD3)	
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC2);
	//SCK11 (PD5)   
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC2);
	//SS11 (PD4) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PD, 4, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PD, 4);

	for(i=0; i<6; i++)
	{
		received_data[i] = 0xFF;
	}

	USARTConfigStruct.tOpMode = USART_SPI_MODE;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tCPOL = 1;
	USARTConfigStruct.tCPHA = 1;
	USARTConfigStruct.tBaudRate = 1000000;		// 1MHz
	USARTConfigStruct.tMSBF = USART_MSBF_MSB;

	USARTConfigStruct.tMS = USART_MS_SLAVE;
	HAL_USART_Init(USART11, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART11, USART_INT_POLLING, ENABLE);
	HAL_USART_Command(USART11, ENABLE);

	i = 0;
	while(1)
	{
		while(!(USART11->ST & USART_ST_RXC));
		received_data[i] = (USART11->DR);
		i++;
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_SPI_MASTER_INTERRUPT_TEST(void)
{
	uint32_t i;
	uint8_t transmit_data[6];	
	uint8_t received_data[6];	
	USART_CFG_Type	USARTConfigStruct;

	/* Initialize SPI10 pin connect */
	// MOSI10 (PB0)
	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC2);
	// MISO10 (PB1)	
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC2);
	//SCK10 (PB2)   
	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC2);
	//SS10 (PB3) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PB, 3, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PB, 3);

	/* Initialize SPI11 pin connect */
	// MOSI11 (PD2)
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC2);
	// MISO11 (PD3)	
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC2);
	//SCK11 (PD5)   
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC2);
	//SS11 (PD4) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PD, 4, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PD, 4);

	for(i=0; i<6; i++)
	{
		received_data[i] = 0xFF;
	}
	transmit_data[0] = 0x9F;

	USARTConfigStruct.tOpMode = USART_SPI_MODE;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tCPOL = 1;
	USARTConfigStruct.tCPHA = 1;
	USARTConfigStruct.tMSBF = USART_MSBF_MSB;
	USARTConfigStruct.tMS = USART_MS_MASTER;
	USARTConfigStruct.tBaudRate = 1000000;		// 1MHz
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_RXCIE_TXCIE, ENABLE);
	NVIC_SetPriority(USART10_IRQn, 3);
	NVIC_EnableIRQ(USART10_IRQn);
	__enable_irq();
	HAL_USART_Command(USART10, ENABLE);

	/* SPI Master Write 1-byte / Read 5-byte */
	HAL_GPIO_ClearPin(PB, 3);	// SS Low

	USARTSend(USART10, transmit_data, 1);
	USARTReceive(USART10, received_data, 5);

	HAL_GPIO_SetPin(PB, 3);		// SS High
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_SPI_MASTER_POLLING_TEST(void)
{
	uint8_t i;
	uint32_t received_data[6];
	USART_CFG_Type	USARTConfigStruct;

	/* Initialize SPI10 pin connect */
	// MOSI10 (PB0)
	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC2);
	// MISO10 (PB1)
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC2);
	//SCK10 (PB2)
	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC2);
	//SS10 (PB3) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PB, 3, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PB, 3);

	/* Initialize SPI11 pin connect */
	// MOSI11 (PD2)
	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC);    
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC2);
	// MISO11 (PD3)	
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC2);
	//SCK11 (PD5)   
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC2);
	//SS11 (PD4) - Set SS pin output at master mode and alternative at slave mode
	HAL_GPIO_ModeConfig(PD, 4, PUSH_PULL_OUTPUT);
	HAL_GPIO_SetPin(PD, 4);

	for(i=0; i<6; i++)
	{
		received_data[i] = 0xFF;
	}

	USARTConfigStruct.tOpMode = USART_SPI_MODE;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tMSBF = USART_MSBF_MSB;
	USARTConfigStruct.tCPOL = 1;
	USARTConfigStruct.tCPHA = 1;
	USARTConfigStruct.tBaudRate = 1000000;		// 1MHz

	USARTConfigStruct.tMS = USART_MS_MASTER;
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_POLLING, ENABLE);
	HAL_USART_Command(USART10, ENABLE);

#if 0	// External Device
	/* SPI Master Write 1-byte / Read 5-byte */
	HAL_GPIO_ClearPin(PB, 3);	// SS Low

	while(!(USART10->ST & USART_ST_DRE));
	HAL_USART_TransmitByte(USART10, '0x9F');

	for(i=0; i<6; i++)
	{
		received_data[i] = HAL_USART_SPI_ReceiveData_POL(USART10);		// Receive : Null, 1st, 2nd, 3rd, 4th, 5th
	}

	HAL_GPIO_SetPin(PB, 3);		// SS High

#else	// LoopBack (For Slave Check)
	HAL_GPIO_ClearPin(PB, 3);	// SS Low
	for(i=0; i<6; i++)
	{
		while(!(USART10->ST & USART_ST_DRE));
		HAL_USART_TransmitByte(USART10, i);
	}
	HAL_GPIO_SetPin(PB, 3);		// SS High
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_USART_INTERRUPT_TEST(void)
{
	USART_CFG_Type USARTConfigStruct;
	uint8_t data;
	uint16_t i;

	/* Config USART10 PIN */
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC); //RXD10
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 1, 1);

	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC); //TXD10
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 0, 1);

	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC); //SCK10
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC2);
	
	/* Config USART11 PIN */
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC); //RXD11
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 3, 1);

	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC); //TXD11
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 2, 1);
	
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC); //SCK11
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC2);

	/* USART10 Initialization */
	USARTConfigStruct.tOpMode = USART_USART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	USARTConfigStruct.tMS = USART_MS_MASTER;
	USARTConfigStruct.tCPOL = USART_CPOL_ActiveLow;
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_RXCIE_TXCIE, ENABLE);
	NVIC_SetPriority(USART10_IRQn, 3);
	NVIC_EnableIRQ(USART10_IRQn);
	__enable_irq();

	/* USART11 Initialization */
	USARTConfigStruct.tOpMode = USART_USART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	USARTConfigStruct.tMS = USART_MS_SLAVE;
	USARTConfigStruct.tCPOL = USART_CPOL_ActiveLow;
	HAL_USART_Init(USART11, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART11, USART_INT_RXCIE_TXCIE, ENABLE);
	NVIC_SetPriority(USART11_IRQn, 3);
	NVIC_EnableIRQ(USART11_IRQn);
	__enable_irq();

	for(i=0; i<0xFFF; i++);	// For debugger timing

	/* TX */
	data = 0;
	HAL_USART_Transmit(USART10, "A", 1);
	while(1)
	{
		HAL_USART_Receive(USART11, &data, 1);
		if(data == 'A')
		{
			PC5_Toggle_RUN();
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_USART_POLLING_TEST(void)
{
	USART_CFG_Type USARTConfigStruct;
	uint8_t data;
	uint16_t i;

	/* Config USART10 PIN */
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC); //RXD10
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 1, 1);

	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC); //TXD10
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 0, 1);

	HAL_GPIO_ModeConfig(PB, 2, ALTERN_FUNC); //SCK10
	HAL_GPIO_AlternativeConfig(PB, 2, ALTERN_FUNC2);
	
	/* Config USART11 PIN */
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC); //RXD11
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 3, 1);

	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC); //TXD11
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 2, 1);
	
	HAL_GPIO_ModeConfig(PD, 5, ALTERN_FUNC); //SCK11
	HAL_GPIO_AlternativeConfig(PD, 5, ALTERN_FUNC2);

	/* USART10 Initialization */
	USARTConfigStruct.tOpMode = USART_USART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	USARTConfigStruct.tMS = USART_MS_SLAVE;
	USARTConfigStruct.tCPOL = USART_CPOL_ActiveLow;
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_POLLING, ENABLE);

	/* USART11 Initialization */
	USARTConfigStruct.tOpMode = USART_USART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	USARTConfigStruct.tMS = USART_MS_MASTER;
	USARTConfigStruct.tCPOL = USART_CPOL_ActiveLow;
	HAL_USART_Init(USART11, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART11, USART_INT_POLLING, ENABLE);

	for(i=0; i<0xFFF; i++);	// For debugger timing

	/* TX */
	data = 0;
	HAL_USART_TransmitByte(USART11, 'A');
	data = HAL_USART_ReceiveByte(USART10);

	if(data == 'A')
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
static void USART_UART_INTERRUPT_Test(void)
{
	USART_CFG_Type USARTConfigStruct;
	int8_t data;
	uint16_t i;

	/* Config USART10 PIN */
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC); //RXD10
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 1, 1);

	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC); //TXD10
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 0, 1);

	/* Config USART11 PIN */
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC); //RXD11
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 3, 1);

	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC); //TXD11
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 2, 1);

#if 1
	/* USART10 Setting */
	USARTConfigStruct.tOpMode = USART_UART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_RXCIE_TXCIE, ENABLE);
	NVIC_SetPriority(USART10_IRQn, 3);
	NVIC_EnableIRQ(USART10_IRQn);
	__enable_irq();
#endif
#if 1
	/* USART11 Setting */
	USARTConfigStruct.tOpMode = USART_UART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	HAL_USART_Init(USART11, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART11, USART_INT_RXCIE_TXCIE, ENABLE);
	NVIC_SetPriority(USART11_IRQn, 3);
	NVIC_EnableIRQ(USART11_IRQn);
	__enable_irq();
#endif

	for(i=0; i<0xFFF; i++);	// For debugger timing

#if 0
	/* TX */
	data = 0;
	for(i=0; i<256; i++)
	{
		HAL_USART_Transmit(USART11, "F", 1);
	}

	while(1)
	{
		HAL_USART_Receive(USART11, &data, 1);
		if(data != -1)
		{
			HAL_USART_Transmit(USART11, &data, 1);
		}
	}
#else
	/* TX */
	data = 0;
	HAL_USART_Transmit(USART11, "A", 1);
	while(1)
	{
		HAL_USART_Receive(USART10, &data, 1);
		if(data == 'A')
		{
			PC5_Toggle_RUN();
		}
	}
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_UART_POLLING_RUN(void)
{
	USART_CFG_Type USARTConfigStruct;
	uint8_t data;
	uint16_t i;

	/* Config USART10 PIN */
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC); //RXD10
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 1, 1);

	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC); //TXD10
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 0, 1);
	
	/* Config USART11 PIN */
	HAL_GPIO_ModeConfig(PD, 3, ALTERN_FUNC); //RXD11
	HAL_GPIO_AlternativeConfig(PD, 3, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 3, 1);

	HAL_GPIO_ModeConfig(PD, 2, ALTERN_FUNC); //TXD11
	HAL_GPIO_AlternativeConfig(PD, 2, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PD, 2, 1);
#if 1	
	/* USART10 Initialization */
	USARTConfigStruct.tOpMode = USART_UART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_POLLING, ENABLE);
#endif
#if 1
	/* USART11 Initialization */
	USARTConfigStruct.tOpMode = USART_UART_MODE;
	USARTConfigStruct.tBaudRate = 38400;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	HAL_USART_Init(USART11, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART11, USART_INT_POLLING, ENABLE);
#endif
	for(i=0; i<0xFFF; i++);	// For debugger timing
#if 0
	/* TX */
	data = 0;
	for(i=0; i<256; i++)
	{
		HAL_USART_TransmitByte(USART11, 'F');
		data++;
	}

	/* RX */
	data = HAL_USART_ReceiveByte(USART11);
	if(data == 0x30)
	{
		HAL_USART_TransmitByte(USART11, 'O');
		HAL_USART_TransmitByte(USART11, 'K');
	}
#else
	/* TX */
	data = 0;
	HAL_USART_TransmitByte(USART10, 'A');
	data = HAL_USART_ReceiveByte(USART11);

	if(data == 'A')
	{
		PC5_Toggle_RUN();
	}
	else
	{
		while(1);
	}
#endif
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
		for(i=0; i<0x5FFF; i++);
		HAL_GPIO_SetPin(PC, 5);
		for(i=0; i<0x5FFF; i++);
		HAL_GPIO_ClearPin(PC, 5);
	}
}

