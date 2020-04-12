/***************************************************************************//**
*   @file					main.c
*   @brief				Example of I2C API
*   @author		ABOV Semiconductor Co., Ltd.
*   @version		V1.00
*   @date				20. Feb. 2020
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

//******************************************************************************
// Include
//******************************************************************************
#include "MC97FGx16_I2C_API_V1.00.h"
#include "MC97FG316.h"

#define FUSE_CONF *(volatile unsigned char xdata *) 0x2F50
#define coderom ((unsigned char volatile code  *) 0)

//******************************************************************************
// Interrupt Service Routine
//******************************************************************************
void INT_I2C() interrupt 9
{
	// I2C interrupt
	// TODO: add your code here
	USI_I2C_InterruptHandler(I2C_CH0);
}

//******************************************************************************
// Function
//******************************************************************************
void port_init()
{
	// initialize ports
	//   3 : (SCL)    out 
	//   4 : (SDA)    out 
	P0   = 0x00;    	// port initial value
	P0IO = 0xFF;    	// direction
	P0PU = 0x00;    	// pullup
	P0OD = 0x00;    	// open drain
	P0DB = 0x00;    	// debounce

	P1   = 0x00;    	// port initial value
	P1IO = 0xFF;    	// direction
	P1PU = 0x00;    	// pullup
	P1OD = 0x00;    	// open drain
	P1DB = 0x00;    	// debounce

	P2   = 0x00;    	// port initial value
	P2IO = 0xFF;    	// direction
	P2PU = 0x00;    	// pullup
	P2OD = 0x00;    	// open drain
	P2DB = 0x00;    	// debounce

	P3   = 0x00;    	// port initial value
	P3IO = 0xFF;    	// direction
	P3PU = 0x00;    	// pullup
	P3OD = 0x00;    	// open drain
	P3DB = 0x00;    	// debounce

	// Set port function
	PSR0 = 0x00;    	// AN7 ~ AN0
	PSR1 = 0x00;    	// AN14 ~ AN8, use I2C = P07~6
	PSR1 |= 0x80; 	// move I2C = P26~5
	//PSR2 = 0xXX;  	// move I2C, UART0, EC0, T0o
}

void clock_init()
{
	// internal RC clock (16.000000MHz)
	// Nothing to do for the default clock
}

void mainloop()
{
	unsigned char i;
	unsigned char u8TxDat[8];	// I2C TX Buffer
	unsigned char u8RxDat[8];	// I2C RX Buffer

	/* I2C Init */
	USI_I2C_Initial(I2C_CH0, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);

	/* I2C Buffer Set */
	for(i=0;i<8;i++)
	{
		u8TxDat[i] = (i + 0x00);
		u8RxDat[i] = 0x00;
	}

	/* I2C Master Write 8-byte */
	USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, u8TxDat, 8, u8RxDat, 0);

	/* I2C Master Read 8-byte */
	USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, u8TxDat, 0, u8RxDat, 8);

	/* I2C Master Write 1-byte wordAddr then Read 8-byte */
	USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, u8TxDat, 1, u8RxDat, 8);

	while(1){}
}

void main()
{
	FUSE_CONF=coderom[0x4000];	// Don't erase this config. option line. FUSE_CONF(0x2F50)

	cli();								// General Interrupt disable
	port_init();			// Port setting for SCL/SDA line
	clock_init();		// System Clock setting to HIRC 40MHz
	sei();							// General Interrupt enable
	
	mainloop();
}
