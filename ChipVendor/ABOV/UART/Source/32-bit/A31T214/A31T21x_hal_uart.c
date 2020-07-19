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
uint8_t u8Dummy;

/* Private Types --------------------------------------------------------------- */
RingBuffer_Type	tx0_RingBuffer;
RingBuffer_Type	tx1_RingBuffer;
RingBuffer_Type	rx0_RingBuffer;
RingBuffer_Type	rx1_RingBuffer;


/* Public Functions -------------------------------------------------------- */
/**********************************************************************//**
* @brief						Read a single data from UART peripheral (Polling mode)
* @param[in]	UARTn	UART peripheral selected, should be:
*											- UART0 : UART0 peripheral
*											- UART1 : UART1 peripheral
* @return					Received data
 **********************************************************************/
uint8_t HAL_UART_ReadBuffer(UART_Type* UARTn)
{
	u8Dummy = UARTn->RBR;		// Dummy read

	while(!(UARTn->LSR & UART_LSR_RDR));
	return UARTn->RBR;
}

/**********************************************************************//**
 * @brief						Write a single data through UART peripheral (Polling mode)
 * @param[in]	UARTn		UART peripheral selected, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return					None
 **********************************************************************/
void HAL_UART_WriteBuffer(UART_Type* UARTn, uint8_t tx_data)
{
	while(!(UARTn->LSR & UART_LSR_THRE));	// Wait for current transmit complete
	UARTn->THR = tx_data;
}

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
 * @param[in]	UARTIntStruct		UART Interrupt selected, should be:
 *											- UART_INT_POLLING
 *											- UART_INT_RXE
 *											- UART_INT_THR
 *											- UART_INT_RXE_THR
 *											- UART_INT_RLS
 *											- UART_INT_RXE_RLS
 *											- UART_INT_THR_RLS
 *											- UART_INT_RXE_THR_RLS
 * @return					None
 **********************************************************************/
HAL_Status_Type HAL_UART_Init(UART_Type *UARTn, UART_CFG_Type *UARTConfigStruct, UART_INTCFG_Type UARTIntConfig)
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

	/* UART Register Initialize */
	UARTn->IER = 0;
	UARTn->LCR = 0;
	UARTn->DCR = 0;
	u8Dummy = UARTn->LSR;
	u8Dummy = UARTn->IIR;

	/* UART RingBuffer Initialize */
	HAL_UART_RingBuffer_Init(UARTn);
	
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
	UARTn->IER = (uint8_t)(u8Reg & UART_IER_BITMASK);		// Set interrupt enable register

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Determines best dividers to get a target clock rate
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @param[in]	baudrate Desired UART baud rate.
 * @param[in]	ref_clk	Reference clock for baudrate.
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
	UARTn->BDR= (uint16_t)(bdr&0xffff);
	UARTn->BFR= (uint8_t)(bfr&0xff);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/*********************************************************************//**
 * @brief						Initialize RingBuffer Parameters
 * @param[in]	None
 * @return				None
 **********************************************************************/
void HAL_UART_RingBuffer_Init(UART_Type *UARTn)
{
	if(UARTn == UART0)
	{
		tx0_RingBuffer.HeadPtr = 0;
		tx0_RingBuffer.TailPtr = 0;
		tx0_RingBuffer.State = UART_TX_IDLE;
		
		rx0_RingBuffer.HeadPtr = 0;
		rx0_RingBuffer.TailPtr = 0;
		rx0_RingBuffer.State = UART_RX_IDLE;
	}
	else if(UARTn == UART1)
	{
		tx1_RingBuffer.HeadPtr = 0;
		tx1_RingBuffer.TailPtr = 0;
		tx1_RingBuffer.State = UART_TX_IDLE;

		rx1_RingBuffer.HeadPtr = 0;
		rx1_RingBuffer.TailPtr = 0;
		rx1_RingBuffer.State = UART_RX_IDLE;
	}
}

/*********************************************************************//**
 * @brief						UART Transmit Data (Interrupt mode)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @param[in]	tx_data		UART Transmit Data
 * @return				None
 **********************************************************************/
void HAL_UART_TransmitData(UART_Type *UARTn, uint8_t tx_data)
{
	UARTn->IER &= ~(UART_IER_THREIE);		// Disable thre interrupt

	if(UARTn == UART1)
	{
		if(tx1_RingBuffer.HeadPtr == tx1_RingBuffer.TailPtr)
		{
			if(tx1_RingBuffer.State == UART_TX_IDLE)
			{
				HAL_UART_WriteBuffer(UARTn, tx_data);	// First data transmit
				tx1_RingBuffer.State = UART_TX_BUSY;
			}
			else
			{
				tx1_RingBuffer.Buffer[tx1_RingBuffer.HeadPtr++] = tx_data;
				if(tx1_RingBuffer.HeadPtr > RING_BUFFER_LENGTH)
				{
					tx1_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			tx1_RingBuffer.Buffer[tx1_RingBuffer.HeadPtr++] = tx_data;
			if(tx1_RingBuffer.HeadPtr > RING_BUFFER_LENGTH)
			{
				tx1_RingBuffer.HeadPtr = 0;
			}
		}
	}

	UARTn->IER |= (UART_IER_THREIE);		// Enable thre interrupt
}

/*********************************************************************//**
 * @brief						UART Receive Data (Interrupt mode)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				received data
 **********************************************************************/
int8_t HAL_UART_ReceiveData(UART_Type *UARTn)
{
	if(UARTn == UART1)
	{
		if(rx1_RingBuffer.HeadPtr != rx1_RingBuffer.TailPtr)
		{
			if(rx1_RingBuffer.TailPtr > RING_BUFFER_LENGTH)
			{
				rx1_RingBuffer.TailPtr = 0;
			}
			return rx1_RingBuffer.Buffer[rx1_RingBuffer.TailPtr++];
		}
		else
		{
				rx1_RingBuffer.State = UART_RX_IDLE;
				return -1;
		}
	}
	else
	{
		return -1;
	}
}

/*********************************************************************//**
 * @brief						UART Transmit Handler (Interrupt Handler)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				None
 **********************************************************************/
void HAL_UART_TX_Handler(UART_Type *UARTn)
{
	if(UARTn == UART1)
	{
		if(tx1_RingBuffer.HeadPtr != tx1_RingBuffer.TailPtr)
		{
			if(UARTn->LSR & UART_LSR_THRE)
			{
				UARTn->THR = tx1_RingBuffer.Buffer[tx1_RingBuffer.TailPtr++];
				if(tx1_RingBuffer.TailPtr > RING_BUFFER_LENGTH)
				{
					tx1_RingBuffer.TailPtr = 0;
				}
			}
		}
		else
		{
				tx1_RingBuffer.State = UART_TX_IDLE;
		}
	}

	u8Dummy = UARTn->IIR;		// Clear interrupt
}

/*********************************************************************//**
 * @brief						UART Receive Handler (Interrupt Handler)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				None
 **********************************************************************/
void HAL_UART_RX_Handler(UART_Type *UARTn)
{
	if(UARTn == UART1)
	{
		if(rx1_RingBuffer.HeadPtr == rx1_RingBuffer.TailPtr)
		{
			if(rx1_RingBuffer.State == UART_RX_IDLE)
			{
				rx1_RingBuffer.Buffer[rx1_RingBuffer.HeadPtr++] = UARTn->RBR;	// First data receive
				rx1_RingBuffer.State = UART_RX_BUSY;
				if(rx1_RingBuffer.HeadPtr > RING_BUFFER_LENGTH)
				{
					rx1_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				rx1_RingBuffer.Buffer[rx1_RingBuffer.HeadPtr++] = UARTn->RBR;
				if(rx1_RingBuffer.HeadPtr > RING_BUFFER_LENGTH)
				{
					rx1_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			rx1_RingBuffer.Buffer[rx1_RingBuffer.HeadPtr++] = UARTn->RBR;
			if(rx1_RingBuffer.HeadPtr > RING_BUFFER_LENGTH)
			{
				rx1_RingBuffer.HeadPtr = 0;
			}
		}
	}
	
	u8Dummy = UARTn->IIR;		// Clear interrupt
}




#if 0
/**********************************************************************//**
 * @brief		Send a block of data via UART peripheral
 * @param[in]	UARTx	Selected UART peripheral used to send data, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	txbuf 	Pointer to Transmit buffer
 * @param[in]	buflen 	Length of Transmit buffer
 * @param[in] 	flag 	Flag used in  UART transfer, should be
 * 					- NONE_BLOCKING
 * 					- BLOCKING
 * @return 		Number of bytes sent.
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t HAL_UART_Transmit(UART_Type *UARTx, uint8_t *txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
{
	uint32_t bToSend, bSent, timeOut;
	uint8_t *pChar = txbuf;

	bToSend = buflen;

	// blocking mode
	if (flag == BLOCKING) {
		bSent = 0;
		while (bToSend){
			timeOut = UART_BLOCKING_TIMEOUT;
			// Wait for THR empty with timeout
			while (!(UARTx->LSR & UART_LSR_THRE)) {
				if (timeOut == 0) break;
				timeOut--;
			}
			// Time out!
			if(timeOut == 0) break;
			HAL_UART_TransmitByte(UARTx, (*pChar++));
			bToSend--;
			bSent++;
		}
	}
	// None blocking mode
	else {
		bSent = 0;
		while (bToSend) {
			if (!(UARTx->LSR & UART_LSR_THRE)){
				//continue;
				break;
			}
			HAL_UART_TransmitByte(UARTx, (*pChar++));
			bToSend--;
			bSent++;
		}
	}
	return bSent;
}

/*********************************************************************//**
 * @brief		Receive a block of data via UART peripheral
 * @param[in]	UARTx	Selected UART peripheral used to send data,
 * 				should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[out]	rxbuf 	Pointer to Received buffer
 * @param[in]	buflen 	Length of Received buffer
 * @param[in] 	flag 	Flag mode, should be:
 * 					- NONE_BLOCKING
 * 					- BLOCKING
 * @return 		Number of bytes received
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t HAL_UART_Receive(UART_Type *UARTx, uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
{
	uint32_t bToRecv, bRecv, timeOut;
	uint8_t *pChar = rxbuf;

	bToRecv = buflen;

	// Blocking mode
	if (flag == BLOCKING) {
		bRecv = 0;
		while (bToRecv){
			timeOut = UART_BLOCKING_TIMEOUT;
			while (!(UARTx->LSR & UART_LSR_RDR)){
				if (timeOut == 0) break;
				timeOut--;
			}
			// Time out!
			if(timeOut == 0) break;
			// Get data from the buffer
			(*pChar++) = HAL_UART_ReceiveByte(UARTx);
			bToRecv--;
			bRecv++;
		}
	}
	// None blocking mode
	else {
		bRecv = 0;
		while (bToRecv) {
			if (!(UARTx->LSR & UART_LSR_RDR)) {
				break;
			} else {
				(*pChar++) = HAL_UART_ReceiveByte(UARTx);
				bRecv++;
				bToRecv--;
			}
		}
	}
	return bRecv;
}
#endif


/* --------------------------------- End Of File ------------------------------ */
