
/* ########################################################################### */
/* Parts that require value changes depending on the product */
#include "A31G12x_series.h"				// device header file
extern void I2C_Init(I2Cn_Type *I2Cn, uint32_t clockrate);		// device i2c init function

//------------------------------------------------------------------------------
//		MASK1 : I2C Control Register		//  @ I2C0->I2Cn_CR , I2C0->I2Cn_DR
//------------------------------------------------------------------------------
#define	IICEN				(1<<7)		// IIC Enable
#define	TXDLYEN	(1<<6)		// Tx Delay Enable
#define	INTEN				(1<<5)		// Interrupt Enable
#define	IIF							(1<<4)		// Interrupt Flag
#define	ACKEN			(1<<3)		// Ack Enable
#define	IMASTER	(1<<2)		// Master Mode
#define	STOPC			(1<<1)		// Stop generate
#define	STARTC		(1<<0)		// Start generate
//------------------------------------------------------------------------------
//		MASK2 : I2C Status Register		// @ I2C0->I2Cn_ST
//------------------------------------------------------------------------------
#define	GCALL			(1<<7)		// General Call
#define	TEND				(1<<6)		// Complete Transfer
#define	STOPD			(1<<5)		// Stop Detected
#define	SSEL				(1<<4)		// Slave Address Matched
#define	MLOST			(1<<3)		// Mastership Lost
#define	BUSY				(1<<2)		// IIC Bus Busy
#define	TMODE		(1<<1)		// Tx Mode
#define	RXACK			(1<<0)		// Ack is received at 9th clock of SCL

#define	SLAVE_ADDR	0xA0		// Slave Address

/* ########################################################################### */


void _I2C_Write(unsigned char* u8Data, unsigned char u8Length);
void _I2C_Read(unsigned char* u8Data, unsigned char u8Length);
void _I2C_RandomRead(unsigned char* u8Data, unsigned char u8Length);
void _I2C_Handler(void);


typedef enum
{
	I2C_IDLE = 0,
	I2C_BUSY,
}t_enum;

unsigned char gu8I2CTx_BuffLen;
unsigned char *pgu8I2C_TxBuff;
unsigned char gu8I2CRx_BuffLen;
unsigned char *pgu8I2C_RxBuff;
unsigned char gu8I2C_Status;


void delay(unsigned int u8Delay)
{
	while(u8Delay)	u8Delay--;
}


void _I2C_Write(unsigned char* u8Data, unsigned char u8Length)
{
	// I2C Busy Check
	if(I2C0->I2Cn_ST&BUSY)	I2C_Init(I2C0,100000);
	
	// I2C TxBuff Setting
	gu8I2CRx_BuffLen = 0;
	pgu8I2C_TxBuff = u8Data;
	gu8I2CTx_BuffLen = u8Length;
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	I2C0->I2Cn_DR = SLAVE_ADDR + 0;
	I2C0->I2Cn_CR = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
//		_I2C_Handler();		// Activate when polling mode
//		delay(1000);					// Activate when polling mode
	}
}


void _I2C_Read(unsigned char* u8Data, unsigned char u8Length)
{
	// I2C Busy Check
	if(I2C0->I2Cn_ST&BUSY)	I2C_Init(I2C0,100000);

	// I2C RxBuff Setting	
	gu8I2CTx_BuffLen = 0;
	pgu8I2C_RxBuff = u8Data;
	gu8I2CRx_BuffLen = u8Length;
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	I2C0->I2Cn_DR = SLAVE_ADDR + 1;
	I2C0->I2Cn_CR = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
//		_I2C_Handler();		// Activate when polling mode
//		delay(1000);					// Activate when polling mode
	}
}


void _I2C_RandomRead(unsigned char* u8Data, unsigned char u8Length)
{
	unsigned char u8WordAddress[1] = {0x00};

	// I2C Busy Check
	if(I2C0->I2Cn_ST&BUSY)	I2C_Init(I2C0,100000);

	// I2C TxBuff & RxBuff Setting	
	pgu8I2C_TxBuff = u8WordAddress;
	gu8I2CTx_BuffLen = 1;
	pgu8I2C_RxBuff = u8Data;
	gu8I2CRx_BuffLen = u8Length;	
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	I2C0->I2Cn_DR = SLAVE_ADDR + 0;
	I2C0->I2Cn_CR = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
//		_I2C_Handler();		// Activate when polling mode
//		delay(1000);					// Activate when polling mode
	}
}


/* I2C Handler */				// Call it from isr
void _I2C_Handler(void)
{
	unsigned char u8Status;
	
	u8Status = I2C0->I2Cn_ST;
	
	if((u8Status&MLOST)||(u8Status&STOPD))
	{
		gu8I2C_Status = I2C_IDLE;
		I2C_Init(I2C0,100000);
	}
	else
	{
		if(u8Status&RXACK)
		{
			// Tx mode
			if(u8Status&TMODE)
			{
				if(gu8I2CTx_BuffLen)
				{
					I2C0->I2Cn_DR = *pgu8I2C_TxBuff;
					pgu8I2C_TxBuff++;
					gu8I2CTx_BuffLen--;
				}
				else
				{
					if(gu8I2CRx_BuffLen)
					{
						// Restart for Rx
						I2C0->I2Cn_DR = SLAVE_ADDR + 1;
						I2C0->I2Cn_CR = IICEN | INTEN | ACKEN | STARTC;
					}
					else
					{
						// TxLength 0 Stop
						I2C0->I2Cn_CR |= STOPC;
					}
				}
			}
			// Rx mode
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
						*pgu8I2C_RxBuff = I2C0->I2Cn_DR;
						pgu8I2C_RxBuff++;
					}
					else if(gu8I2CRx_BuffLen == 1)
					{
						*pgu8I2C_RxBuff = I2C0->I2Cn_DR;
						pgu8I2C_RxBuff++;
						I2C0->I2Cn_CR &= ~(ACKEN);	// NoAck generate
					}
					else
					{
						// RxLength 0 Stop
						I2C0->I2Cn_CR |= STOPC;
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
				*pgu8I2C_RxBuff = I2C0->I2Cn_DR;
				pgu8I2C_RxBuff++;
			}
			// NoAck Stop
			I2C0->I2Cn_CR |= STOPC;
		}
	}
	I2C0->I2Cn_ST = 0xFF;
}


//===============================================================//
// Application main example //

unsigned char gu8TxDat[8];
unsigned char gu8RxDat[8];

void main_example(void)
{
	unsigned char i;

	I2C_Init(I2C0,100000);

	/* I2C TX Procedure */
	// Init I2C Tx Data
	for(i=0;i<8;i++)
	{
		gu8TxDat[i] = (i + 0x00);
	}
	// I2C Write
	_I2C_Write(gu8TxDat, 8);

	/* I2C RX Procedure */
	// Init I2C Rx Data
	for(i=0;i<8;i++)
	{
		gu8RxDat[i] = 0x00;
	}
	// I2C Read
	_I2C_Read(gu8RxDat, 8);
	_I2C_RandomRead(gu8RxDat, 8);

	while(1);
}

// End //
//===============================================================//