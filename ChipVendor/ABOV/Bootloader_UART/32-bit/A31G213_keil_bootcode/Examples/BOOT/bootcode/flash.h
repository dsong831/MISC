#ifndef _FLASH_H_
#define	_FLASH_H_

#define	FWORD					unsigned long
#define	SECTOR_SIZE				(0x200)

int		flash_erase_sector(unsigned long addr);
int		flash_write_sector(unsigned long addr,unsigned long *pdata);

int EraseChip (void);
int EraseSector (int sel, unsigned long addr);
int ProgramPage (int sel,unsigned long addr, unsigned long size, unsigned char *buf);
unsigned long Verify (unsigned long addr, unsigned long size, unsigned char *buf);

int EraseSector1k (int sel, unsigned long addr);
int EraseSector4k (int sel, unsigned long addr);

#endif
