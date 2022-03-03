#include <intrins.h>

#include "MC97FG316.h"
#include "def.h"
#include "flash.h"

#define PGM_TIME 0xDA	// 3.5ms @ Fsys 16MHz
#define NOP _nop_()
#define DUMMY_FLASH_ADDRESS	0x600			// Set a dummy write area within the protection zone.

unsigned char flash_state_id = 0;

// from asm.a51
extern void Flash_LoadData(unsigned char dt, unsigned int add);
extern void Flash_Enter(void);

//==============================================================================

void Flash_Exit()
{
	FECR = 0x33;
	FEMR = 0x00;
	flash_state_id = 0;
	
	FEARL = (unsigned char)( DUMMY_FLASH_ADDRESS & 0xff);
	FEARM = (unsigned char)(( DUMMY_FLASH_ADDRESS >> 8) & 0xff);
	FEARH = 0;
}

//
// flash erase, write addr -> 0, (0+PAGEBUF_SIZE), (0+PAGEBUF_SIZE*2),...
// flash erase, write unit -> PAGEBUF_SIZE
//
char Flash_Erase(unsigned int addr)
{
	unsigned int i;
	
	// Enable program mode
	Flash_Enter();

	// erase
	FEMR = 0x81;	// Select flash and enable control.
	FECR = 0x02;	// Reset page buffer.
	FEMR = 0x89;	// Select page buffer.

	for(i=0;i<PAGEBUF_SIZE;i++){
		Flash_LoadData(0, i+0x1FE0);
	}

	FEMR = 0x91;	// Set Erase mode.
	FETCR = PGM_TIME;	// Set Erase time.
	FEARL = (unsigned char)(addr&0xff);
	FEARM = (unsigned char)((addr>>8)&0xff);
	FEARH = 0;
	
	if(flash_state_id != FLASH_ERASE_SECURITY_ID)
	{
		Flash_Exit();
	
		return -1;
	}
	
	FECR = 0x0B;	// Start Erase.
	_nop_();
	_nop_();
	_nop_();

	while( !( FESR & 0x80));		// Read status register.
 
	Flash_Exit();
	
	// read data  
	for(i=0; i<PAGEBUF_SIZE;i++){	// Read flash data.
		if(coderom[addr] != 0x00) 
      		return -1;

		addr++;
	}
	return 0;
}

char Flash_Write(unsigned int addr, unsigned char *dt)
{
	unsigned int i;
	unsigned char *pdt;
	
	// Enable program mode
	Flash_Enter();
	
	// write
	FEMR = 0x81;	// Select flash and enable control.
	FECR = 0x02;	// Reset page buffer.
	FEMR = 0x89;	// Select page buffer.

	pdt = dt;
	
	for(i=0;i<PAGEBUF_SIZE;i++){
		Flash_LoadData(*pdt, i+0x1FE0);
		pdt++;
	}

	FEMR = 0xA1;			// Set write mode.
	FETCR = PGM_TIME;		// Set program time.
	
	FEARL = (unsigned char)(addr&0xff);
	FEARM = (unsigned char)((addr>>8)&0xff);
	FEARH = 0x00;
	
	if(flash_state_id != FLASH_WRITE_SECURITY_ID)
	{
		Flash_Exit();
		
		return -1;
	}
	
	FECR = 0x0B;			// Start program.
	_nop_();
	_nop_();
	_nop_();

	while (!( FESR & 0x80));
 
	Flash_Exit();
	
	// read data
	pdt = dt;
	for(i=0;i<PAGEBUF_SIZE;i++)		// Read flash data.
	{	
		if (coderom[addr]!=*pdt) 
	  		return -2;
		
		addr++;
		pdt++;
	}
	
	return 0;
}

void Flash_SetID(unsigned char id)
{
	flash_state_id = id;
}