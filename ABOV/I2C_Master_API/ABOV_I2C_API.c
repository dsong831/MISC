
/* ########################################################################### */
/* Parts that require value changes depending on the product */
#include "MC97FG316.h"				// device header file
extern void I2C_Initial(void);		// device i2c init function

//------------------------------------------------------------------------------
//		MASK1 : I2C Control Register		//  @ I2CCR
//------------------------------------------------------------------------------
#define	IIF				(1<<7)		// Interrupt Flag
#define	IICEN			(1<<6)		// IIC Enable
#define	RESET			(1<<5)		// IIC Reset
#define	INTEN			(1<<4)		// Interrupt Enable
#define	ACKEN			(1<<3)		// Ack Enable
#define	IMASTER			(1<<2)		// Master Mode
#define	STOPC			(1<<1)		// Stop generate
#define	STARTC			(1<<0)		// Start generate
//------------------------------------------------------------------------------
//		MASK2 : I2C Status Register		// @ I2CSR
//------------------------------------------------------------------------------
#define	GCALL			(1<<7)		// General Call
#define	TEND			(1<<6)		// Complete Transfer
#define	STOPD			(1<<5)		// Stop Detected
#define	SSEL			(1<<4)		// Slave Address Matched
#define	MLOST			(1<<3)		// Mastership Lost
#define	BUSY			(1<<2)		// IIC Bus Busy
#define	TMODE			(1<<1)		// Tx Mode
#define	RXACK			(1<<0)		// Ack is received at 9th clock of SCL

/* ########################################################################### */


unsigned char gu8I2CTx_BuffLen;		// TX Buffer Length
unsigned char *pgu8I2C_TxBuff;		// TX Buffer
unsigned char gu8I2CRx_BuffLen;		// RX Buffer Length
unsigned char *pgu8I2C_RxBuff;		// RX Buffer
unsigned char gu8I2C_Status;
unsigned char gu8I2C_SlaveAddr;

typedef enum
{
	I2C_IDLE = 0,
	I2C_BUSY,
};


void _I2C_Write(unsigned char u8Addr, unsigned char* u8Data, unsigned char u8Length);
void _I2C_Read(unsigned char u8Addr, unsigned char* u8Data, unsigned char u8Length);
void _I2C_RandomRead(unsigned char u8Addr, unsigned char* u8Data, unsigned char u8Length);
void _I2C_SlaveTxSet(void);
void _I2C_Handler(void);


void delay(unsigned int u8Delay)
{
	while(u8Delay)	u8Delay--;
}


void _I2C_Write(unsigned char u8Addr, unsigned char* u8Data, unsigned char u8Length)
{
	// I2C Busy Check
	if(I2CSR&BUSY)	I2C_Initial();
	
	// I2C TxBuff Setting
	gu8I2CRx_BuffLen = 0;
	pgu8I2C_TxBuff = u8Data;
	gu8I2CTx_BuffLen = u8Length;
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	gu8I2C_SlaveAddr = u8Addr;
	I2CDR = gu8I2C_SlaveAddr | 0x00;
	I2CCR = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
//		_I2C_Handler();		// Activate when polling mode
//		delay(1000);		// Activate when polling mode
	}
}


void _I2C_Read(unsigned char u8Addr, unsigned char* u8Data, unsigned char u8Length)
{
	// I2C Busy Check
	if(I2CSR&BUSY)	I2C_Initial();

	// I2C RxBuff Setting	
	gu8I2CTx_BuffLen = 0;
	pgu8I2C_RxBuff = u8Data;
	gu8I2CRx_BuffLen = u8Length;
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	gu8I2C_SlaveAddr = u8Addr;
	I2CDR = gu8I2C_SlaveAddr | 0x01;
	I2CCR = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
//		_I2C_Handler();		// Activate when polling mode
//		delay(1000);		// Activate when polling mode
	}
}


void _I2C_RandomRead(unsigned char u8Addr, unsigned char* u8Data, unsigned char u8Length)
{
	unsigned char u8WordAddress[1] = {0x00};

	// I2C Busy Check
	if(I2CSR&BUSY)	I2C_Initial();

	// I2C TxBuff & RxBuff Setting	
	pgu8I2C_TxBuff = u8WordAddress;
	gu8I2CTx_BuffLen = 1;
	pgu8I2C_RxBuff = u8Data;
	gu8I2CRx_BuffLen = u8Length;	
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	gu8I2C_SlaveAddr = u8Addr;
	I2CDR = gu8I2C_SlaveAddr | 0x00;
	I2CCR = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
//		_I2C_Handler();		// Activate when polling mode
//		delay(1000);		// Activate when polling mode
	}
}


void _I2C_SlaveTxSet(void)
{
	unsigned char u8Data[8] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77};

	// I2C Busy Check
	if(I2CSR&BUSY)	I2C_Initial();
	
	// I2C TxBuff Setting
	pgu8I2C_TxBuff = u8Data;
	gu8I2C_Status = I2C_BUSY;

	// I2C Ready
	I2CCR = IICEN | INTEN | ACKEN;
}


/* I2C Handler */				// Call it from isr
void _I2C_Handler(void)
{
	unsigned char u8Status;
	
	u8Status = I2CSR;
	
	if((u8Status&MLOST)||(u8Status&STOPD))
	{
		gu8I2C_Status = I2C_IDLE;
		I2C_Initial();
	}
	else
	{
		// I2C Master Mode
		if(I2CCR&IMASTER)
		{
			if(u8Status&RXACK)
			{
				// Master Tx mode
				if(u8Status&TMODE)
				{
					if(gu8I2CTx_BuffLen)
					{
						I2CDR = *pgu8I2C_TxBuff;
						pgu8I2C_TxBuff++;
						gu8I2CTx_BuffLen--;
					}
					else
					{
						if(gu8I2CRx_BuffLen)
						{
							// Restart for Rx
							I2CDR = gu8I2C_SlaveAddr | 0x01;
							I2CCR = IICEN | INTEN | ACKEN | STARTC;
						}
						else
						{
							// TxLength 0 Stop
							I2CCR |= STOPC;
						}
					}
				}
				// Master Rx mode
				else
				{
					if(u8Status&GCALL)
					{
						// Address is received
					}
					else
					{
						gu8I2CRx_BuffLen--;
						if(gu8I2CRx_BuffLen > 1)
						{
							*pgu8I2C_RxBuff = I2CDR;
							pgu8I2C_RxBuff++;
						}
						else if(gu8I2CRx_BuffLen == 1)
						{
							*pgu8I2C_RxBuff = I2CDR;
							pgu8I2C_RxBuff++;
							I2CCR &= ~(ACKEN);	// NoAck generate
						}
						else
						{
							// RxLength 0 Stop
							I2CCR |= STOPC;
						}
					}
				}
			}
			else
			{
				// NoAck data is received
				if(gu8I2CRx_BuffLen == 1)
				{
					gu8I2CRx_BuffLen = 0;
					*pgu8I2C_RxBuff = I2CDR;
					pgu8I2C_RxBuff++;
				}
				// NoAck Stop
				I2CCR |= STOPC;
			}
		}
		// I2C Slave Mode
		else
		{
			// Slave Tx mode
			if(u8Status&TMODE)
			{
				I2CDR = *pgu8I2C_TxBuff;
				pgu8I2C_TxBuff++;
			}
			// Slave Rx mode
			else
			{
				*pgu8I2C_RxBuff = I2CDR;
				pgu8I2C_RxBuff++;
			}
		}
	}
	I2CSR = 0x00;
}


//===============================================================//
// Application main example //

void _I2C_main(void)
{
	unsigned char i;
	unsigned char u8TxDat[8];
	unsigned char u8RxDat[8];
	unsigned char u8SlaveAddr = 0xA0;	// Slave Device Address
	
	I2C_Initial();

////////* I2C Master TX Procedure *////////
	for(i=0;i<8;i++)
	{
		u8TxDat[i] = (i + 0x00);
	}
	_I2C_Write(u8SlaveAddr, u8TxDat, 8);
///////////////////////////////////////
	
////////* I2C MasterRX Procedure *////////
	for(i=0;i<8;i++)
	{
		u8RxDat[i] = 0x00;
	}
	_I2C_Read(u8SlaveAddr, u8RxDat, 8);
	_I2C_RandomRead(u8SlaveAddr, u8RxDat, 8);
///////////////////////////////////////

////////* I2C Slave Mode *////////
	_I2C_SlaveTxSet();
///////////////////////////////////////

	while(1);
}

// End //
//===============================================================//