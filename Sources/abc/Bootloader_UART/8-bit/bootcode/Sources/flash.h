#ifndef _FLASH_HEADER_
#define _FLASH_HEADER_

#define PAGEBUF_SIZE  (32)			
#define MEMORY_SIZE (32*512)		

#define coderom ((unsigned char volatile code  *) 0)

#define	FLASH_WRITE_SECURITY_ID		0xF0
#define	FLASH_ERASE_SECURITY_ID		0xE0

extern char Flash_Erase(unsigned int addr);
extern char Flash_Write(unsigned int addr, unsigned char *dt);

extern void Flash_SetID(unsigned char id);
extern unsigned char flash_state_id;

#endif //_FLASH_HEADER_
