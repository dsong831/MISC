/**********************************************************************
* @file		A31G21x_uartn.c
* @brief	Contains all functions support for UART firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "A31G21x_uartn.h"
#include "debug_frmwrk.h"

/* Private Functions ---------------------------------------------------------- */
uint32_t UartBaseClock; 

void uart_set_divisors(UART_Type *UARTx, uint32_t baudrate);

//=========================================================================
//	global variables
//=========================================================================
char	InData[80];
int		InFlag;
int		InCount; 

/*********************************************************************
 * @brief		Determines best dividers to get a target clock rate
 * @param[in]	UARTx	Pointer to selected UART peripheral, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	baudrate Desired UART baud rate.
 * @return 		None
 **********************************************************************/
void uart_set_divisors(UART_Type *UARTx, uint32_t baudrate)
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
	numerator = UartBaseClock; 
	denominator = 16 * baudrate; 

	bdr = numerator / denominator; 

	//------------------------------------------------------------------------------
	//	fraction = numerator - bdr x denominator
	//
	//	bfr = INT (fraction * 256) = ((f x d ) << 8) / d 
	//------------------------------------------------------------------------------

	fd = numerator - (bdr * denominator); 
	bfr = (fd * 256) / denominator; 
	UARTx->BDR= (uint16_t)(bdr&0xffff);
	UARTx->BFR= (uint8_t)(bfr&0xff);
}

/* End of Private Functions ---------------------------------------------------- */

/* Public Functions ----------------------------------------------------------- */
/* UART Init/DeInit functions -------------------------------------------------*/
/********************************************************************
 * @brief		Initializes the UARTx peripheral according to the specified
 *               parameters in the UART_ConfigStruct.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	UART_ConfigStruct Pointer to a UART_CFG_Type structure
 *              that contains the configuration information for the
 *              specified UART peripheral.
 * @return 		None
 *********************************************************************/
void UART_Init(UART_Type *UARTx, UART_CFG_Type *UART_ConfigStruct)
{
	uint8_t tmp;

	if(UARTx == UART0)
	{
		SCU->PER2&=~(1<<12);
		SCU->PCER2&=~(1<<12);		
		/* Set up peripheral clock for UART0 module */
		SCU->PER2|=(1<<12);
		SCU->PCER2|=(1<<12);		
	}

	if(UARTx == UART1)
	{
		SCU->PER2&=~(1<<13);
		SCU->PCER2&=~(1<<13);		
		/* Set up peripheral clock for UART1 module */
		SCU->PER2|=(1<<13);
		SCU->PCER2|=(1<<13);
	}

	// Dummy reading
	while (UARTx->LSR & UART_LSR_RDR)
	{
		tmp = UARTx->RBR;
	}
	// Wait for current transmit complete
	while (!(UARTx->LSR & UART_LSR_THRE));

	// Disable interrupt
	UARTx->IER = 0;
	// Set LCR, DCR to default state
	UARTx->LCR = 0;
	UARTx->DCR = 0;
	// Dummy reading
	tmp = UARTx->LSR;
	tmp = UARTx->IIR;	

	// uart clock set
	UartBaseClock=SystemCoreClock/2;
	
	// Set Line Control register ----------------------------	
	uart_set_divisors(UARTx, (UART_ConfigStruct->Baud_rate));

	tmp = (UARTx->LCR & UART_LCR_BREAK_EN) & UART_LCR_BITMASK;

	switch (UART_ConfigStruct->Databits){
	case UART_DATABIT_5:
		tmp |= UART_LCR_WLEN5;
		break;
	case UART_DATABIT_6:
		tmp |= UART_LCR_WLEN6;
		break;
	case UART_DATABIT_7:
		tmp |= UART_LCR_WLEN7;
		break;
	case UART_DATABIT_8:
	default:
		tmp |= UART_LCR_WLEN8;
		break;
	}

	if (UART_ConfigStruct->Parity == UART_PARITY_NONE)
	{
		// Do nothing...
	}
	else
	{
		tmp |= UART_LCR_PARITY_EN;
		switch (UART_ConfigStruct->Parity)
		{
		case UART_PARITY_ODD:
			tmp |= UART_LCR_PARITY_ODD;
			break;

		case UART_PARITY_EVEN:
			tmp |= UART_LCR_PARITY_EVEN;
			break;

		case UART_PARITY_SP_1:
			tmp |= UART_LCR_PARITY_F_1;
			break;

		case UART_PARITY_SP_0:
			tmp |= UART_LCR_PARITY_F_0;
			break;
		default:
			break;
		}
	}

	switch (UART_ConfigStruct->Stopbits){
	case UART_STOPBIT_2:
		tmp |= UART_LCR_STOPBIT_SEL;
		break;
	case UART_STOPBIT_1:
	default:
		// Do no thing
		break;
	}

	UARTx->LCR = (uint8_t)(tmp & UART_LCR_BITMASK);
}

/*********************************************************************
 * @brief		De-initializes the UARTx peripheral registers to their
 *              default reset values.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return 		None
 **********************************************************************/
void UART_DeInit(UART_Type* UARTx)
{
	if (UARTx == UART0)
	{
		/* Set up peripheral clock for UART0 module */
		SCU->PER2&=~(1<<12);
		SCU->PCER2&=~(1<<12);
	}

	if (UARTx == UART1)
	{
		/* Set up peripheral clock for UART1 module */
		SCU->PER2&=~(1<<13);
		SCU->PCER2&=~(1<<13);
	}
}

/******************************************************************************
 * @brief		Fills each UART_InitStruct member with its default value:
 * 					- 38400 bps
 * 					- 8-bit data
 * 					- 1 Stopbit
 * 					- None Parity
 * @param[in]	UART_InitStruct Pointer to a UART_CFG_Type structure which will
 * 				be initialized.
 * @return		None
 *******************************************************************************/
void UART_ConfigStructInit(UART_CFG_Type *UART_InitStruct)
{
	UART_InitStruct->Baud_rate = 38400;
	UART_InitStruct->Databits = UART_DATABIT_8;
	UART_InitStruct->Parity = UART_PARITY_NONE;
	UART_InitStruct->Stopbits = UART_STOPBIT_1;
}

/* UART Send/Recieve functions -------------------------------------------------*/
/**********************************************************************
 * @brief		Transmit a single data through UART peripheral
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	Data	Data to transmit (must be 8-bit long)
 * @return 		None
 **********************************************************************/
void UART_SendByte(UART_Type* UARTx, uint8_t Data)
{
	UARTx->THR = Data;
}

/**********************************************************************
 * @brief		Receive a single data from UART peripheral
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return 		Data received
 **********************************************************************/
uint8_t UART_ReceiveByte(UART_Type* UARTx)
{
	return (UARTx->RBR);
}

/**********************************************************************
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
uint32_t UART_Send(UART_Type *UARTx, uint8_t *txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
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
			UART_SendByte(UARTx, (*pChar++));
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
			UART_SendByte(UARTx, (*pChar++));
			bToSend--;
			bSent++;
		}
	}
	return bSent;
}

/*********************************************************************
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
uint32_t UART_Receive(UART_Type *UARTx, uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
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
			(*pChar++) = UART_ReceiveByte(UARTx);
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
				(*pChar++) = UART_ReceiveByte(UARTx);
				bRecv++;
				bToRecv--;
			}
		}
	}
	return bRecv;
}

/*********************************************************************
 * @brief		Force BREAK character on UART line, output pin UARTx TXD is
				forced to logic 0.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return 		None
 **********************************************************************/
void UART_ForceBreak(UART_Type* UARTx)
{
	UARTx->LCR |= UART_LCR_BREAK_EN;
}

/********************************************************************
 * @brief 		Enable or disable specified UART interrupt.
 * @param[in]	UARTx	UART peripheral selected, should be
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	UARTIntCfg	Specifies the interrupt flag,
 * 				should be one of the following:
 * 					- UART_INTCFG_RBR 	:RBR Interrupt enable
 * 					- UART_INTCFG_THRE 	:THR Interrupt enable
 * 					- UART_INTCFG_RLS 	:RX line status interrupt enable
 * 					- UART_INTCFG_DRX 	:THR Interrupt enable
 * 					- UART_INTCFG_DTX 	:RX line status interrupt enable
 * @param[in]	NewState New state of specified UART interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this UART interrupt type.
 * 					- DISALBE	:Disable this UART interrupt type.
 * @return 		None
 *********************************************************************/
void UART_IntConfig(UART_Type *UARTx, UART_INT_Type UARTIntCfg, FunctionalState NewState)
{
	uint32_t tmp;

	switch(UARTIntCfg){
		case UART_INTCFG_RBR:
			tmp = UART_IER_RBRINT_EN;
			break;
		case UART_INTCFG_THRE:
			tmp = UART_IER_THREINT_EN;
			break;
		case UART_INTCFG_RLS:
			tmp = UART_IER_RLSINT_EN;
			break;
		case UART_INTCFG_TXE:
			tmp = UART_IER_TXE_EN;
			break;	
	}

	if (NewState == ENABLE)
	{
		UARTx->IER |= tmp;
	}
	else
	{
		UARTx->IER &= (~tmp) & UART_IER_BITMASK;
	}
}


/*********************************************************************
 * @brief 		Get current value of Line Status register in UART peripheral.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return		Current value of Line Status register in UART peripheral.
 * Note:	The return value of this function must be ANDed with each member in
 * 			UART_LS_Type enumeration to determine current flag status
 * 			corresponding to each Line status type. Because some flags in
 * 			Line Status register will be cleared after reading, the next reading
 * 			Line Status register could not be correct. So this function used to
 * 			read Line status register in one time only, then the return value
 * 			used to check all flags.
 *********************************************************************/
uint8_t UART_GetLineStatus(UART_Type* UARTx)
{
	return ((UARTx->LSR) & UART_LSR_BITMASK);
}

/**********************************************************************
 * @brief		Check whether if UART is busy or not
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return		RESET if UART is not busy, otherwise return SET.
 **********************************************************************/
FlagStatus UART_CheckBusy(UART_Type *UARTx)
{
	if (UARTx->LSR & UART_LSR_TEMT){
		return RESET;
	} else {
		return SET;
	}
}

/*********************************************************************
 * @brief		Configure Data Control mode for UART peripheral
 * @param[in]	UARTx	
 * @param[in]	Data Control mode, should be:
 * 					- UART_DATA_CONTROL_LOOPBACK	:Loop back mode.
 * 					- UART_DATA_CONTROL_RXINV	: RX Data inversion mode.
 * 					- UART_DATA_CONTROL_TXINV	:TX Data inversion mode.
 * 					- UART_DATA_CONTROL_RTXINV	:TX RX Data inversion mode.
 * @param[in]	NewState New State of this mode, should be:
 * 					- ENABLE	:Enable this mode.
					- DISABLE	:Disable this mode.
 * @return none
 **********************************************************************/
void UART_DataControlConfig(UART_Type *UARTx, UART_DATA_CONTROL_Type Mode, FunctionalState NewState)
{
	uint8_t tmp;

	switch(Mode){
	case UART_DATA_CONTROL_LOOPBACK:
		tmp = UART_DCR_LBON;
		break;
	case UART_DATA_CONTROL_RXINV:
		tmp = UART_DCR_RXINV;
		break;
	case UART_DATA_CONTROL_TXINV:
		tmp = UART_DCR_TXINV;
		break;
	case UART_DATA_CONTROL_RTXINV:
		tmp = UART_DCR_RXINV | UART_DCR_TXINV;
		break;	
	default:
		break;
	}

	if (NewState == ENABLE)
	{
		UARTx->DCR |= tmp;
	}
	else
	{
		UARTx->DCR &= (~tmp) & UART_DCR_BITMASK;
	}
}

/*********************************************************************
 * @brief		Configure inter-frame delay time for UART peripheral
 * @param[in]	UARTx	
 * @param[in]	inter-frame delay time : 1 bit time  unit (0~7)
 * @return none
 **********************************************************************/
void UART_IFDelayConfig(UART_Type *UARTx, uint8_t waitval)
{
	if (waitval<8){ 
		UARTx->IDTR=waitval;
	}
}

/* --------------------------------- End Of File ------------------------------ */

