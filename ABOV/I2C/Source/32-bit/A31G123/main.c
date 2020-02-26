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
#include "aa_types.h"

#include "A31G12x_I2C_API_V1.00.h"
#include "A31G12x.h"

#include "A31G12x_SCUCG.h"
#include "A31G12x_PCU.h"

//******************************************************************************
// Interrupt Service Routine
//******************************************************************************

void I2C0_Handler(void)
{
	USI_I2C_InterruptHandler(I2C_CH0);
}

void I2C1_Handler(void)
{
	USI_I2C_InterruptHandler(I2C_CH1);
}

//******************************************************************************
// Function
//******************************************************************************

void Init_Port( void )
{
	// enable peripheral clock
	SCUCG_SetPeriClk(0x3f, 0x0);	// enable all ports clock

	// I2C0->PD0:SCL0, PD1:SDA0
	PCU_ConfigureFunction( ( Pn_Type* )PD, 0, AFSRx_AF1 );
	PCU_SetDirection( ( Pn_Type* )PD, 0, ALTERN_FUNC );
	PCU_ConfigurePullupdown( ( Pn_Type* )PD, 0, PUPDx_Disable );

	PCU_ConfigureFunction( ( Pn_Type* )PD, 1, AFSRx_AF1 );
	PCU_SetDirection( ( Pn_Type* )PD, 1, ALTERN_FUNC );
	PCU_ConfigurePullupdown( ( Pn_Type* )PD, 1, PUPDx_Disable );

	// I2C1->PA1:SCL1, PA0:SDA1
	PCU_ConfigureFunction( ( Pn_Type* )PA, 0, AFSRx_AF1 );
	PCU_SetDirection( ( Pn_Type* )PA, 0, ALTERN_FUNC );
	PCU_ConfigurePullupdown( ( Pn_Type* )PA, 0, PUPDx_Disable );

	PCU_ConfigureFunction( ( Pn_Type* )PA, 1, AFSRx_AF1 );
	PCU_SetDirection( ( Pn_Type* )PA, 1, ALTERN_FUNC );
	PCU_ConfigurePullupdown( ( Pn_Type* )PA, 1, PUPDx_Disable );
}

void Init_Clock( void )
{
	uint32_t i;

	/* enable clock source */
	SCUCG_EnClkSrc( CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN | CLKSRCR_WDTRCEN, HIRCSEL_HIRC1 );
	for( i = 0; i < 1000; i++ );  // Clock Stable Time

	/* select system clock to HIRC 40MHz	*/
	SCUCG_ChangeSysClk( MCLKSEL_HIRC );
	SCUCG_SetClkDiv( WLDIV_MCLK64 | HDIV_MCLK1, SYSTDIV_HCLK1 | PDIV_HCLK1 );

	SystemCoreClock = 40000000uL;   // HCLK
	SystemPeriClock = 40000000uL;    // PCLK

	/* disable unused clock source */
	SCUCG_DisClkSrc( CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN );

	// enable clock monitoring
	SCUCG_SetMonClk( MACTS_SysClkChg, MONCS_MCLK );
}


void mainloop( void )
{
	uint8_t i;
	uint8_t u8TxDat[8];	// I2C TX Buffer
	uint8_t u8RxDat[8];	// I2C RX Buffer

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

//******************************************************************************
// Main Function
//******************************************************************************

int main( void )
{
	__disable_irq();	// General Interrupt disable

	Init_Port();					// Port setting for SCL/SDA line
	Init_Clock();				// System Clock setting to HIRC 40MHz

	__enable_irq();	// General Interrupt enable
	
	mainloop();

	return 0;
}
