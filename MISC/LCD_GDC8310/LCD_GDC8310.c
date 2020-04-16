
/**
* @brief GDC8310 (1/4 duty, 1/3 bias LCD)
Number : SEG(n),SEG(n+1)
0 : 0xF,0xA
1 : 0x0,0xA
2 : 0xB,0xC
3 : 0x9,0xE
4 : 0x4,0xE
5 : 0xD,0x6
6 : 0xF,0x6
7 : 0xC,0xA
8 : 0xF,0xE
9 : 0xD,0xE
* @param   
* @return
*/
void LCD_TO_NUM(unsigned char dec_num, unsigned char *pu8SEG1, unsigned char *pu8SEG2)
{
	switch(dec_num)
	{
		case 0:
			*pu8SEG1 = 0x0F;
			*pu8SEG2 = 0x0A;
			break;
		case 1:
			*pu8SEG1 = 0x00;
			*pu8SEG2 = 0x0A;
			break;
		case 2:
			*pu8SEG1 = 0x0B;
			*pu8SEG2 = 0x0C;
			break;
		case 3:
			*pu8SEG1 = 0x09;
			*pu8SEG2 = 0x0E;
			break;
		case 4:
			*pu8SEG1 = 0x04;
			*pu8SEG2 = 0x0E;
			break;
		case 5:
			*pu8SEG1 = 0x0D;
			*pu8SEG2 = 0x06;
			break;
		case 6:
			*pu8SEG1 = 0x0F;
			*pu8SEG2 = 0x06;
			break;
		case 7:
			*pu8SEG1 = 0x0C;
			*pu8SEG2 = 0x0A;
			break;
		case 8:
			*pu8SEG1 = 0x0F;
			*pu8SEG2 = 0x0E;
			break;
		case 9:
			*pu8SEG1 = 0x0D;
			*pu8SEG2 = 0x0E;
			break;
		default:
			*pu8SEG1 = 0x0F;
			*pu8SEG2 = 0x0A;
			break;
	}
}


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
