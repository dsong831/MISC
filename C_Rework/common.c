#include "common.h"

unsigned char Function_ReadByte(unsigned int reg_address)
{
	unsigned int word_address = reg_address & 0xFFFFFFFC;
	unsigned int word_data = *(volatile unsigned int *)word_address;

	switch(reg_address & 0x3)
	{
		case 0: word_data &= 0x000000FF; word_data = (word_data >> 0); break;
		case 1: word_data &= 0x0000FF00; word_data = (word_data >> 8); break;
		case 2: word_data &= 0x00FF0000; word_data = (word_data >> 16); break;
		case 3: word_data &= 0xFF000000; word_data = (word_data >> 24); break;
		default: break;
	}
	return (unsigned char)word_data;
}

unsigned char Function_ReadField(unsigned int reg_address, unsigned char field_shift, unsigned char field_mask)
{
	return ((Function_ReadByte(reg_address) & field_mask) >> field_shift);
}

void Function_WriteByteUpdate(unsigned int reg_address, unsigned char data)
{
	unsigned int word_address = reg_address & 0xFFFFFFFC;
	unsigned int word_data = *(volatile unsigned int *)word_address;

	switch(reg_address & 0x3)
	{
		case 0: word_data &= 0xFFFFFF00; word_data += (data << 0); break;
		case 1: word_data &= 0xFFFF00FF; word_data += (data << 8); break;
		case 2: word_data &= 0xFF00FFFF; word_data += (data << 16); break;
		case 3: word_data &= 0x00FFFFFF; word_data += (data << 24); break;
		default: break;
	}

	*(volatile unsigned int *)(word_address) = word_data;
}

void Function_WriteField(unsigned int reg_address, unsigned char field_shift, unsigned char field_mask, unsigned char data)
{
	unsigned char write_data = Function_ReadByte(reg_address);

	write_data &= ~(field_mask);
	write_data |= ((data << field_shift) & field_mask);

	Function_WriteByteUpdate(reg_address, write_data);
}

