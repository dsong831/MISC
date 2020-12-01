/**********************************************************************
* @file		test_dma_A31T21x.c
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
void test_dma_main(void);
static void SPI_TRX_DMA_TEST(void);
static void UART_RX_DMA_TEST(void);
static void UART_TX_DMA_TEST(void);
static void USART_UART_RX_DMA_TEST(void);
static void USART_UART_TX_DMA_TEST(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_dma_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void SPI_TRX_DMA_TEST(void)
{
	uint8_t i;
	uint32_t received_data[6];
	SPI_CFG_Type	SPIConfigStruct;
	DMAC_CFG_Type DMAConfigStruct;
	uint8_t dma_data[6] = {0x9F, 0x00, 0x00, 0x00, 0x00, 0x00};

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
		received_data[i] = received_data[i];
	}
	
	SPIConfigStruct.tBitsize = SPI_CR_BITSZ_8;
	SPIConfigStruct.tCPOL = SPI_CPOL_ActiveHigh;
	SPIConfigStruct.tCPHA = SPI_CPHA_ODD;
	SPIConfigStruct.tMSBF = SPI_MSBF_MSB;
	SPIConfigStruct.tMS = SPI_MS_MASTER;
	SPIConfigStruct.tBaudrate = 1000000;		// 1MHz //  Baudrate = PCLK / (BR+1);
	HAL_SPI_Init(SPI21, &SPIConfigStruct);
	HAL_SPI_ConfigInterrupt(SPI21, SPI_INT_POLLING, ENABLE);
	HAL_SPI_Command(SPI21, ENABLE);

#if 1
	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = 6;
	DMAConfigStruct.TransferSize = DMAC_SIZE_BYTE;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_SPI21TX;
	DMAConfigStruct.Dir = DMAC_DIR_TX;

	DMAConfigStruct.PeriAddr = 0x40004D00UL;	// SPI21 Data Register
	DMAConfigStruct.MemAddr = (uint32_t)(&dma_data);

	HAL_DMAC_Init(DMA3, &DMAConfigStruct, ENABLE);

	HAL_DMAC_Cmd(DMA3, SET);
	while(HAL_DMAC_GetStatus(DMA3) == RESET);
	HAL_DMAC_Cmd(DMA3, RESET);

#else
	/* SPI Master Write 1-byte / Read 5-byte */
	HAL_SPI_TransmitData_POL(SPI21, 0x9F);		// Transmit : 1

	for(i=0; i<5; i++)
	{
		received_data[i] = HAL_SPI_ReceiveData_POL(SPI21);		// Receive : Null, 1st, 2nd, 3rd, 4th
	}
	received_data[5] = HAL_SPI_ReadBuffer(SPI21);								// 5th
#endif
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void UART_RX_DMA_TEST(void)
{
	UART_CFG_Type UARTConfigStruct;
	DMAC_CFG_Type DMAConfigStruct;
	uint16_t i;
	uint8_t dma_data[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	/* Initialize UART1 pin connect */
	HAL_GPIO_ModeConfig(PB, 7, ALTERN_FUNC); //RX1
	HAL_GPIO_AlternativeConfig(PB, 7, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 7, 1);
	HAL_GPIO_ModeConfig(PB, 6, ALTERN_FUNC); //TX1
	HAL_GPIO_AlternativeConfig(PB, 6, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 6, 1);	
	
	UARTConfigStruct.tBaudRate = 9600;
	UARTConfigStruct.tDataLength = UART_DATALEN_8;
	UARTConfigStruct.tParity = UART_PARITY_NONE;
	UARTConfigStruct.tStopbit = UART_STOPBIT_1;
	HAL_UART_Init(UART1, &UARTConfigStruct);
	HAL_UART_ConfigInterrupt(UART1, UART_INT_POLLING, ENABLE);

	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = 10;
	DMAConfigStruct.TransferSize = DMAC_SIZE_BYTE;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_UART1RX;
	DMAConfigStruct.Dir = DMAC_DIR_RX;

	DMAConfigStruct.PeriAddr = 0x40004100UL;	// UART Data Register
	DMAConfigStruct.MemAddr = (uint32_t)(&dma_data);

	HAL_DMAC_Init(DMA1, &DMAConfigStruct, ENABLE);

	HAL_DMAC_Cmd(DMA1, SET);
	while(HAL_DMAC_GetStatus(DMA1) == RESET);
	HAL_DMAC_Cmd(DMA1, RESET);
	
	for(i=0; i<10; i++)
	{
		HAL_UART_TransmitByte(UART1, dma_data[i]);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void UART_TX_DMA_TEST(void)
{
	UART_CFG_Type UARTConfigStruct;
	DMAC_CFG_Type DMAConfigStruct;
	uint8_t dma_data[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	/* Initialize UART1 pin connect */
	HAL_GPIO_ModeConfig(PB, 7, ALTERN_FUNC); //RX1
	HAL_GPIO_AlternativeConfig(PB, 7, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 7, 1);
	HAL_GPIO_ModeConfig(PB, 6, ALTERN_FUNC); //TX1
	HAL_GPIO_AlternativeConfig(PB, 6, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 6, 1);	
	
	UARTConfigStruct.tBaudRate = 9600;
	UARTConfigStruct.tDataLength = UART_DATALEN_8;
	UARTConfigStruct.tParity = UART_PARITY_NONE;
	UARTConfigStruct.tStopbit = UART_STOPBIT_1;
	HAL_UART_Init(UART1, &UARTConfigStruct);
	HAL_UART_ConfigInterrupt(UART1, UART_INT_POLLING, ENABLE);

	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = 10;
	DMAConfigStruct.TransferSize = DMAC_SIZE_BYTE;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_UART1TX;
	DMAConfigStruct.Dir = DMAC_DIR_TX;

	DMAConfigStruct.PeriAddr = 0x40004100UL;	// UART Data Register
	DMAConfigStruct.MemAddr = (uint32_t)(&dma_data);

	HAL_DMAC_Init(DMA1, &DMAConfigStruct, ENABLE);

	HAL_DMAC_Cmd(DMA1, SET);
	while(HAL_DMAC_GetStatus(DMA1) == RESET);
	HAL_DMAC_Cmd(DMA1, RESET);
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_UART_RX_DMA_TEST(void)
{
	USART_CFG_Type USARTConfigStruct;
	DMAC_CFG_Type DMAConfigStruct;
	uint16_t i;
	uint8_t dma_data[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	/* Config USART10 PIN */
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC); //RXD10
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 1, 1);

	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC); //TXD10
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 0, 1);

	/* USART10 Initialization */
	USARTConfigStruct.tBaudRate = 9600;
	USARTConfigStruct.tOpMode = USART_UART_MODE;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_RXCIE_TXCIE, ENABLE);

	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = 10;
	DMAConfigStruct.TransferSize = DMAC_SIZE_BYTE;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_USART10RX;
	DMAConfigStruct.Dir = DMAC_DIR_RX;

	DMAConfigStruct.PeriAddr = 0x40003814UL;	// USART Data Register
	DMAConfigStruct.MemAddr = (uint32_t)(&dma_data);

	HAL_DMAC_Init(DMA3, &DMAConfigStruct, ENABLE);

	HAL_DMAC_Cmd(DMA3, SET);
	while(HAL_DMAC_GetStatus(DMA3) == RESET);
	HAL_DMAC_Cmd(DMA3, RESET);
	
	for(i=0; i<10; i++)
	{
		HAL_USART_TransmitByte(USART10, dma_data[i]);
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void USART_UART_TX_DMA_TEST(void)
{
	USART_CFG_Type USARTConfigStruct;
	DMAC_CFG_Type DMAConfigStruct;
	uint8_t dma_data[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	/* Config USART10 PIN */
	HAL_GPIO_ModeConfig(PB, 1, ALTERN_FUNC); //RXD10
	HAL_GPIO_AlternativeConfig(PB, 1, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 1, 1);

	HAL_GPIO_ModeConfig(PB, 0, ALTERN_FUNC); //TXD10
	HAL_GPIO_AlternativeConfig(PB, 0, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 0, 1);

	/* USART10 Initialization */
	USARTConfigStruct.tBaudRate = 9600;
	USARTConfigStruct.tOpMode = USART_UART_MODE;
	USARTConfigStruct.tDataLength = USART_DATALEN_8;
	USARTConfigStruct.tParity = USART_PARITY_NONE;
	USARTConfigStruct.tStopbit = USART_STOPBIT_1;
	HAL_USART_Init(USART10, &USARTConfigStruct);
	HAL_USART_ConfigInterrupt(USART10, USART_INT_POLLING, ENABLE);

	/* DMA Initialization */
	DMAConfigStruct.TransferCnt = 10;
	DMAConfigStruct.TransferSize = DMAC_SIZE_BYTE;
	DMAConfigStruct.PeriSel = DMAC_PERISEL_USART10TX;
	DMAConfigStruct.Dir = DMAC_DIR_TX;

	DMAConfigStruct.PeriAddr = 0x40003814UL;	// USART Data Register
	DMAConfigStruct.MemAddr = (uint32_t)(&dma_data);

	HAL_DMAC_Init(DMA1, &DMAConfigStruct, ENABLE);

#if 1		// DMA USART TRX
	HAL_DMAC_Cmd(DMA1, SET);
	while(HAL_DMAC_GetStatus(DMA1) == RESET);
	HAL_DMAC_Cmd(DMA1, RESET);

#else	// Normal USART TRX
	for(i=0; i<0xFFF; i++);	// For debugger timing

	/* TX */
	data = 0;
	for(i=0; i<256; i++)
	{
		HAL_USART_TransmitByte(USART10, 'F');
		data++;
	}

	/* RX */
	data = HAL_USART_ReceiveByte(USART10);
	if(data == 0x30)
	{
		HAL_USART_TransmitByte(USART10, 'O');
		HAL_USART_TransmitByte(USART10, 'K');
	}
#endif
}

