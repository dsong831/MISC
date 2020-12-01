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
void test_uart_main(void);
extern void UART_INTERRUPT_TEST(void);
extern void UART_POLLING_TEST(void);
/* Private variables ---------------------------------------------------------*/


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_uart_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void UART_INTERRUPT_TEST(void)
{
	UART_CFG_Type UARTConfigStruct;
	int8_t data;
	uint16_t i;
#if 0
	/* Initialize UART0 pin connect */
	HAL_GPIO_ModeConfig(PB, 5, ALTERN_FUNC); //RX0
	HAL_GPIO_AlternativeConfig(PB, 5, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 5, 1);
	HAL_GPIO_ModeConfig(PB, 4, ALTERN_FUNC); //TX0
	HAL_GPIO_AlternativeConfig(PB, 4, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 4, 1);	
#endif
	/* Initialize UART1 pin connect */
	HAL_GPIO_ModeConfig(PB, 7, ALTERN_FUNC); //RX1
	HAL_GPIO_AlternativeConfig(PB, 7, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 7, 1);
	HAL_GPIO_ModeConfig(PB, 6, ALTERN_FUNC); //TX1
	HAL_GPIO_AlternativeConfig(PB, 6, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 6, 1);	

	/* UART Setting */
	UARTConfigStruct.tBaudRate = 460800;
	UARTConfigStruct.tDataLength = UART_DATALEN_8;
	UARTConfigStruct.tParity = UART_PARITY_NONE;
	UARTConfigStruct.tStopbit = UART_STOPBIT_1;
	HAL_UART_Init(UART1, &UARTConfigStruct);
	HAL_UART_ConfigInterrupt(UART1, UART_INT_RXE_THRE_RLS, ENABLE);
	NVIC_SetPriority(UART1_IRQn, 3);
	NVIC_EnableIRQ(UART1_IRQn);
	__enable_irq();

	for(i=0; i<0xFFF; i++);	// For debugger timing

	/* TX */
	data = 0;
	for(i=0; i<256; i++)
	{
		while(!(UART1->LSR & UART_LSR_THRE));
		HAL_UART_TransmitByte(UART1, 0x35);
	}
	
	while(1)
	{
		HAL_UART_Receive(UART1, &data, 1, BLOCKING);
		if(data != -1)
		{
			while(!(UART1->LSR & UART_LSR_THRE));
			HAL_UART_TransmitByte(UART1, data);
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void UART_POLLING_TEST(void)
{
	UART_CFG_Type UARTConfigStruct;
	uint8_t data;
	uint16_t i;
#if 0
	/* Initialize UART0 pin connect */
	HAL_GPIO_ModeConfig(PB, 5, ALTERN_FUNC); //RX0
	HAL_GPIO_AlternativeConfig(PB, 5, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 5, 1);
	HAL_GPIO_ModeConfig(PB, 4, ALTERN_FUNC); //TX0
	HAL_GPIO_AlternativeConfig(PB, 4, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 4, 1);	
#endif
	/* Initialize UART1 pin connect */
	HAL_GPIO_ModeConfig(PB, 7, ALTERN_FUNC); //RX1
	HAL_GPIO_AlternativeConfig(PB, 7, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 7, 1);
	HAL_GPIO_ModeConfig(PB, 6, ALTERN_FUNC); //TX1
	HAL_GPIO_AlternativeConfig(PB, 6, ALTERN_FUNC1);
	HAL_GPIO_PUPDConfig(PB, 6, 1);	
	
	UARTConfigStruct.tBaudRate = 38400;
	UARTConfigStruct.tDataLength = UART_DATALEN_8;
	UARTConfigStruct.tParity = UART_PARITY_NONE;
	UARTConfigStruct.tStopbit = UART_STOPBIT_1;
	HAL_UART_Init(UART1, &UARTConfigStruct);
	HAL_UART_ConfigInterrupt(UART1, UART_INT_POLLING, ENABLE);

	
	for(i=0; i<0xFFF; i++);	// For debugger timing

	/* TX */
	data = 0;
	for(i=0; i<256; i++)
	{
		while(!(UART1->LSR & UART_LSR_THRE));
		HAL_UART_TransmitByte(UART1, 'F');
		data++;
	}

	/* RX */
	data = HAL_UART_ReceiveByte(UART1);
	if(data == 0x30)
	{
		while(!(UART1->LSR & UART_LSR_THRE));
		HAL_UART_TransmitByte(UART1, 'O');
		while(!(UART1->LSR & UART_LSR_THRE));
		HAL_UART_TransmitByte(UART1, 'K');
	}
	
	RegisterWrite_Handler(UART1);
}
