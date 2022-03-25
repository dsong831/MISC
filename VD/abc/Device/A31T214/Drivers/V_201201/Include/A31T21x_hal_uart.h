/**********************************************************************//**
* @file				A31T21x_hal_uart.h
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			26. JUNE. 2020
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
*ABOV assumes no liability for application assistance or the design of purchasers?? products. No license, 
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


#ifndef A31T21X_HAL_UART_H
#define A31T21X_HAL_UART_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros -------------------------------------------------------- */
#define UART_BLOCKING_TIMEOUT			(0xFFFFFFFFUL)

#define UART_IER_DRIE					((uint8_t)(1<<0)) 	/*!<Data Receive Interrupt Enable bit */
#define UART_IER_THREIE			((uint8_t)(1<<1)) 	/*!<Transmit Holding Register Empty Interrupt Enable bit*/
#define UART_IER_RLSIE				((uint8_t)(1<<2)) 	/*!<Receiver Line Status Interrupt Enable bit */
#define UART_IER_TXEIE				((uint8_t)(1<<3)) 	/*!<Transmit Register Empty Interrupt Enable */
#define UART_IER_BITMASK	((uint8_t)(0x3F)) /*!<UART Interrupt enable register bit mask */

#define UART_IIR_IPEN					((uint8_t)(1<<0)) /*!<Interrupt Pending bit (0: Interrupt pending, 1: No pending) */
#define UART_IIR_IID_RLS			((uint8_t)(3<<1)) /*!<Interrupt identification: Receive line status*/
#define UART_IIR_IID_RDA			((uint8_t)(2<<1)) /*!<Interrupt identification: Receive data available*/
#define UART_IIR_IID_THRE		((uint8_t)(1<<1)) /*!<Interrupt identification: Transmitter Holding Register Empty*/
#define UART_IIR_TXE						((uint8_t)(1<<4)) /*!<Transmit Complete Interrupt Source ID bit */
#define UART_IIR_BITMASK		((uint8_t)(0x1F)) /*!<UART interrupt identification register bit mask */

#define UART_LCR_DLEN_5		((uint8_t)(0<<0)) /*!<5-bit data length */
#define UART_LCR_DLEN_6		((uint8_t)(1<<0)) /*!<6-bit data length */
#define UART_LCR_DLEN_7		((uint8_t)(2<<0)) /*!<7-bit data length */
#define UART_LCR_DLEN_8		((uint8_t)(3<<0)) /*!<8-bit data length */
#define UART_LCR_STOPBIT	((uint8_t)(1<<2)) /*!<0: 1-stop bit, 1: 2-stop bit */
#define UART_LCR_PEN					((uint8_t)(1<<3)) /*!<0: Disable parity, 1: Enable parity */
#define UART_LCR_PARITY		((uint8_t)(1<<4)) /*!<0: Odd parity, 1: Even parity */
#define UART_LCR_STICKP		((uint8_t)(1<<5)) /*!<0: Disable parity stuck, 1: Enable parity stuck */
#define UART_LCR_BREAK			((uint8_t)(1<<6)) /*!<0: Normal transfer mode, 1: Break transmit mode */
#define UART_LCR_BITMASK	((uint8_t)(0x7F)) /*!<0: Normal transfer mode, 1: Break transmit mode */

#define UART_DCR_TXINV			((uint8_t)(1<<2)) /*!<data control register: Tx Data Inversion*/
#define UART_DCR_RXINV			((uint8_t)(1<<3)) /*!<data control register: Rx Data Inversion*/
#define UART_DCR_LBON			((uint8_t)(1<<4)) /*!<data control register: Local loopback test mode*/
#define UART_DCR_BITMASK	((uint8_t)(0x1C)) /*!<UART data control bit mask */

#define UART_LSR_RDR				((uint8_t)(1<<0)) 	/*!<Line status register: Receive data ready*/
#define UART_LSR_OE						((uint8_t)(1<<1)) 	/*!<Line status register: Overrun error*/
#define UART_LSR_PE						((uint8_t)(1<<2)) 	/*!<Line status register: Parity error*/
#define UART_LSR_FE						((uint8_t)(1<<3)) 	/*!<Line status register: Framing error*/
#define UART_LSR_BI						((uint8_t)(1<<4)) 	/*!<Line status register: Break interrupt*/
#define UART_LSR_THRE				((uint8_t)(1<<5)) 	/*!<Line status register: Transmit holding register empty*/
#define UART_LSR_TEMT			((uint8_t)(1<<6)) 	/*!<Line status register: Transmitter empty*/
#define UART_LSR_BITMASK	((uint8_t)(0x7F)) 	/*!<Line status bit mask */


/* Public Types --------------------------------------------------------------- */
typedef enum {
	UART_DATALEN_5 = 0,				/*!< UART 5 bit data mode */
	UART_DATALEN_6,							/*!< UART 6 bit data mode */
	UART_DATALEN_7,							/*!< UART 7 bit data mode */
	UART_DATALEN_8							/*!< UART 8 bit data mode */
} UART_DATALEN_Type;

typedef enum {
	UART_STOPBIT_1 = 0,				/*!< UART 1 Stop Bits Select */
	UART_STOPBIT_2							/*!< UART 2 Stop Bits Select */
} UART_STOPBIT_Type;

typedef enum {
	UART_PARITY_NONE = 0,		/*!< No parity */
	UART_PARITY_ODD,						/*!< Odd parity */
	UART_PARITY_EVEN,					/*!< Even parity */
	UART_PARITY_SP_1,					/*!< Forced "1" stick parity */
	UART_PARITY_SP_0						/*!< Forced "0" stick parity */
} UART_PARITY_Type;

typedef struct {
	uint32_t tBaudRate;														/**< UART baud rate */
	UART_DATALEN_Type tDataLength;		/**< Number of data bits, should be:
																															- UART_DATABIT_5: UART 5 bit data mode
																															- UART_DATABIT_6: UART 6 bit data mode
																															- UART_DATABIT_7: UART 7 bit data mode
																															- UART_DATABIT_8: UART 8 bit data mode	*/	
  UART_STOPBIT_Type tStopbit;						/**< Number of stop bits, should be:
																															- UART_STOPBIT_1: UART 1 Stop Bits Select
																															- UART_STOPBIT_2: UART 2 Stop Bits Select	*/
  UART_PARITY_Type tParity;								/**< Parity selection, should be:
																															- UART_PARITY_NONE: No parity
																															- UART_PARITY_ODD: Odd parity
																															- UART_PARITY_EVEN: Even parity
																															- UART_PARITY_SP_1: Forced "1" stick parity
																															- UART_PARITY_SP_0: Forced "0" stick parity	*/
} UART_CFG_Type;

typedef enum {
	UART_INT_POLLING = 0,					/**< Polling mode */
	UART_INT_RXE,											/**< Rx Interrupt Only */
	UART_INT_THRE,										/**< Tx Interrupt Only */
	UART_INT_RXE_THRE,						/**< Rx/Tx Interrupt */
	UART_INT_RLS,											/**< LineError Interrupt Only */
	UART_INT_RXE_RLS,							/**< Rx/LineError Interrupt */
	UART_INT_THRE_RLS,						/**< Tx/LineError Interrupt */
	UART_INT_RXE_THRE_RLS		/**< Rx/Tx/LineError Interrupt */
} UART_INTCFG_Type;


/* Public Functions ----------------------------------------------------------- */
HAL_Status_Type HAL_UART_Init(UART_Type *UARTn, UART_CFG_Type *UARTConfigStruct);
HAL_Status_Type HAL_UART_DeInit(UART_Type *UARTn);
HAL_Status_Type HAL_UART_ConfigInterrupt(UART_Type *UARTn, UART_INTCFG_Type UARTIntConfig, EN_DIS_Type int_en);
void HAL_UART_BaudrateSet(UART_Type *UARTn, uint32_t baudrate);
void HAL_UART_TransmitByte(UART_Type* UARTn, uint8_t tx_data);
uint8_t HAL_UART_ReceiveByte(UART_Type* UARTn);
uint32_t HAL_UART_Transmit(UART_Type* UARTn, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag);
uint32_t HAL_UART_Receive(UART_Type *UARTn, uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag);

void RegisterWrite_Handler(UART_Type *UARTn);



/////////////////////////////////////////////////////////////////////////////////////////////////////
/* For Using UART Interrupt ----------------------------------------------------------- */
#define UART_RING_BUFSIZE 32

#define __BUF_MASK (UART_RING_BUFSIZE-1)
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
#define __BUF_INCR(bufidx)	(bufidx=(bufidx+1)&__BUF_MASK)

typedef struct {
	uint32_t tx_head;														/*!< UART Tx ring buffer head index */
	uint32_t tx_tail;																/*!< UART Tx ring buffer tail index */
	uint32_t rx_head;														/*!< UART Rx ring buffer head index */
	uint32_t rx_tail;																/*!< UART Rx ring buffer tail index */
	uint8_t  tx[UART_RING_BUFSIZE];	/*!< UART Tx data ring buffer */
	uint8_t  rx[UART_RING_BUFSIZE];	/*!< UART Rx data ring buffer */
	SET_RESET_Type TxIntStat;						/*!< UART Current Tx Interrupt enable state */
} UART_RING_BUFFER_T;


uint32_t UARTSend(UART_Type *UARTn, uint8_t *txbuf, uint8_t buflen);
uint32_t UARTReceive(UART_Type *UARTn, uint8_t *rxbuf, uint8_t buflen);
void UART_IRQHandler_IT(UART_Type *UARTn);
void UART_IntErr(uint32_t error_code);
void UART_IntReceive(UART_Type *UARTn);
void UART_IntTransmit(UART_Type *UARTn);

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif


#endif		/* A31T21X_HAL_UART_H */

/* --------------------------------- End Of File ------------------------------ */
