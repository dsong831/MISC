
/* ########################################################################### */
/* Parts that require value changes depending on the product */
#include "MC96F6432.h"				// device header file
extern void I2C0_Init(void);		// device i2c init function

//------------------------------------------------------------------------------
//		MASK1 : I2C Control Register		//  @ USI0CR4 , USI0DR
//------------------------------------------------------------------------------
#define	IIF							(1<<7)		// Interrupt Flag
#define	IICEN				(1<<6)		// IIC Enable
#define	RESET			(1<<5)		// IIC Reset
#define	INTEN				(1<<4)		// Interrupt Enable
#define	ACKEN			(1<<3)		// Ack Enable
#define	IMASTER	(1<<2)		// Master Mode
#define	STOPC			(1<<1)		// Stop generate
#define	STARTC		(1<<0)		// Start generate
//------------------------------------------------------------------------------
//		MASK2 : I2C Status Register		// @ USI0ST2
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


void I2C_Write(unsigned char* u8Data, unsigned char u8Length);
void I2C_Read(unsigned char* u8Data, unsigned char u8Length);
void I2C_RandomRead(unsigned char* u8Data, unsigned char u8Length);
void I2C_Polling_Loop(void);


typedef enum
{
	I2C_IDLE = 0,
	I2C_BUSY,
};

unsigned char gu8I2CTx_BuffLen;
unsigned char *pgu8I2C_TxBuff;
unsigned char gu8I2CRx_BuffLen;
unsigned char *pgu8I2C_RxBuff;
unsigned char gu8I2C_Status;


void delay(unsigned int u8Delay)
{
	while(u8Delay)	u8Delay--;
}


void I2C_Write(unsigned char* u8Data, unsigned char u8Length)
{
	// I2C Busy Check
	if(USI0ST2&BUSY)	I2C0_Init();
	
	// I2C TxBuff Setting
	gu8I2CRx_BuffLen = 0;
	pgu8I2C_TxBuff = u8Data;
	gu8I2CTx_BuffLen = u8Length;
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	USI0DR = SLAVE_ADDR + 0;
	USI0CR4 = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
		I2C_Polling_Loop();
		delay(1000);
	}
}


void I2C_Read(unsigned char* u8Data, unsigned char u8Length)
{
	// I2C Busy Check
	if(USI0ST2&BUSY)	I2C0_Init();

	// I2C RxBuff Setting	
	gu8I2CTx_BuffLen = 0;
	pgu8I2C_RxBuff = u8Data;
	gu8I2CRx_BuffLen = u8Length;
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	USI0DR = SLAVE_ADDR + 1;
	USI0CR4 = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
		I2C_Polling_Loop();
		delay(1000);
	}
}


void I2C_RandomRead(unsigned char* u8Data, unsigned char u8Length)
{
	unsigned char u8WordAddress[1] = {0x00};

	// I2C Busy Check
	if(USI0ST2&BUSY)	I2C0_Init();

	// I2C TxBuff & RxBuff Setting	
	pgu8I2C_TxBuff = u8WordAddress;
	gu8I2CTx_BuffLen = 1;
	pgu8I2C_RxBuff = u8Data;
	gu8I2CRx_BuffLen = u8Length;	
	gu8I2C_Status = I2C_BUSY;

	// I2C Start
	USI0DR = SLAVE_ADDR + 0;
	USI0CR4 = IICEN | INTEN | ACKEN | STARTC;

	// I2C Loop
	while(gu8I2C_Status == I2C_BUSY)
	{
		I2C_Polling_Loop();
		delay(1000);
	}
}


/* I2C Polling Loop */
void I2C_Polling_Loop(void)
{
	unsigned char u8Status;
	
	u8Status = USI0ST2;
	
	if((u8Status&MLOST)||(u8Status&STOPD))
	{
		gu8I2C_Status = I2C_IDLE;
		I2C0_Init();
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
					USI0DR = *pgu8I2C_TxBuff;
					pgu8I2C_TxBuff++;
					gu8I2CTx_BuffLen--;
				}
				else
				{
					if(gu8I2CRx_BuffLen)
					{
						// Restart for Rx
						USI0DR = SLAVE_ADDR + 1;
						USI0CR4 = IICEN | INTEN | ACKEN | STARTC;
					}
					else
					{
						// TxLength 0 Stop
						USI0CR4 |= STOPC;
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
						*pgu8I2C_RxBuff = USI0DR;
						pgu8I2C_RxBuff++;
					}
					else if(gu8I2CRx_BuffLen == 1)
					{
						*pgu8I2C_RxBuff = USI0DR;
						pgu8I2C_RxBuff++;
						USI0CR4 &= ~(ACKEN);	// NoAck generate
					}
					else
					{
						// RxLength 0 Stop
						USI0CR4 |= STOPC;
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
				*pgu8I2C_RxBuff = USI0DR;
				pgu8I2C_RxBuff++;
			}
			// NoAck Stop
			USI0CR4 |= STOPC;
		}
	}
	USI0ST2 = 0x00;
}


//===============================================================//
// Application main example //

unsigned char gu8TxDat[8];
unsigned char gu8RxDat[8];

void main_example(void)
{
	unsigned char i;

	I2C0_Init();

	/* I2C TX Procedure */
	// Init I2C Tx Data
	for(i=0;i<8;i++)
	{
		gu8TxDat[i] = (i + 0x00);
	}
	// I2C Write
	I2C_Write(gu8TxDat, 8);

	/* I2C RX Procedure */
	// Init I2C Rx Data
	for(i=0;i<8;i++)
	{
		gu8RxDat[i] = 0x00;
	}
	// I2C Read
	I2C_Read(gu8RxDat, 8);
	I2C_RandomRead(gu8RxDat, 8);

	while(1);
}

// End //
//===============================================================//