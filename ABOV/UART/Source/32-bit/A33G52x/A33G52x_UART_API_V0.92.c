/***************************************************************************//**
*   @file					A33G52x_UART_API_V0.92.c
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
#include "A33G52x_UART_API_V0.92.h"
#include "A33G52x.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/
tUART_BUFFER	gt_Uart0Buffer;
tUART_BUFFER	gt_Uart1Buffer;
tUART_BUFFER	gt_Uart2Buffer;
tUART_BUFFER	gt_Uart3Buffer;
/*******************************************************************************
* Private Variable
*******************************************************************************/
uint32_t	gu32_SystemPeriClock = 75000000;
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/

/*******************************************************************************
* Public Function
*******************************************************************************/

/**
* @brief
* @param   
* @return
*/
UART_Type* HAL_UART_Get_Object(uint8_t uart_no)
{
	UART_Type* UARTx;

	/* Get Uart Channel Object */
	switch(uart_no)
	{
		case 0:
			UARTx = UART0;
			break;
		case 1:
			UARTx = UART1;
			break;
		case 2:
			UARTx = UART2;
			break;
		case 3:
		default:
			UARTx = UART3;
			break;
	}

	return (UARTx);
}

/**
* @brief
* @param   
* @return
*/
tUART_BUFFER* HAL_Uart_GetBufferBaseAddr(uint8_t uart_no)
{
	tUART_BUFFER* tp_UartBuffer;
	
	/* Get Buffer BaseAddress */
	switch(uart_no)
	{
		case 0:
			tp_UartBuffer = &(gt_Uart0Buffer);
			break;
		case 1:
			tp_UartBuffer = &(gt_Uart1Buffer);
			break;
		case 2:
			tp_UartBuffer = &(gt_Uart2Buffer);
			break;
		case 3:
		default:
			tp_UartBuffer = &(gt_Uart3Buffer);
			break;
	}

	return (tp_UartBuffer);
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_BufferInit(uint8_t uart_no)
{
	tUART_BUFFER* tp_UartBuffer;
	uint8_t i;

	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);

	/* Set Buffer Parameter */
	tp_UartBuffer->RxState = fUART_RX_STATE_IDLE;
	tp_UartBuffer->TxState = fUART_TX_STATE_IDLE;
	tp_UartBuffer->RxBuffer_HeadIndex = 0;
	tp_UartBuffer->TxBuffer_HeadIndex = 0;
	tp_UartBuffer->RxBuffer_TailIndex = 0;
	tp_UartBuffer->TxBuffer_TailIndex = 0;

	for(i=0; i<fUART_MAX_RX_BUFFER; i++)
	{
		tp_UartBuffer->RxBuffer[i] = 0;
	}
	for(i=0; i<fUART_MAX_TX_BUFFER; i++)
	{
		tp_UartBuffer->TxBuffer[i] = 0;
	}
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_IntConfig(uint8_t uart_no, uint32_t intr_mask, uint32_t int_enable)
{
	UART_Type* UARTx;

	uint32_t intr_status;
	uint32_t reg_val;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);

	/* Disable interrupt */
	reg_val = UARTx->IER_DLM;
	reg_val = (reg_val & ~fUnIER_INTR_MASK);
	UARTx->IER_DLM = reg_val;

	/* Clear interrupt flags */
	intr_status = UARTx->IIR;
	if (intr_status & fUnIIR_IID_RCV_LINE_STATUS)
	{
		reg_val = UARTx->LSR;
	}
	if (intr_status & fUnIIR_IID_RBR_READY)
	{
		reg_val = UARTx->RBR_THR_DLL;
	}

	/* Enable interrupt */
	if(int_enable == 1)
	{
		reg_val = UARTx->IER_DLM;
		reg_val = reg_val | (intr_mask & ~(fUnIER_TEMTIE|fUnIER_THREIE));	// When tx function start, tx interrupt is enabled
		UARTx->IER_DLM = reg_val;
	}
	else
	{
		UARTx->IER_DLM = 0;
	}

	/* NVIC Setup */
	NVIC_ClearPendingIRQ(38+uart_no);
	NVIC_EnableIRQ(38+uart_no);
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_SetBaudrate(UART_Type* UARTx, uint32_t baudrate, uint32_t ref_clk)
{
	uint32_t reg_val;
	uint32_t bdr, dlm, dll;
#ifdef fUSE_UART_BFR
	uint32_t d_product_f;
	uint32_t bfr;
#endif

	/* Cal. DLM:DLL */
	bdr = ref_clk / (16*2*baudrate);

	dlm = (bdr>>8) & 0x00FF;
	dll = (bdr & 0x00FF);

	/* Enable DLAB */
	reg_val = UARTx->LCR;
	UARTx->LCR = (reg_val | fUnLCR_DLAB);

	/* Set Baudrate */
	UARTx->IER_DLM = dlm;
	UARTx->RBR_THR_DLL = dll;
#ifdef fUSE_UART_BFR
	d_product_f = ref_clk - (bdr * (16*2*baudrate));
	bfr = (d_product_f << 8) / (16*2*baudrate);				// FCNT = point_value * 256
	UARTx->BFR = bfr;
#endif 

	/* Disable DLAB */
	reg_val = UARTx->LCR;
	UARTx->LCR = (reg_val  & ~fUnLCR_DLAB);
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Init(uint8_t uart_no, uint32_t Baudrate, uint8_t DataBits, uint8_t Parity, uint8_t StopBits, uint8_t int_enable)
{
	UART_Type* UARTx;

	uint32_t uart_intr;
	uint32_t uart_mode;
	uint8_t uart_sampling;
	uint8_t uart_invert;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);

	/* Set Uart Parameter */
	uart_intr = (fUnIER_RLSIE|fUnIER_DRIE|fUnIER_THREIE);							// Enable line/rx/tx interrupt
	uart_sampling = 0;																																				// 0: 1 time sampling , 1: 3 time sampling
	uart_invert = 0;																																							// 0: no invert , 1: invert

	/* Set Uart Buffer */
	HAL_Uart_BufferInit(uart_no);

	/* Set Uart Interrupt */
	HAL_Uart_IntConfig(uart_no, uart_intr, int_enable);

	/* Set Uart Baudrate */
	HAL_Uart_SetBaudrate(UARTx, Baudrate, SystemPeriClock);

	/* Set Uart DataBits */
	switch(DataBits)
	{
		case 5:
			uart_mode = fUnLCR_DLEN_5_BITS;
			break;
		case 6:
			uart_mode = fUnLCR_DLEN_6_BITS;
			break;
		case 7:
			uart_mode = fUnLCR_DLEN_7_BITS;
			break;
		case 8:
		default:
			uart_mode = fUnLCR_DLEN_8_BITS;
			break;
	}

	/* Set Uart Parity */
	if(Parity == fUART_ODD_PARITY)
	{
		uart_mode |= fUnLCR_PEN; 
	}
	else if(Parity == fUART_EVEN_PARITY)
	{
		uart_mode |= (fUnLCR_PEN|fUnLCR_PARITY); 
	}

	/* Set Uart StopBits */
	if(StopBits == 1)
	{
		uart_mode |= fUnLCR_STOPBIT_1_STOPBIT;
	}
	else
	{
		uart_mode |= fUnLCR_STOPBIT_2_STOPBITS;
	}
	
	/* Set Uart Sampling Time */
	if(uart_sampling)
	{
		UARTx->DTR |= (fUnDTR_SMS | fUnDTR_DMS);	// Sampling 3 time run
	}
	else
	{
		UARTx->DTR = 0;																									// Sampling 1 time run
	}

	/* Set Uart Invert */
	if(uart_invert)
	{
		UARTx->DTR |= (fUnDTR_RXINV|fUnDTR_TXINV);
	}

	UARTx->LCR = uart_mode;		// Set Line Control Register
	UARTx->FCR = 0;									// Disable FIFO
	UARTx->SCR = 0;									// Disable Scratch
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Enable_Tx_Interrupt(UART_Type* UARTx, uint8_t status)
{
	uint32_t reg_val;
	
	/* Enable Tx Interrupt */
	if(status == 1)
	{
		reg_val = UARTx->IER_DLM;
		reg_val |= fUnIER_THREIE;
		UARTx->IER_DLM = reg_val;
	}
	/* Disable Tx Interrupt */
	else
	{
		reg_val = UARTx->IER_DLM;
		reg_val &= ~(fUnIER_THREIE);
		UARTx->IER_DLM = reg_val;
	}
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Tx_Handler(uint8_t uart_no)
{
	UART_Type* UARTx;
	tUART_BUFFER *tp_UartBuffer;
	volatile uint32_t line_status;
	uint8_t send_data;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);	

	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);

	/* Read Line Status Register */
	line_status = UARTx->LSR;

	/* Line Error Check */
	if((line_status & fUnLSR_BI) == fUnLSR_BI || (line_status & fUnLSR_PE) == fUnLSR_PE || (line_status & fUnLSR_OE) == fUnLSR_OE)
	{
		line_status = UARTx->LSR;		// Clear Error Flags
	}
	/* Line OK */
	else
	{
		if((line_status & fUnLSR_THRE) == fUnLSR_THRE)
		{
			/* Load data to THR */
			if(tp_UartBuffer->TxBuffer_TailIndex < tp_UartBuffer->TxBuffer_HeadIndex)
			{
				send_data = tp_UartBuffer->TxBuffer[tp_UartBuffer->TxBuffer_TailIndex++];
				UARTx->RBR_THR_DLL = send_data;
				
				// Tx Tail Index Full
				if(tp_UartBuffer->TxBuffer_TailIndex == fUART_MAX_TX_BUFFER)
				{
					tp_UartBuffer->TxBuffer_TailIndex = 0;
				}
			}
			/* Load data to THR */
			else if(tp_UartBuffer->TxBuffer_TailIndex > tp_UartBuffer->TxBuffer_HeadIndex)
			{
				send_data = tp_UartBuffer->TxBuffer[tp_UartBuffer->TxBuffer_TailIndex++];
				UARTx->RBR_THR_DLL = send_data;
				
				// Tx Tail Index Full
				if(tp_UartBuffer->TxBuffer_TailIndex == fUART_MAX_TX_BUFFER)
				{
					tp_UartBuffer->TxBuffer_TailIndex = 0;
				}
			}
			/* Transmission Done */
			else
			{
				if(tp_UartBuffer->TxBuffer_HeadIndex != 0)
				{
#ifdef	fUSE_UART_TX_INTERRUPT
					// Disable tx interrupt
					HAL_Uart_Enable_Tx_Interrupt(UARTx, 0);
#endif
					// Initial parameters
					tp_UartBuffer->TxBuffer_HeadIndex = 0;
					tp_UartBuffer->TxBuffer_TailIndex = 0;
					tp_UartBuffer->TxState = fUART_TX_STATE_IDLE;
				}
			}
		}
	}
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Rx_Handler(uint8_t uart_no)
{
	UART_Type* UARTx;
	tUART_BUFFER *tp_UartBuffer;
	volatile uint32_t line_status;
	uint8_t receive_data;
	
	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);	

	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);
	
	/* Check Line Status Register */
	line_status = UARTx->LSR;
	// Line Error
	if((line_status & fUnLSR_BI) || (line_status & fUnLSR_PE) || (line_status & fUnLSR_OE))
	{
		line_status = UARTx->LSR;	// Clear Error Flags
	}
	// Line OK
	else
	{
		if(line_status & fUnLSR_DR)
		{
			// Fetch data from RBR register
			receive_data = UARTx->RBR_THR_DLL;
			tp_UartBuffer->RxBuffer[tp_UartBuffer->RxBuffer_HeadIndex++] = receive_data;

			// Rx Head Index Full
			if(tp_UartBuffer->RxBuffer_HeadIndex == fUART_MAX_RX_BUFFER)
			{
				tp_UartBuffer->RxBuffer_HeadIndex = 0;
			}
		}
		else
		{
			// Receive Done
			if(tp_UartBuffer->RxBuffer_HeadIndex == tp_UartBuffer->RxBuffer_TailIndex)
			{
				if(tp_UartBuffer->RxBuffer_HeadIndex != 0)
				{
					// Initial parameters
					tp_UartBuffer->RxBuffer_HeadIndex = 0;
					tp_UartBuffer->RxBuffer_TailIndex = 0;
					tp_UartBuffer->RxState = fUART_RX_STATE_IDLE;
				}
			}
		}
	}
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Handler(uint8_t uart_no)
{
	UART_Type* UARTx;
	volatile uint32_t intr_status;
	volatile uint32_t line_status;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);
	
	/* Check Interrupt ID */
	intr_status = UARTx->IIR;

	/* Line Interrupt */
	if((intr_status & fUnIIR_INTR_BASIC_MASK) == fUnIIR_IID_RCV_LINE_STATUS)
	{
		line_status = UARTx->LSR;					// Line Interrupt clear
	}

	/* Rx Interrupt */
	if((intr_status & fUnIIR_INTR_BASIC_MASK) == fUnIIR_IID_RBR_READY)
	{
		HAL_Uart_Rx_Handler(uart_no);		// Rx Process
	}

	/* Tx Interrupt */
	if((intr_status & fUnIIR_INTR_BASIC_MASK) == fUnIIR_IID_THR_EMPTY)
	{
		HAL_Uart_Tx_Handler(uart_no);		// Tx Process
	}
}

/**
* @brief
* @param   
* @return
*/
uint8_t HAL_Uart_WriteBuffer(uint8_t uart_no, uint8_t *p_data, uint32_t data_count)
{
	UART_Type* UARTx;
	tUART_BUFFER *tp_UartBuffer;
	uint32_t i;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);
	
	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);
	
	/* Wait until past last sequence */
//	while(tp_UartBuffer->TxState != fUART_TX_STATE_IDLE);
	for(i=0; i<0x100000; i++)
	{
		if(tp_UartBuffer->TxBuffer_TailIndex == tp_UartBuffer->TxBuffer_HeadIndex) {break;}
		if(i == 0xFFFFF) {return (fUART_TX_BUFFER_ERROR_WAIT_TIMEOUT);}
	}

	/* Load data to TxBuffer */
	for(i=0; i<data_count; i++)
	{
		tp_UartBuffer->TxBuffer[i] = *(p_data + i);
	}
	tp_UartBuffer->TxBuffer_HeadIndex = data_count;
	tp_UartBuffer->TxBuffer_TailIndex = 0;

	/* Update Tx Status */
	tp_UartBuffer->TxState = fUART_TX_STATE_TRANSMIT;

#ifdef	fUSE_UART_TX_INTERRUPT
	/* Enable Tx Interrupt */
	HAL_Uart_Enable_Tx_Interrupt(UARTx, 1);
#else
	while(1)
	{
		HAL_Uart_Tx_Handler(uart_no);
		if(tp_UartBuffer->TxState == fUART_TX_STATE_IDLE)
		{
			break;
		}
	}
#endif

	return (fUART_TX_BUFFER_SUCCESS);
}

/**
* @brief
* @param   
* @return
*/
uint8_t HAL_Uart_ReadBuffer(uint8_t uart_no, uint8_t *p_status)
{
	tUART_BUFFER *tp_UartBuffer;
	uint8_t receive_data;
	
	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);

	if(tp_UartBuffer->RxBuffer_TailIndex < tp_UartBuffer->RxBuffer_HeadIndex)
	{
		// Load data from Rx Buffer
		receive_data = tp_UartBuffer->RxBuffer[tp_UartBuffer->RxBuffer_TailIndex++];
		*p_status = fUART_RX_BUFFER_SUCCESS;

		// Rx Tail Index Full
		if(tp_UartBuffer->RxBuffer_TailIndex == fUART_MAX_RX_BUFFER)
		{
			tp_UartBuffer->RxBuffer_TailIndex = 0;
		}
		// Receive Done
		if(tp_UartBuffer->RxBuffer_TailIndex == tp_UartBuffer->RxBuffer_HeadIndex)
		{
			tp_UartBuffer->RxBuffer_HeadIndex = 0;
			tp_UartBuffer->RxBuffer_TailIndex = 0;
			tp_UartBuffer->RxState = fUART_RX_STATE_IDLE;
		}
	}
	else if(tp_UartBuffer->RxBuffer_TailIndex > tp_UartBuffer->RxBuffer_HeadIndex)
	{
		// Load data from Rx Buffer
		receive_data = tp_UartBuffer->RxBuffer[tp_UartBuffer->RxBuffer_TailIndex++];
		*p_status = fUART_RX_BUFFER_SUCCESS;

		// Rx Tail Index Full		
		if(tp_UartBuffer->RxBuffer_TailIndex == fUART_MAX_RX_BUFFER)
		{
			tp_UartBuffer->RxBuffer_TailIndex = 0;
		}
		// Receive Done
		if(tp_UartBuffer->RxBuffer_TailIndex == tp_UartBuffer->RxBuffer_HeadIndex)
		{
			tp_UartBuffer->RxBuffer_HeadIndex = 0;
			tp_UartBuffer->RxBuffer_TailIndex = 0;
			tp_UartBuffer->RxState = fUART_RX_STATE_IDLE;
		}
	}
	else
	{
		// Rx Buffer Empty
		receive_data = 0;
		*p_status = fUART_RX_BUFFER_ERROR_EMPTY;
	}

	/* Update Rx Status */
	tp_UartBuffer->RxState = fUART_RX_STATE_RECEIVE;
	
#ifndef	fUSE_UART_RX_INTERRUPT
	HAL_Uart_Rx_Handler(uart_no);
#endif

	return (receive_data);
}

/**
* @brief
* @param   
* @return
*/
void aputc(uint8_t uart_no, uint32_t data)
{
	uint8_t p_data[4];

	if(data > 0xFFFFFF)
	{
		p_data[0] = (data>>24) & 0x0F;
		p_data[1] = (data>>16) & 0x0F;
		p_data[2] = (data>>8) & 0x0F;
		p_data[3] = (data>>0) & 0x0F;
		HAL_Uart_WriteBuffer(uart_no, p_data, 4);
	}
	else if(data > 0xFFFF)
	{
		p_data[0] = (data>>16) & 0x0F;
		p_data[1] = (data>>8) & 0x0F;
		p_data[2] = (data>>0) & 0x0F;
		HAL_Uart_WriteBuffer(uart_no, p_data, 3);
	}
	else if(data > 0xFF)
	{
		p_data[0] = (data>>8) & 0x0F;
		p_data[1] = (data>>0) & 0x0F;
		HAL_Uart_WriteBuffer(uart_no, p_data, 2);
	}
	else
	{
		p_data[0] = (uint8_t)data;
		HAL_Uart_WriteBuffer(uart_no, p_data, 1);
	}
}

/**
* @brief
* @param   
* @return
*/
void aputs(uint8_t uart_no, uint8_t *p_data)
{
	uint32_t i;
	
	for(i=0; ; i++)
	{
		if(*(p_data+i) == 0){break;}
	}

	HAL_Uart_WriteBuffer(uart_no, p_data, i);
}

/**
* @brief
* @param   
* @return
*/
uint8_t agetc(uint8_t uart_no)
{
	uint8_t status;
	uint8_t data;

	data = HAL_Uart_ReadBuffer(uart_no, &status);
	if(status != fUART_RX_BUFFER_SUCCESS){return 0;}

	return data;
}

/**
* @brief
* @param   
* @return
*/
uint8_t agets(uint8_t uart_no, uint8_t *p_data)
{
	uint8_t status;
	uint8_t data_cnt;

	data_cnt = 0;
	while(1)
	{		
		*p_data = HAL_Uart_ReadBuffer(uart_no, &status);
		if(status == fUART_RX_BUFFER_SUCCESS)
		{
			if(*(p_data) == 0x0d)
			{
				break;
			}
			else
			{
				p_data++;
				data_cnt++;
			}
		}
	}
	
	return (data_cnt);
}

/**
* @brief
* @param   
* @return
*/
void aputhex(uint8_t uart_no, uint32_t data)
{
	uint8_t i;
	uint8_t p_data[1];
	uint32_t nibble;

	if(data > 0xFFFFFFF)
	{
		i = 32;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFFFFF)
	{
		i = 28;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFFFF)
	{
		i = 24;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFFF)
	{
		i = 20;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFF)
	{
		i = 16;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
	else if(data > 0xFF)
	{
		i = 12;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
	else if(data > 0xF)
	{
		i = 8;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
	else
	{
		i = 4;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			p_data[0] = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			HAL_Uart_WriteBuffer(uart_no, p_data, 1);
			if(i==0){break;}
		}
	}
}

/**
* @brief
* @param   
* @return
*/
uint8_t agethex(uint8_t uart_no, uint8_t *data)
{
	uint8_t i;
	uint8_t data_cnt;
	
	data_cnt = agets(1,data);
	
	for(i=0; i<data_cnt; i++)
	{
		switch(data[i])
		{
			case 0x30:
				data[i] = 0x0;
				break;
			case 0x31:
				data[i] = 0x1;
				break;
			case 0x32:
				data[i] = 0x2;
				break;
			case 0x33:
				data[i] = 0x3;
				break;
			case 0x34:
				data[i] = 0x4;
				break;
			case 0x35:
				data[i] = 0x5;
				break;
			case 0x36:
				data[i] = 0x6;
				break;
			case 0x37:
				data[i] = 0x7;
				break;
			case 0x38:
				data[i] = 0x8;
				break;
			case 0x39:
				data[i] = 0x9;
				break;
			case 0x61:
				data[i] = 0xa;
				break;
			case 0x62:
				data[i] = 0xb;
				break;
			case 0x63:
				data[i] = 0xc;
				break;
			case 0x64:
				data[i] = 0xd;
				break;
			case 0x65:
				data[i] = 0xe;
				break;
			case 0x66:
				data[i] = 0xf;
				break;
			default :
				data[i] = 0;				
				break;
		}
	}
	
	return data_cnt;
}

/**
* @brief
* @param   
* @return
*/
void RegisterWrite_Handler(void)
{
	uint32_t addr, val;
	uint32_t *p_Register;
	uint8_t data[9];
	uint8_t i, data_cnt;

	aputs(1,"\n\r#########################");
	aputs(1,"\n\r## Register Write/Read ##");
	aputs(1,"\n\r#########################\n\r");
	
	while(1)
	{
		addr = 0; val = 0;
		
		/* Input Target Address */
		aputs(1,"\n\r");		
		aputs(1,">>TargetAddr : 0x");		
		data_cnt = agethex(1, data);
		for(i=0; i<data_cnt; i++)
		{
			addr = (addr<<4);
			addr |= data[i];
		}
		aputhex(1, addr);
		aputs(1," => OK\n\r");
		
		/* Input Value */
		aputs(1,">>WriteValue : 0x");		
		data_cnt = agethex(1, data);
		for(i=0; i<data_cnt; i++)
		{
			val = (val<<4);
			val |= data[i];
		}
		aputhex(1, val);
		aputs(1," => OK\n\r");		

		/* Register Write/Read */
		p_Register = (uint32_t *)addr;
		if(data_cnt >0)
		{
			*p_Register = val;
		}
		aputs(1,"[0x");
		aputhex(1,addr);
		aputs(1,"] = 0x");		
		aputhex(1, *p_Register);
		aputs(1," [OK]\n\r");
	}
}

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
