/*******************************************************************************
* Included File
*******************************************************************************/
#include "IR_API_V1.00.h"
#include "I2C_API_V1.00.h"
#include "DeviceName.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/
unsigned int			gu16Timer_Buffer;
unsigned long	gu32IR_Data;
unsigned char	gu8IR_DataCnt;
unsigned char	gu8IR_LeadOn;

unsigned char gu8CustomData;
unsigned char	gu8UserData;
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/
extern unsigned char au8TxData[8];
extern unsigned char au8RxData[1];
/*******************************************************************************
* Public Function
*******************************************************************************/
/**
* @brief Cathod Common (Bit Set makes LED ON)
dec_num : Bit numbers
0 : 0,1,2,3,4,5
1 : 1,2
2 : 0,1,3,4,6
3 : 0,1,2,3,6
4 : 1,2,5,6
5 : 0,2,3,5,6
6 : 0,2,3,4,5,6
7 : 0,1,2,5
8 : 0,1,2,3,4,5,6
9 : 0,1,2,3,5,6
A : 0,1,2,4,5,6,7
B : 0,1,2,3,4,5,6,7
C : 0,3,4,5,7
D : 0,1,2,3,4,5,7
E : 0,3,4,5,6,7
F : 0,4,5,6,7
* @param   
* @return
*/
unsigned char SEG_TO_NUM(unsigned char dec_num)
{
	unsigned char u8Data;
	switch(dec_num)
	{
		case 0:
			u8Data = 0x3F;
			break;
		case 1:
			u8Data = 0x06;
			break;
		case 2:
			u8Data = 0x5B;
			break;
		case 3:
			u8Data = 0x4F;
			break;
		case 4:
			u8Data = 0x66;
			break;
		case 5:
			u8Data = 0x6D;
			break;
		case 6:
			u8Data = 0x7D;
			break;
		case 7:
			u8Data = 0x27;
			break;
		case 8:
			u8Data = 0x7F;
			break;
		case 9:
			u8Data = 0x6F;
			break;
		case 10:
			u8Data = 0xF7;
			break;
		case 11:
			u8Data = 0xFF;
			break;
		case 12:
			u8Data = 0xB9;
			break;
		case 13:
			u8Data = 0xBF;
			break;
		case 14:
			u8Data = 0xF9;
			break;
		case 15:
			u8Data = 0xF1;
			break;
		default :
			u8Data = 0xFF;
			break;
	}
	return u8Data;
}

/**
* @brief
* @param   
* @return
*/
void IR_Rec_Init()
{
	gu32IR_Data = 0;
	gu8IR_DataCnt = 0;
	gu8IR_LeadOn = 0;
}

/**
* @brief
* @param   
* @return
*/
void IR_API_Run()
{
	gu8CustomData = (unsigned char)((gu32IR_Data >> 24) & (0xFF));
	gu8UserData = (unsigned char)((gu32IR_Data >> 8) & (0xFF));

	/* User specific code */
	switch(gu8UserData)
	{
		case 0x04:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(0);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0x84:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(1);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0x44:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(2);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0xC4:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(3);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0x24:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(4);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0xA4:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(5);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0x64:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(6);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0xE4:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(7);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0x14:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(8);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
		
		case 0x94:
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(9);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;

		default :
			au8TxData[0] = 0x02;
			au8TxData[1] = 0xFF;
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
			break;
	}
}

/**
* @brief
* @param   
* @return
*/
void IR_Rec_Handler(TIMER1n_Type *TIMER1n)
{
	/* Load Timer Capture Data */
	gu16Timer_Buffer	= TIMER1n->CAPDR;
	
	/* No IR Lead */
	if(gu8IR_LeadOn == 0)
	{
	 	if((Lead_Min <= gu16Timer_Buffer) && (gu16Timer_Buffer <= Lead_Max))
		{
			gu8IR_LeadOn = 1;
			gu8IR_DataCnt = 0;
		}
		else
		{
			IR_Rec_Init();
		} 
	}
	/* IR Lead On */
	else
	{
		gu32IR_Data = gu32IR_Data << 1;
		
		// Bit-1 detected
		if((Bit1_Min <= gu16Timer_Buffer) && (gu16Timer_Buffer <= Bit1_Max))
		{
			gu32IR_Data |= 1;
			gu8IR_DataCnt++;
		}
		//Bit-0 detected
		else if((Bit0_Min <= gu16Timer_Buffer) && (gu16Timer_Buffer <= Bit0_Max))
		{
			gu8IR_DataCnt++;
 		}
		// IR Data Error
		else
		{
			IR_Rec_Init();	
		}
	}
	/* IR Receive Complete */
	if(gu8IR_DataCnt == 32)
	{
		IR_API_Run();
		IR_Rec_Init();
	}
}

