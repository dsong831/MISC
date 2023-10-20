#ifndef COMMON_H
#define COMMON_H

typedef enum
{
	false = 0,
	true = 1
} bool;

typedef unsigned int DeviceId;
typedef unsigned int sERR_Code;

#define sERR_SUCCESS              0  /* success (always zero) */
#define sERR_NOT_INITIALIZED      1  /* parameter not initialized */
#define sERR_INVALID_PARAMETER    2  /* parameter is invalid */
#define sERR_NOT_SUPPORTED 	  3  /* requested feature is not supported */
#define sERR_UNKNOWN              4  /* unknown */
#define sERR_NOT_AVAILABLE        5  /* no resource available */
#define sERR_NOT_IMPLEMENTED	  6  /* function is not implemented yet*/
#define sERR_TIME_OUT		  7  /* time out*/

#define INFINITE_TIMEOUT	  0xFFFFFFFF


#define sREG_SHIFT(Register,Field)			TEMPLATE_##Register##_##Field##_SHIFT
#define sREG_MASK(Register,Field)			TEMPLATE_##Register##_##Field##_MASK
#define sREG_FIELD_DATA(Register,Field,Data)		(((unsigned char)(Data) << sREG_SHIFT(Register,Field)) & (sREG_MASK(Register,Field)))

#define READ_WORD(Address)				(*(volatile unsigned int *)(Address))
#define READ_BYTE(Address)				Function_ReadByte(Address)
#define READ_FIELD(Address,Register,Field)		Function_ReadField(Address, sREG_SHIFT(Register,Field), sREG_MASK(Register,Field))

#define WRITE_WORD(Address,Data)			do { *(volatile unsigned int *)(Address) = Data; } while(0)
#define WRITE_BYTE(Address,Data)			do { *(volatile unsigned char *)(Address) = Data; } while(0)
#define WRITE_BYTE_UPDATE(Address,Data)			Function_WriteByteUpdate(Address, Data)
#define WRITE_FIELD(Address,Register,Field,Data)	Function_WriteField(Address, sREG_SHIFT(Register,Field), sREG_MASK(Register,Field), Data)

#define WRITE_BIT32(Address,Shift,Data)			do { (*(volatile unsigned int *)(Address)) &= ~(1 << Shift); (*(volatile unsigned int *)(Address)) |= (Data << Shift); } while(0)
#define WRITE_BIT8(Address,Shift,Data)			do { (*(volatile unsigned char *)(Address)) &= ~(1 << Shift); (*(volatile unsigned char *)(Address)) |= (Data << Shift); } while(0)

unsigned char Function_ReadByte(unsigned int reg_address);
unsigned char Function_ReadField(unsigned int reg_address, unsigned char field_shift, unsigned char field_mask);
void Function_WriteByteUpdate(unsigned int reg_address, unsigned char data);
void Function_WriteField(unsigned int reg_address, unsigned char field_shift, unsigned char field_mask, unsigned char data);


/* Template */
#define CONFIG_REG					CONFIG_REG
#define ADDR_CONFIG_REG					(0x40000000UL)
#define TEMPLATE_CONFIG_REG_field0_SHIFT		(0)
#define TEMPLATE_CONFIG_REG_field0_MASK			(0x1 << TEMPLATE_CONFIG_REG_field0_SHIFT)
#define TEMPLATE_CONFIG_REG_field4_SHIFT		(4)
#define TEMPLATE_CONFIG_REG_field4_MASK			(0xF << TEMPLATE_CONFIG_REG_field4_SHIFT)

#endif /*COMMON_H*/
