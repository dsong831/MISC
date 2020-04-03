
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
		default :
			u8Data = 0xFF;
			break;
	}
	return u8Data;
}
