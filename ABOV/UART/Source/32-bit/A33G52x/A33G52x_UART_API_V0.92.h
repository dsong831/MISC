/***************************************************************************//**
*   @file					A33G52x_UART_API_V0.92.h
*   @brief				UART Peripheral Function
*   @author		ABOV Semiconductor Co., Ltd.
*   @version		V1.00
*   @date				5. Mar. 2020
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
********************************************************************************
* DISCLAIMER 
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, ABOV SEMICONDUCTOR DISCLAIMS ALL LIABILITIES FROM ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
*******************************************************************************/

/*******************************************************************************
* Included File
*******************************************************************************/
#include <stdint.h>
#include "A33G52x.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/
#define fUSE_UART_TX_INTERRUPT
#define fUSE_UART_RX_INTERRUPT
#define fUART_MAX_RX_BUFFER							(100)
#define fUART_MAX_TX_BUFFER							(100)

#define fUART_RX_STATE_IDLE								0
#define fUART_RX_STATE_RECEIVE					1
#define fUART_TX_STATE_IDLE								0
#define fUART_TX_STATE_TRANSMIT			1

#define fUART_NO_PARITY																					(0)
#define fUART_ODD_PARITY																			(1)
#define fUART_EVEN_PARITY																			(2)

#define fUART_TX_BUFFER_SUCCESS												(0)
#define fUART_TX_BUFFER_ERROR_WAIT_TIMEOUT	(2)
#define fUART_RX_BUFFER_SUCCESS												(0)
#define fUART_RX_BUFFER_ERROR_EMPTY							(2)

#define fUnIER_TEMTIE													(0x0001UL<<3)
#define fUnIER_RLSIE															(0x0001UL<<2)
#define fUnIER_THREIE													(0x0001UL<<1)
#define fUnIER_DRIE															(0x0001UL<<0)
#define fUnIER_INTR_MASK										(0x000FUL<<0)

#define fUnIIR_IID_RCV_LINE_STATUS		(0x0003UL<<1)
#define fUnIIR_IID_RBR_READY							(0x0002UL<<1)
#define fUnIIR_IID_THR_EMPTY							(0x0001UL<<1)

#define fUnLCR_DLAB														(0x0001UL<<7)
#define fUnLCR_BREAK													(0x0001UL<<6)
#define fUnLCR_STICKP													(0x0001UL<<5)
#define fUnLCR_PARITY													(0x0001UL<<4)
#define fUnLCR_PEN															(0x0001UL<<3)
#define fUnLCR_STOPBIT											(0x0001UL<<2)
#define fUnLCR_STOPBIT_1_STOPBIT		(0x0000UL<<2)
#define fUnLCR_STOPBIT_2_STOPBITS	(0x0001UL<<2)
#define fUnLCR_DLEN_5_BITS								(0x0000UL<<0)
#define fUnLCR_DLEN_6_BITS								(0x0001UL<<0)
#define fUnLCR_DLEN_7_BITS								(0x0002UL<<0)
#define fUnLCR_DLEN_8_BITS								(0x0003UL<<0)
#define fUnLCR_DLEN_MASK									(0x0003UL<<0)

#define fUnLSR_FIFOE														(0x0001UL<<7)
#define fUnLSR_TEMT														(0x0001UL<<6)
#define fUnLSR_THRE														(0x0001UL<<5)
#define fUnLSR_BI																	(0x0001UL<<4)
#define fUnLSR_FE																	(0x0001UL<<3)
#define fUnLSR_PE																(0x0001UL<<2)
#define fUnLSR_OE																(0x0001UL<<1)
#define fUnLSR_DR																(0x0001UL<<0)

#define fUnDTR_SMS															(0x0001UL<<7)
#define fUnDTR_DMS															(0x0001UL<<6)
#define fUnDTR_RXINV														(0x0001UL<<5)
#define fUnDTR_TXINV														(0x0001UL<<4)
#define fUnDTR_WAITVAL(n)									(((n)&0x07UL)<<0)

#define fUnIIR_INTR_BASIC_MASK					(0x0007UL<<0)
/*******************************************************************************
* Private Typedef
*******************************************************************************/
typedef struct {
	uint16_t	RxState; 
	uint16_t	TxState; 
	uint16_t	RxBuffer_HeadIndex;
	uint16_t 	RxBuffer_TailIndex;
	uint16_t	TxBuffer_HeadIndex;
	uint16_t 	TxBuffer_TailIndex;
	uint8_t		RxBuffer[fUART_MAX_RX_BUFFER];
	uint8_t		TxBuffer[fUART_MAX_TX_BUFFER]; 
} tUART_BUFFER;

/*******************************************************************************
* Private Variable
*******************************************************************************/

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
UART_Type* HAL_UART_Get_Object(uint8_t uart_no);
tUART_BUFFER* HAL_Uart_GetBufferBaseAddr(uint8_t uart_no);
void HAL_Uart_BufferInit(uint8_t uart_no);
void HAL_Uart_IntConfig(uint8_t uart_no, uint32_t intr_mask, uint32_t int_enable);
void HAL_Uart_SetBaudrate(UART_Type* UARTx, uint32_t baudrate, uint32_t ref_clk);
void HAL_Uart_Init(uint8_t uart_no, uint32_t Baudrate, uint8_t DataBits, uint8_t Parity, uint8_t StopBits, uint8_t int_enable);
void HAL_Uart_Enable_Tx_Interrupt(UART_Type* UARTx, uint8_t status);
void HAL_Uart_Tx_Handler(uint8_t uart_no);
void HAL_Uart_Rx_Handler(uint8_t uart_no);
void HAL_Uart_Handler(uint8_t uart_no);
uint8_t HAL_Uart_WriteBuffer(uint8_t uart_no, uint8_t *p_data, uint32_t data_count);
uint8_t HAL_Uart_ReadBuffer(uint8_t uart_no, uint8_t *p_status);


/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
