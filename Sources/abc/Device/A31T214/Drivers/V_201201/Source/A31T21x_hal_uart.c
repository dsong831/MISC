/**********************************************************************//**
* @file				A31T21x_hal_uart.c
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
#include "A31T21x_hal_uart.h"


/* Private variable -------------------------------------------------------- */
/* Private Types --------------------------------------------------------------- */
UART_RING_BUFFER_T RingBuff0;	// For UART0 Interrupt
UART_RING_BUFFER_T RingBuff1;	// For UART1 Interrupt
/* Public Functions -------------------------------------------------------- */
/**********************************************************************//**
 * @brief						Initializes the UARTx peripheral according to the specified
 *											parameters in the UART_ConfigStruct.
 * @param[in]	UARTn			UART peripheral selected, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[in]	UARTConfigStruct		Pointer to a UART_CFG_Type structure
 *											that contains the configuration information for the
 *											specified UART peripheral.
 *											- tBaudRate / tDataLength / tStopbit / tParity
 * @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_UART_Init(UART_Type *UARTn, UART_CFG_Type *UARTConfigStruct)
{
	uint8_t u8Reg;

	/* UART Block Reset */
	if(UARTn == UART0)
	{
		SCU->PER2 &= ~(1<<12);
		SCU->PCER2 &= ~(1<<12);
		SCU->PER2 |= (1<<12);
		SCU->PCER2 |= (1<<12);		
	}
	else if(UARTn == UART1)
	{
		SCU->PER2 &= ~(1<<13);
		SCU->PCER2 &= ~(1<<13);
		SCU->PER2 |= (1<<13);
		SCU->PCER2 |= (1<<13);
	}
	else
	{
		return HAL_ERROR;
	}	

	/* Dummy data reading */
	while(UARTn->LSR & UART_LSR_RDR)
	{
		u8Reg = UARTn->RBR;
	}
	while(!(UARTn->LSR & UART_LSR_THRE));

	/* UART Register Initialize */
	UARTn->IER = 0;
	UARTn->LCR = 0;
	UARTn->DCR = 0;
	u8Reg = UARTn->LSR;
	u8Reg = UARTn->IIR;

	/* Set BaudRate */
	HAL_UART_BaudrateSet(UARTn, (UARTConfigStruct->tBaudRate));

	/* Set DataLength */
	u8Reg = 0;
	switch(UARTConfigStruct->tDataLength)
	{
		case UART_DATALEN_5 :
			u8Reg |= UART_LCR_DLEN_5;
			break;
		case UART_DATALEN_6 :
			u8Reg |= UART_LCR_DLEN_6;
			break;
		case UART_DATALEN_7 :
			u8Reg |= UART_LCR_DLEN_7;
			break;
		case UART_DATALEN_8 :
			u8Reg |= UART_LCR_DLEN_8;
			break;
		default :
			u8Reg |= UART_LCR_DLEN_8;
			break;
	}

	/* Set StopBit */
	switch(UARTConfigStruct->tStopbit)
	{
		case UART_STOPBIT_2 :
			u8Reg |= UART_LCR_STOPBIT;
			break;
		case UART_STOPBIT_1 :
		default:
			u8Reg &= ~UART_LCR_STOPBIT;
			break;
	}

	/* Set Parity */
	if(UARTConfigStruct->tParity == UART_PARITY_NONE)
	{
		u8Reg &= ~(0x38);
	}
	else
	{
		u8Reg |= UART_LCR_PEN;
		switch(UARTConfigStruct->tParity)
		{
			case UART_PARITY_ODD :
				u8Reg &= ~UART_LCR_PARITY;
				break;

			case UART_PARITY_EVEN :
				u8Reg |= UART_LCR_PARITY;
				break;

			case UART_PARITY_SP_1 :
				u8Reg |= UART_LCR_STICKP;
				break;

			case UART_PARITY_SP_0 :
				u8Reg &= ~UART_LCR_STICKP;
				break;

			default :
				u8Reg &= ~(0x38);
				break;
		}
	}
	UARTn->LCR = (uint8_t)(u8Reg & UART_LCR_BITMASK);		// Set Line Control Register (DataLen/StopBit/Parity)

	return HAL_OK;
}

/**********************************************************************//**
 * @brief						De-Initializes the UARTx peripheral registers to their
 *											default reset values.
 * @param[in]	UARTn			UART peripheral selected, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_UART_DeInit(UART_Type *UARTn)
{
	if(UARTn == UART0)
	{
		SCU->PER2 &= ~(1<<12);
		SCU->PCER2 &= ~(1<<12);
	}
	else if(UARTn == UART1)
	{
		SCU->PER2 &= ~(1<<13);
		SCU->PCER2 &= ~(1<<13);
	}
	else
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

/**********************************************************************//**
 * @brief						Initializes the UARTx peripheral according to the specified
 *											parameters in the UART_ConfigStruct.
 * @param[in]	UARTn			UART peripheral selected, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[in]	UARTIntStruct		UART Interrupt selected, should be:
 *											- UART_INT_POLLING
 *											- UART_INT_RXE
 *											- UART_INT_THR
 *											- UART_INT_RXE_THR
 *											- UART_INT_RLS
 *											- UART_INT_RXE_RLS
 *											- UART_INT_THR_RLS
 *											- UART_INT_RXE_THR_RLS
* @param[in]	int_en Interrupt Enable
*											- DISABLE: wdt disable
*											- ENABLE: wdt enable 
 * @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_UART_ConfigInterrupt(UART_Type *UARTn, UART_INTCFG_Type UARTIntConfig, EN_DIS_Type int_en)
{
	uint8_t u8Reg;

	/* UART Interrupt Setting */
	u8Reg = 0;
	switch(UARTIntConfig)
	{
		case UART_INT_POLLING:
			u8Reg = 0;
			break;
		case UART_INT_RXE:
			u8Reg = UART_IER_DRIE;
			break;
		case UART_INT_THRE:
			u8Reg = UART_IER_THREIE;
			break;
		case UART_INT_RXE_THRE:
			u8Reg = UART_IER_DRIE | UART_IER_THREIE;
			break;
		case UART_INT_RLS:
			u8Reg = UART_IER_RLSIE;
			break;
		case UART_INT_RXE_RLS:
			u8Reg = UART_IER_DRIE | UART_IER_RLSIE;
			break;
		case UART_INT_THRE_RLS:
			u8Reg = UART_IER_THREIE | UART_IER_RLSIE;
			break;
		case UART_INT_RXE_THRE_RLS:
			u8Reg = UART_IER_DRIE | UART_IER_THREIE | UART_IER_RLSIE;
			break;
		default :
			u8Reg = 0;
			break;
	}
	
	if(int_en == ENABLE)
	{
		UARTn->IER |= (uint8_t)(u8Reg & UART_IER_BITMASK);				// Set interrupt enable register

		/* Ring Buffer Initialization */
		if(UARTn == UART0)
		{
			RingBuff0.TxIntStat = RESET;
			RingBuff0.rx_head=0;
			RingBuff0.rx_tail=0;
			RingBuff0.tx_head=0;
			RingBuff0.tx_tail=0;
		}
		else
		{
			RingBuff1.TxIntStat = RESET;
			RingBuff1.rx_head=0;
			RingBuff1.rx_tail=0;
			RingBuff1.tx_head=0;
			RingBuff1.tx_tail=0;
		}
	}
	else
	{
		UARTn->IER &= (uint8_t)(~(u8Reg) & UART_IER_BITMASK);	// Clear interrupt enable register
	}

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Determines best dividers to get a target clock rate
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @param[in]	baudrate Desired UART baud rate.
 * @return				None
 **********************************************************************/
void HAL_UART_BaudrateSet(UART_Type *UARTn, uint32_t baudrate)
{
	uint32_t numerator;
	uint32_t denominator; 
	uint32_t bdr, bfr;
	uint32_t fd;

	//------------------------------------------------------------------------------
	// numerator & denominator 
	// 
	// bdr = (UartBaseClock) / (16 * baudrate) 
	//------------------------------------------------------------------------------
	numerator = SystemPeriClock/2;
	denominator = 16 * baudrate; 

	bdr = numerator / denominator;

	//------------------------------------------------------------------------------
	//	fraction = numerator - bdr x denominator
	//
	//	bfr = INT (fraction * 256) = ((f x d ) << 8) / d 
	//------------------------------------------------------------------------------

	fd = numerator - (bdr * denominator); 
	bfr = (fd * 256) / denominator; 
	UARTn->BDR = (uint16_t)(bdr&0xffff);
	UARTn->BFR = (uint8_t)(bfr&0xff);
}

/**********************************************************************//**
 * @brief						Write a single data through UART peripheral (Polling mode)
 * @param[in]	UARTn		UART peripheral selected, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_UART_TransmitByte(UART_Type* UARTn, uint8_t tx_data)
{
	UARTn->THR = tx_data;
}

/**********************************************************************//**
* @brief						Read a single data from UART peripheral (Polling mode)
* @param[in]	UARTn	UART peripheral selected, should be:
*											- UART0 : UART0 peripheral
*											- UART1 : UART1 peripheral
* @return					Received data
 **********************************************************************/
uint8_t HAL_UART_ReceiveByte(UART_Type* UARTn)
{
	return (UARTn->RBR);
}

/**********************************************************************//**
 * @brief						Send a block of data via UART peripheral
 * @param[in]	UARTn	Selected UART peripheral used to send data, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[in]	txbuf 	Pointer to Transmit buffer
 * @param[in]	buflen 	Length of Transmit buffer
 * @param[in]	flag 	Flag used in  UART transfer, should be
 *											- NONE_BLOCKING
 *											- BLOCKING
 * @return				Number of bytes sent.
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t HAL_UART_Transmit(UART_Type* UARTn, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
{
	uint32_t bToSend, bSent, timeOut;
	uint8_t *pChar = txbuf;

	bToSend = buflen;

	/* Blocking mode */
	if(flag == BLOCKING)
	{
		bSent = 0;

		while(bToSend)
		{
			timeOut = UART_BLOCKING_TIMEOUT;
			// Wait for THR empty with timeout
			while(!(UARTn->LSR & UART_LSR_THRE))
			{
				if(timeOut == 0)
				{
					break;
				}
				timeOut--;
			}
			// Time out!
			if(timeOut == 0)
			{
				break;
			}
			else
			{
				HAL_UART_TransmitByte(UARTn, (*pChar++));
				bToSend--;
				bSent++;
			}
		}
	}

	/* Non-blocking mode */
	else
	{
		bSent = 0;

		while(bToSend)
		{
			if((UARTn->LSR & UART_LSR_THRE))
			{
				HAL_UART_TransmitByte(UARTn, (*pChar++));
				bToSend--;
				bSent++;
			}
			else
			{
				break;
			}
		}
	}

	return bSent;
}

/*********************************************************************//**
 * @brief						Receive a block of data via UART peripheral
 * @param[in]	UARTn	Selected UART peripheral used to send data, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[out]	rxbuf 	Pointer to Received buffer
 * @param[in]	buflen 	Length of Received buffer
 * @param[in]	flag 	Flag used in  UART transfer, should be
 *											- NONE_BLOCKING
 *											- BLOCKING
 * @return				Number of bytes received
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t HAL_UART_Receive(UART_Type *UARTn, uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
{
	uint32_t bToRecv, bRecv, timeOut;
	uint8_t *pChar = rxbuf;

	bToRecv = buflen;

	/* Blocking mode */
	if(flag == BLOCKING)
	{
		bRecv = 0;

		while(bToRecv)
		{
			timeOut = UART_BLOCKING_TIMEOUT;
			// Wait for RDR with timeout
			while(!(UARTn->LSR & UART_LSR_RDR))
			{
				if(timeOut == 0)
				{
					break;
				}
				timeOut--;
			}
			// Time out!
			if(timeOut == 0)
			{
				break;
			}
			else
			{
				// Get data from the buffer
				(*pChar++) = HAL_UART_ReceiveByte(UARTn);
				bToRecv--;
				bRecv++;
			}
		}
	}

	/* Non-blocking mode */
	else
	{
		bRecv = 0;

		while(bToRecv)
		{
			if((UARTn->LSR & UART_LSR_RDR))
			{
				(*pChar++) = HAL_UART_ReceiveByte(UARTn);
				bRecv++;
				bToRecv--;
			}
			else
			{
				break;
			}
		}
	}

	return bRecv;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// UART Interrupt APIs
////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************
 * @brief							UART transmit function for interrupt mode (using ring buffers)
 * @param[in]		UARTPort	Selected UART peripheral used to send data.
 * @param[out]	txbuf Pointer to Transmit buffer
 * @param[in]		buflen Length of Transmit buffer
 * @return					Number of bytes actually sent to the ring buffer
 **********************************************************************/
uint32_t UARTSend(UART_Type *UARTn, uint8_t *txbuf, uint8_t buflen)
{
	uint8_t *tx_data = (uint8_t *)txbuf;
	uint32_t tx_cnt = 0;

	/* Temporarily lock out UART transmit interrupts during this
	read so the UART transmit interrupt won't cause problems
	with the index values */
	UARTn->IER &= ~(UART_IER_THREIE);

	/* UART0 */
	if(UARTn == UART0)
	{
		/* Loop until transmit run buffer is full or until n_bytes
		expires */
		while((buflen > 0) && (!__BUF_IS_FULL(RingBuff0.tx_head, RingBuff0.tx_tail)))
		{
			/* Write data from buffer into ring buffer */
			RingBuff0.tx[RingBuff0.tx_head] = *tx_data;
			tx_data++;

			/* Increment head pointer */
			__BUF_INCR(RingBuff0.tx_head);

			/* Increment data count and decrement buffer size count */
			tx_cnt++;
			buflen--;
		}

		/*
		* Check if current Tx interrupt enable is reset,
		* that means the Tx interrupt must be re-enabled
		* due to call UART_IntTransmit() function to trigger
		* this interrupt type
		*/
		if(RingBuff0.TxIntStat == RESET)
		{
			UART_IntTransmit(UARTn);
		}
		else
		{
			UARTn->IER |= (UART_IER_THREIE);
		}
	}
	
	/* UART1 */
	else
	{
		/* Loop until transmit run buffer is full or until n_bytes
		expires */
		while((buflen > 0) && (!__BUF_IS_FULL(RingBuff1.tx_head, RingBuff1.tx_tail)))
		{
			/* Write data from buffer into ring buffer */
			RingBuff1.tx[RingBuff1.tx_head] = *tx_data;
			tx_data++;

			/* Increment head pointer */
			__BUF_INCR(RingBuff1.tx_head);

			/* Increment data count and decrement buffer size count */
			tx_cnt++;
			buflen--;
		}

		/*
		* Check if current Tx interrupt enable is reset,
		* that means the Tx interrupt must be re-enabled
		* due to call UART_IntTransmit() function to trigger
		* this interrupt type
		*/
		if(RingBuff1.TxIntStat == RESET)
		{
			UART_IntTransmit(UARTn);
		}
		else
		{
			UARTn->IER |= (UART_IER_THREIE);
		}
	}

	return tx_cnt;
}

/*********************************************************************
 * @brief							UART read function for interrupt mode (using ring buffers)
 * @param[in]		UARTPort	Selected UART peripheral used to send data,
 * @param[out]	rxbuf Pointer to Received buffer
 * @param[in]		buflen Length of Received buffer
 * @return					Number of bytes actually read from the ring buffer
 **********************************************************************/
uint32_t UARTReceive(UART_Type *UARTn, uint8_t *rxbuf, uint8_t buflen)
{
	uint8_t *rx_data = (uint8_t *)rxbuf;
	uint32_t rx_cnt = 0;

	/* Temporarily lock out UART receive interrupts during this
	read so the UART receive interrupt won't cause problems
	with the index values */
	UARTn->IER &= ~(UART_IER_DRIE);

	/* UART0 */
	if(UARTn == UART0)
	{
		/* Loop until receive buffer ring is empty or
		until max_bytes expires */
		while((buflen > 0) && (!(__BUF_IS_EMPTY(RingBuff0.rx_head, RingBuff0.rx_tail))))
		{
			/* Read data from ring buffer into user buffer */
			*rx_data = RingBuff0.rx[RingBuff0.rx_tail];
			rx_data++;

			/* Update tail pointer */
			__BUF_INCR(RingBuff0.rx_tail);

			/* Increment data count and decrement buffer size count */
			rx_cnt++;
			buflen--;
		}
	}

	/* UART1 */
	else
	{
		/* Loop until receive buffer ring is empty or
		until max_bytes expires */
		while((buflen > 0) && (!(__BUF_IS_EMPTY(RingBuff1.rx_head, RingBuff1.rx_tail))))
		{
			/* Read data from ring buffer into user buffer */
			*rx_data = RingBuff1.rx[RingBuff1.rx_tail];
			rx_data++;

			/* Update tail pointer */
			__BUF_INCR(RingBuff1.rx_tail);

			/* Increment data count and decrement buffer size count */
			rx_cnt++;
			buflen--;
		}
	}

	/* Re-enable UART interrupts */
	UARTn->IER |= (UART_IER_DRIE);

	return rx_cnt;
}

/*********************************************************************
 * @brief							UART interrupt handler sub-routine
 * @param[in]		UARTPort	Selected UART peripheral used to send data.
 * @return					None
 **********************************************************************/
void UART_IRQHandler_IT(UART_Type *UARTn)
{
	volatile uint32_t int_status;
	volatile uint32_t line_error;

	int_status = UARTn->IIR;
	int_status = (int_status & UART_IIR_BITMASK);

	/* Line Interrupt */
	if(int_status == UART_IIR_IID_RLS)
	{
		line_error = UARTn->LSR;		// Clear line Error Interrupt
		line_error &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI);
		if(line_error)
		{
			UART_IntErr(line_error);
		}
	}

	/* Rx Interrupt */
	if(int_status == UART_IIR_IID_RDA)
	{
		UART_IntReceive(UARTn);
	}

	/* Tx Interrupt */
	if(int_status == UART_IIR_IID_THRE)
	{
		UART_IntTransmit(UARTn);
	}
}

/*********************************************************************//**
 * @brief						UART Line Status Error
 * @param[in]	error_code	UART Line Status Error Type
 * @return				None
 **********************************************************************/
void UART_IntErr(uint32_t error_code)
{
	uint32_t data, timeOut;

	/* User specific code */
	data = error_code; data = data;
	timeOut = UART_BLOCKING_TIMEOUT;
	while(1)
	{
		timeOut--;
		if(timeOut == 0)
		{
			break;
		}
	}
}

/********************************************************************//**
 * @brief						UART receive function (ring buffer used)
 * @param[in]	None
 * @return				None
 *********************************************************************/
void UART_IntReceive(UART_Type *UARTn)
{
	uint8_t receive_buffer;
	uint32_t receive_count;

	while(1)
	{
		// Rx Buffer read
		receive_count = HAL_UART_Receive((UART_Type *)UARTn, &receive_buffer, 1, NONE_BLOCKING);

		// If data received
		if (receive_count)
		{
			/* UART0 */
			if(UARTn == UART0)
			{
				if(!__BUF_IS_FULL(RingBuff0.rx_head,RingBuff0.rx_tail))
				{
					RingBuff0.rx[RingBuff0.rx_head] = receive_buffer;
					__BUF_INCR(RingBuff0.rx_head);
				}
			}
			/* UART1 */
			else
			{
				if(!__BUF_IS_FULL(RingBuff1.rx_head,RingBuff1.rx_tail))
				{
					RingBuff1.rx[RingBuff1.rx_head] = receive_buffer;
					__BUF_INCR(RingBuff1.rx_head);
				}
			}
		}
		// No more data
		else
		{
			break;
		}
	}
}

/********************************************************************//**
 * @brief						UART transmit function (ring buffer used)
 * @param[in]	None
 * @return				None
 *********************************************************************/
void UART_IntTransmit(UART_Type *UARTn)
{
	// Disable THRE interrupt
	UARTn->IER &= ~(UART_IER_THREIE);

	/* Wait until THR empty */
	while(!(UARTn->LSR & UART_LSR_TEMT));

	/* UART0 */
	if(UARTn == UART0)
	{
		while(!__BUF_IS_EMPTY(RingBuff0.tx_head, RingBuff0.tx_tail))
		{
			/* Move a piece of data into the transmit FIFO */
			if(HAL_UART_Transmit((UART_Type *)UARTn, (uint8_t *)&RingBuff0.tx[RingBuff0.tx_tail], 1, NONE_BLOCKING))
			{
				/* Update transmit ring FIFO tail pointer */
				__BUF_INCR(RingBuff0.tx_tail);
			}
			else
			{
				break;
			}
		}

		/* If there is no more data to send, disable the transmit
		interrupt - else enable it or keep it enabled */
		if(__BUF_IS_EMPTY(RingBuff0.tx_head, RingBuff0.tx_tail))
		{
			// Reset Tx Interrupt state
			UARTn->IER &= ~(UART_IER_THREIE);
			RingBuff0.TxIntStat = RESET;
		}
		else
		{
			// Set Tx Interrupt state
			UARTn->IER |= (UART_IER_THREIE);
			RingBuff0.TxIntStat = SET;
		}
	}

	/* UART1 */
	else
	{
		while(!__BUF_IS_EMPTY(RingBuff1.tx_head, RingBuff1.tx_tail))
		{
			/* Move a piece of data into the transmit FIFO */
			if(HAL_UART_Transmit((UART_Type *)UARTn, (uint8_t *)&RingBuff1.tx[RingBuff1.tx_tail], 1, NONE_BLOCKING))
			{
				/* Update transmit ring FIFO tail pointer */
				__BUF_INCR(RingBuff1.tx_tail);
			}
			else
			{
				break;
			}
		}

		/* If there is no more data to send, disable the transmit
		interrupt - else enable it or keep it enabled */
		if(__BUF_IS_EMPTY(RingBuff1.tx_head, RingBuff1.tx_tail))
		{
			// Reset Tx Interrupt state
			UARTn->IER &= ~(UART_IER_THREIE);
			RingBuff1.TxIntStat = RESET;
		}
		else
		{
			// Set Tx Interrupt state
			UARTn->IER |= (UART_IER_THREIE);
			RingBuff1.TxIntStat = SET;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void aputc(UART_Type *UARTn, uint8_t data)
{
	while(!(UARTn->LSR & UART_LSR_THRE));
	HAL_UART_TransmitByte(UARTn, data);
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void aputs(UART_Type *UARTn, uint8_t *p_data)
{
	while(1)
	{
		aputc(UARTn, *p_data);
		if(*p_data == 0){break;}
		else{p_data++;}
	}
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void aputhex(UART_Type *UARTn, uint32_t data)
{
	uint8_t i;
	uint8_t tx_data;
	uint32_t nibble;

	if(data > 0xFFFFFFF)
	{
		i = 32;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
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
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
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
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
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
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
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
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
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
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
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
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
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
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
uint8_t agetc(UART_Type *UARTn)
{
	while(!(UARTn->LSR & UART_LSR_RDR));
	return HAL_UART_ReceiveByte(UARTn);
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
uint8_t agets(UART_Type *UARTn, uint8_t *p_data)
{
	uint8_t receive_count = 0;

	while(1)
	{
		*p_data = agetc(UARTn);
		if(*p_data == 0x0D)
		{
			break;
		}
		else
		{
			p_data++;
			receive_count++;
		}
	}

	return receive_count;
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
uint8_t agethex(UART_Type *UARTn, uint8_t *p_data)
{
	uint8_t i;
	uint8_t receive_count;
	
	receive_count = agets(UARTn, p_data);
	
	for(i=0; i<receive_count; i++)
	{
		switch(p_data[i])
		{
			case 0x30:
				p_data[i] = 0x0;
				break;
			case 0x31:
				p_data[i] = 0x1;
				break;
			case 0x32:
				p_data[i] = 0x2;
				break;
			case 0x33:
				p_data[i] = 0x3;
				break;
			case 0x34:
				p_data[i] = 0x4;
				break;
			case 0x35:
				p_data[i] = 0x5;
				break;
			case 0x36:
				p_data[i] = 0x6;
				break;
			case 0x37:
				p_data[i] = 0x7;
				break;
			case 0x38:
				p_data[i] = 0x8;
				break;
			case 0x39:
				p_data[i] = 0x9;
				break;
			case 0x61:
				p_data[i] = 0xa;
				break;
			case 0x62:
				p_data[i] = 0xb;
				break;
			case 0x63:
				p_data[i] = 0xc;
				break;
			case 0x64:
				p_data[i] = 0xd;
				break;
			case 0x65:
				p_data[i] = 0xe;
				break;
			case 0x66:
				p_data[i] = 0xf;
				break;
			default :
				p_data[i] = 0;				
				break;
		}
	}
	
	return receive_count;
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void RegisterWrite_Handler(UART_Type *UARTn)
{
	uint32_t addr, val;
	uint32_t *p_Register;
	uint8_t data[9];
	uint8_t i, data_cnt;

	aputs(UARTn,"\n\r#########################");
	aputs(UARTn,"\n\r## Register Write/Read ##");
	aputs(UARTn,"\n\r#########################\n\r");
	
	while(1)
	{
		addr = 0; val = 0;
		
		/* Input Target Address */
		aputs(UARTn,"\n\r");		
		aputs(UARTn,">>TargetAddr : 0x");		
		data_cnt = agethex(UARTn, data);
		for(i=0; i<data_cnt; i++)
		{
			addr = (addr<<4);
			addr |= data[i];
		}
		aputhex(UARTn, addr);
		aputs(UARTn," => OK\n\r");
		
		/* Input Value */
		aputs(UARTn,">>WriteValue : 0x");		
		data_cnt = agethex(UARTn, data);
		for(i=0; i<data_cnt; i++)
		{
			val = (val<<4);
			val |= data[i];
		}
		aputhex(UARTn, val);
		aputs(UARTn," => OK\n\r");		

		/* Register Write/Read */
		p_Register = (uint32_t *)addr;
		if(data_cnt >0)
		{
			*p_Register = val;
		}
		aputs(UARTn,"[0x");
		aputhex(UARTn,addr);
		aputs(UARTn,"] = 0x");		
		aputhex(UARTn, *p_Register);
		aputs(UARTn," [OK]\n\r");
	}
}


/* --------------------------------- End Of File ------------------------------ */
