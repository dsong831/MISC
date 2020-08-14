/**********************************************************************//**
* @file				A31T21x_hal_usart1n.c
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			29. JUNE. 2020
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements, 
*modifications, and improvements to ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and 
*ABOV assumes no liability for application assistance or the design of purchasers¡¯ products. No license, 
*express or implied, to any intellectual property rights is granted by ABOV herein. 
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications. 
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners. 
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
**********************************************************************/


/* Includes ------------------------------------------------------------ */
#include "A31T21x_hal_usart1n.h"


/* Private variable -------------------------------------------------------- */
uint8_t u8UstDummy;

/* Private Types --------------------------------------------------------------- */
ustRingBuffer_Type	tx10_RingBuffer;
ustRingBuffer_Type	tx11_RingBuffer;
ustRingBuffer_Type	rx10_RingBuffer;
ustRingBuffer_Type	rx11_RingBuffer;


/* Public Functions -------------------------------------------------------- */
/**********************************************************************//**
* @brief						Read a single data from USART peripheral (Polling mode)
* @param[in]	USARTn	USART peripheral selected, should be:
*											- USART10 : USART10 peripheral
*											- USART11 : USART11 peripheral
* @return					Received data
 **********************************************************************/
uint8_t HAL_USART_ReadBuffer(USART_Type* USARTn)
{
	u8UstDummy = USARTn->DR;		// Dummy read

	while(!(USARTn->ST & USART_ST_RXC));
	return USARTn->DR;
}

/**********************************************************************//**
 * @brief						Write a single data through USART peripheral (Polling mode)
 * @param[in]	USARTn		USART peripheral selected, should be:
 *											- USART0 : USART10 peripheral
 *											- USART1 : USART11 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_USART_WriteBuffer(USART_Type* USARTn, uint8_t tx_data)
{
	while(!(USARTn->ST & USART_ST_DRE));	// Wait for current transmit complete
	USARTn->DR = tx_data;
}

/**********************************************************************//**
 * @brief						Initializes the USARTn peripheral according to the specified
 *											parameters in the USART_ConfigStruct.
 * @param[in]	USARTn			USART peripheral selected, should be:
 *											- USART10 : USART10 peripheral
 *											- USART11 : USART11 peripheral
 * @param[in]	USARTModeConfig		USART mode selected, should be:
 *											- USART_UART_MODE
 *											- USART_USART_MODE
 *											- USART_SPI_MODE
 * @param[in]	USARTConfigStruct		Pointer to a USART_CFG_Type structure
 *											that contains the configuration information for the
 *											specified USART peripheral.
 *											- tBaudRate / tDataLength / tStopbit / tParity
 * @param[in]	USARTIntStruct		USART Interrupt selected, should be:
 *											- UART_INT_POLLING
 *											- USART_INT_RXCIE
 *											- USART_INT_DRIE
 *											- USART_INT_RXCIE_DRIE
 * @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_USART_Init(USART_Type *USARTn, USART_MODE_Type USARTModeConfig, USART_CFG_Type *USARTConfigStruct, USART_INTCFG_Type USARTIntConfig)
{
	uint32_t u32Reg = 0;

	/* USART Block Reset */
	if(USARTn == USART10)
	{
		SCU->PER2 &= ~(1<<8);
		SCU->PCER2 &= ~(1<<8);
		SCU->PER2 |= (1<<8);
		SCU->PCER2 |= (1<<8);		
	}
	else if(USARTn == USART11)
	{
		SCU->PER2 &= ~(1<<9);
		SCU->PCER2 &= ~(1<<9);
		SCU->PER2 |= (1<<9);
		SCU->PCER2 |= (1<<9);
	}
	else
	{
		return HAL_ERROR;
	}	

	/* USART Register Initialize */
	USARTn->CR1 = 0;
	USARTn->CR2 = 0;
	u8UstDummy = USARTn->ST;

	/* UART MODE */
	if(USARTModeConfig == USART_UART_MODE)
	{
		/* USART RingBuffer Initialize */
		HAL_USART_RingBuffer_Init(USARTn);

		/* Set BaudRate */
		HAL_USART_BaudrateSet(USARTn, USART_UART_MODE, (USARTConfigStruct->tBaudRate));

		/* Set UART Mode */
		u32Reg &= ~(3<<14);

		/* Set Parity */
		if(USARTConfigStruct->tParity == USART_PARITY_NONE)
		{
			u32Reg &= ~(3<<12);
		}
		else
		{
			switch(USARTConfigStruct->tParity)
			{
				case USART_PARITY_EVEN :
					u32Reg |= (2<<12);
					break;
				case USART_PARITY_ODD :
					u32Reg |= (3<<12);
					break;
				default :
					u32Reg &= ~(3<<12);
					break;
			}
		}

		/* Set DataLength */
		switch(USARTConfigStruct->tDataLength)
		{
			case USART_DATALEN_5 :
				u32Reg &= ~(7<<9);
				break;
			case USART_DATALEN_6 :
				u32Reg |= (1<<9);
				break;
			case USART_DATALEN_7 :
				u32Reg |= (2<<9);
				break;
			case USART_DATALEN_8 :
				u32Reg |= (3<<9);
				break;
			case USART_DATALEN_9 :
				u32Reg |= (7<<9);
				break;
			default :
				u32Reg |= (3<<9);
				break;
		}
		USARTn->CR1 = u32Reg | (3<<0);	// Set Mode/Parity/DataLength/TRX

		/* Set StopBit */
		switch(USARTConfigStruct->tStopbit)
		{
			case USART_STOPBIT_2 :
				USARTn->CR2 |= (1<<2);
				break;
			case USART_STOPBIT_1 :
			default:
				USARTn->CR2 &= ~(1<<2);
				break;
		}
		USARTn->CR2 |= (1<<9);

		/* UART Interrupt Setting */
		u32Reg = 0;
		switch(USARTIntConfig)
		{
			case USART_INT_POLLING:
				u32Reg = 0;
				break;
			case USART_INT_RXCIE:
				u32Reg |= (1<<3);
				break;
			case USART_INT_TXCIE:
				u32Reg |= (1<<4);
				break;
			case USART_INT_RXCIE_TXCIE:
				u32Reg |= (1<<3) | (1<<4);
				break;
			default :
				u32Reg = 0;
				break;
		}
		USARTn->CR1 |= u32Reg;		// Set interrupt
	}
	else if(USARTModeConfig == USART_SPI_MODE)
	{
		
	}
	else
	{
		
	}

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Determines best dividers to get a target clock rate
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @param[in]	USARTModeConfig		USART mode selected, should be:
 *											- USART_UART_MODE
 *											- USART_USART_MODE
 *											- USART_SPI_MODE
 * @param[in]	baudrate Desired USART baud rate.
 * @return				None
 **********************************************************************/
void HAL_USART_BaudrateSet(USART_Type *USARTn, USART_MODE_Type USARTModeConfig, uint32_t baudrate)
{
	uint32_t numerator;
	uint32_t denominator; 
	uint32_t bdr;

	if(USARTModeConfig == USART_UART_MODE)
	{
		//------------------------------------------------------------------------------
		// Baud Rate = PCLK / (16(USTnBD+1))	@ Normal Mode
		// Baud Rate = PCLK / (8(USTnBD+1))		@ Double Speed Mode
		//------------------------------------------------------------------------------
		numerator = SystemPeriClock;
		if(USARTn->CR2 & (1<<8))
		{
			denominator = 8 * baudrate;		// Double Speed Mode
		}
		else
		{
			denominator = 16 * baudrate;	// Normal Mode
		}

		bdr = (numerator / denominator) - 1;

		/* Baudrate Error Check */
		if(bdr <= 3)
		{
			// Error then to change speed mode
			USARTn->CR2 |= (1<<8);
			denominator = 8 * baudrate;	// Double Speed Mode
			bdr = (numerator / denominator) - 1;
		}

		USARTn->BDR= (uint16_t)(bdr&0xffff);
	}
	else if(USARTModeConfig == USART_SPI_MODE)
	{
		
	}
	else
	{

	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/*********************************************************************//**
 * @brief						Initialize RingBuffer Parameters
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_RingBuffer_Init(USART_Type *USARTn)
{
	if(USARTn == USART10)
	{
		tx10_RingBuffer.HeadPtr = 0;
		tx10_RingBuffer.TailPtr = 0;
		tx10_RingBuffer.State = USART_UART_TX_IDLE;
		
		rx10_RingBuffer.HeadPtr = 0;
		rx10_RingBuffer.TailPtr = 0;
		rx10_RingBuffer.State = USART_UART_RX_IDLE;
	}
	else if(USARTn == USART11)
	{
		tx11_RingBuffer.HeadPtr = 0;
		tx11_RingBuffer.TailPtr = 0;
		tx11_RingBuffer.State = USART_UART_TX_IDLE;

		rx11_RingBuffer.HeadPtr = 0;
		rx11_RingBuffer.TailPtr = 0;
		rx11_RingBuffer.State = USART_UART_RX_IDLE;
	}
}

/*********************************************************************//**
 * @brief						USART Transmit Data (Interrupt mode)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @param[in]	tx_data		USART Transmit Data
 * @return				None
 **********************************************************************/
void HAL_USART_TransmitData(USART_Type *USARTn, uint8_t tx_data)
{
	USARTn->CR1 &= ~(1<<4);		// Disable txc interrupt
	
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(tx10_RingBuffer.HeadPtr == tx10_RingBuffer.TailPtr)
		{
			if(tx10_RingBuffer.State == USART_UART_TX_IDLE)
			{
				HAL_USART_WriteBuffer(USARTn, tx_data);		// First data to transmit
				tx10_RingBuffer.State = USART_UART_TX_BUSY;
			}
			else
			{
				tx10_RingBuffer.Buffer[tx10_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(tx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					tx10_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			tx10_RingBuffer.Buffer[tx10_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(tx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				tx10_RingBuffer.HeadPtr = 0;
			}
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(tx11_RingBuffer.HeadPtr == tx11_RingBuffer.TailPtr)
		{
			if(tx11_RingBuffer.State == USART_UART_TX_IDLE)
			{
				HAL_USART_WriteBuffer(USARTn, tx_data);		// First data to transmit
				tx11_RingBuffer.State = USART_UART_TX_BUSY;
			}
			else
			{
				tx11_RingBuffer.Buffer[tx11_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(tx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					tx11_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			tx11_RingBuffer.Buffer[tx11_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(tx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				tx11_RingBuffer.HeadPtr = 0;
			}
		}
	}

	USARTn->CR1 |= (1<<4);		// Enable txc interrupt
}

/*********************************************************************//**
 * @brief						USART Receive Data (Interrupt mode)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				Received data
 **********************************************************************/
int8_t HAL_USART_ReceiveData(USART_Type *USARTn)
{
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(rx10_RingBuffer.HeadPtr != rx10_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(rx10_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
			{
				rx10_RingBuffer.TailPtr = 0;
				return rx10_RingBuffer.Buffer[rx10_RingBuffer.TailPtr];
			}
			return rx10_RingBuffer.Buffer[rx10_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				rx10_RingBuffer.State = USART_UART_RX_IDLE;
				return -1;
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(rx11_RingBuffer.HeadPtr != rx11_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(rx11_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
			{
				rx11_RingBuffer.TailPtr = 0;
				return rx11_RingBuffer.Buffer[rx11_RingBuffer.TailPtr];
			}
			return rx11_RingBuffer.Buffer[rx11_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				rx11_RingBuffer.State = USART_UART_RX_IDLE;
				return -1;
		}
	}
	/* No Unit */
	else
	{
		return -1;
	}
}

/*********************************************************************//**
 * @brief						USART Handler (Interrupt Handler)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_Handler(USART_Type *USARTn)
{
	volatile uint32_t int_status;
	volatile uint32_t line_error;
	
	int_status = USARTn->ST;

	/* Line Interrupt */
	if(int_status & 0x07)
	{
		line_error = USARTn->ST;									// Clear line Error Interrupt
	}
	
	/* Rx Interrupt */
	if(int_status & 0x20)
	{
		HAL_USART_RX_Handler(USARTn);		// Rx Process
	}
	
	/* Tx Interrupt */
	if(int_status & 0x40)
	{
		HAL_USART_TX_Handler(USARTn);		// Tx Process
		USARTn->ST &= ~(0x80);										// Clear TXC Flag
	}
}

/*********************************************************************//**
 * @brief						USART Transmit Handler (Interrupt Handler)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_TX_Handler(USART_Type *USARTn)
{
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(tx10_RingBuffer.HeadPtr != tx10_RingBuffer.TailPtr)
		{
			if(USARTn->ST & 0x80)
			{
				USARTn->DR = tx10_RingBuffer.Buffer[tx10_RingBuffer.TailPtr++];
				// Check buffer max length
				if(tx10_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
				{
					tx10_RingBuffer.TailPtr = 0;
				}
			}
		}
		else
		{
				tx10_RingBuffer.State = USART_UART_TX_IDLE;
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(tx11_RingBuffer.HeadPtr != tx11_RingBuffer.TailPtr)
		{
			if(USARTn->ST & 0x80)
			{
				USARTn->DR = tx11_RingBuffer.Buffer[tx11_RingBuffer.TailPtr++];
				// Check buffer max length
				if(tx11_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
				{
					tx11_RingBuffer.TailPtr = 0;
				}
			}
		}
		else
		{
				tx11_RingBuffer.State = USART_UART_TX_IDLE;
		}
	}
}

/*********************************************************************//**
 * @brief						USART Receive Handler (Interrupt Handler)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_RX_Handler(USART_Type *USARTn)
{
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(rx10_RingBuffer.HeadPtr == rx10_RingBuffer.TailPtr)
		{
			if(rx10_RingBuffer.State == USART_UART_RX_IDLE)
			{
				rx10_RingBuffer.Buffer[rx10_RingBuffer.HeadPtr++] = USARTn->DR;		// First data to receive
				rx10_RingBuffer.State = USART_UART_RX_BUSY;
				// Check buffer max length
				if(rx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx10_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				rx10_RingBuffer.Buffer[rx10_RingBuffer.HeadPtr++] = USARTn->DR;
				// Check buffer max length
				if(rx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx10_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			rx10_RingBuffer.Buffer[rx10_RingBuffer.HeadPtr++] = USARTn->DR;
			// Check buffer max length
			if(rx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				rx10_RingBuffer.HeadPtr = 0;
			}
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(rx11_RingBuffer.HeadPtr == rx11_RingBuffer.TailPtr)
		{
			if(rx11_RingBuffer.State == USART_UART_RX_IDLE)
			{
				rx11_RingBuffer.Buffer[rx11_RingBuffer.HeadPtr++] = USARTn->DR;		// First data to receive
				rx11_RingBuffer.State = USART_UART_RX_BUSY;
				// Check buffer max length
				if(rx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx11_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				rx11_RingBuffer.Buffer[rx11_RingBuffer.HeadPtr++] = USARTn->DR;
				// Check buffer max length
				if(rx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx11_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			rx11_RingBuffer.Buffer[rx11_RingBuffer.HeadPtr++] = USARTn->DR;
			// Check buffer max length
			if(rx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				rx11_RingBuffer.HeadPtr = 0;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/* --------------------------------- End Of File ------------------------------ */
