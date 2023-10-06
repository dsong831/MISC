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


//#define sREG_SHIFT(Register,Field)			Register##_##Field##_SHIFT
//#define sREG_MASK(Register,Field)			Register##_##Field##_MASK
//#define sREG_FIELD_DATA(Register,Field,Data) 		(((unsigned char)(Data) << sREG_SHIFT(Register##,Field##)) & (sREG_MASK(Register##,Field##)))
#define sREG_FIELD_DATA(Register,Shift,Mask,Data) 	(((unsigned char)(Data) << Shift) & (Mask))

#define READ_WORD(Register)				(*(volatile unsigned int *)(Register))
#define READ_BYTE(Register)				Function_ReadByte(Register)
#define READ_FIELD(Register,Shift,Mask)			Function_ReadField(Register, Shift, Mask)
//#define READ_FIELD(Register,Field)			Function_ReadField(Register, sREG_SHIFT(Register##,Field##), sREG_MASK(Register##,Field##))

#define WRITE_WORD(Register,Data)			do { *(volatile unsigned int *)(Register) = Data; } while(0)
#define WRITE_BYTE(Register,Data)			do { *(volatile unsigned char *)(Register) = Data; } while(0)
#define WRITE_BYTE_UPDATE(Register,Data)		Function_WriteByteUpdate(Register, Data)
#define WRITE_FIELD(Register,Shift,Mask,Data)		Function_WriteField(Register, Shift, Mask, Data)
//#define WRITE_FIELD(Register,Field,Data)		Function_WriteField(Register, sREG_SHIFT(Register##,Field##), sREG_MASK(Register##,Field##), Data)

#define WRITE_BIT32(Register,Shift,Data)		do { (*(volatile unsigned int *)(Register)) &= ~(1 << Shift); (*(volatile unsigned int *)(Register)) |= (Data << Shift); } while(0)
#define WRITE_BIT8(Register,Shift,Data)			do { (*(volatile unsigned char *)(Register)) &= ~(1 << Shift); (*(volatile unsigned char *)(Register)) |= (Data << Shift); } while(0)

unsigned char Function_ReadByte(unsigned int reg_address);
unsigned char Function_ReadField(unsigned int reg_address, unsigned char field_shift, unsigned char field_mask);
void Function_WriteByteUpdate(unsigned int reg_address, unsigned char data);
void Function_WriteField(unsigned int reg_address, unsigned char field_shift, unsigned char field_mask, unsigned char data);


/* Template */
#define TEMPLATE_REG					(0x40030000)
#define TEMPLATE_REG_field0_SHIFT			(0)
#define TEMPLATE_REG_field0_MASK			(0x1 << TEMPLATE_REG_field0_SHIFT)
#define TEMPLATE_REG_field4_SHIFT			(4)
#define TEMPLATE_REG_field4_MASK			(0xF << TEMPLATE_REG_field4_SHIFT)

#endif /*COMMON_H*/

