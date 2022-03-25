#ifndef _A31T21x_FMC_H_
#define	_A31T21x_FMC_H_

int EraseChip(void);
int ErasePage(unsigned long target_addr);
int EraseSector1KB(unsigned long target_addr);
int EraseSector4KB(unsigned long target_addr);
int ProgramPage(unsigned long target_addr, unsigned long page_size, unsigned char *data_buffer);

#endif
