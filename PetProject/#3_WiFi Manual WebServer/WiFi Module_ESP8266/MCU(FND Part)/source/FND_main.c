
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

void SEG_TEST(void)
{
	unsigned char au8TxData[8];
	unsigned char au8RxData[8];
	unsigned char i = 0;
	unsigned char inputData = 0;
	unsigned long j;
	
	// I2C0 PD0:SCL0, PD1:SDA0	
	PCU_SetDirection(PD, 0, ALTERN_FUNC);
	PCU_ConfigureFunction(PD, 0, FUNC1);
	PCU_SetDirection(PD, 1, ALTERN_FUNC);
	PCU_ConfigureFunction(PD, 1, FUNC1);
	
	USI_I2C_Initial(I2C_CH0, I2C_SPEED, I2C_DEVICE_ADDRESS, I2C_ACK_ENABLE);
	
	// Driver set to output mode
	au8TxData[0] = 0x06;
	au8TxData[1] = 0x00;
	USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);

	// Driver set to SEG number
	au8TxData[0] = 0x02;
	au8TxData[1] = ~SEG_TO_NUM(0);
	USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);

	while(1)
	{
		inputData = 0;
		inputData = PA->INDR;
		inputData &= 0x0F;
		
		if(inputData == 0x00)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(0);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x01)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(1);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x02)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(2);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x03)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(3);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x04)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(4);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x05)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(5);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x06)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(6);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x07)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(7);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x08)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(8);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else if(inputData == 0x09)
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(9);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
		}
		else
		{
			au8TxData[0] = 0x02;
			au8TxData[1] = ~SEG_TO_NUM(0);
			USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);						
		}
	}
}

int main(void)
{
	SEG_TEST();
	
	return (0);
}

