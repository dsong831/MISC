#include "A31G21x.h"
#include "aa_types.h"
#include "flash.h"

#define	FMCFG_ENTRY				0x78580300
#define	FMCFG_EXIT				0x78580300

#define	BV_LOCKSEL			(1<<24)
#define	BV_SELFPGM			(1<<23)
#define	BV_TIMEREN			(1<<20)
#define	BV_CFGRDEN			(1<<15)
#define	BV_IFEN					(1<<12)
#define	BV_BBLOCK				(1<<8)
//#define	BV_AE						(1<<7)
#define	BV_MAS					(1<<7)
#define	BV_4KEN					(1<<6)
#define	BV_1KEN					(1<<5)
#define	BV_PMOD				(1<<4)
#define	BV_WADCK				(1<<3)
#define	BV_PGM					(1<<2)
#define	BV_ERS					(1<<1)
#define	BV_HVEN					(1<<0)

#define	FMMR_BUSY				(FMC->BUSY&0x01)	//__bita(FMBUSY,0)

#define	FMCR_LOCKSEL_SET			(FMC->CR|= BV_LOCKSEL)	
#define	FMCR_LOCKSEL_CLR			(FMC->CR&=~BV_LOCKSEL)
#define	FMCR_SELFPGM_SET			(FMC->CR|= BV_SELFPGM)
#define	FMCR_SELFPGM_CLR			(FMC->CR&=~BV_SELFPGM)
#define	FMCR_TIMEREN_SET			(FMC->CR|= BV_TIMEREN)
#define	FMCR_TIMEREN_CLR			(FMC->CR&=~BV_TIMEREN)
#define	FMCR_CFGRDEN_SET			(FMC->CR|= BV_CFGRDEN)
#define	FMCR_CFGRDEN_CLR			(FMC->CR&=~BV_CFGRDEN)
#define	FMCR_IFEN_SET			(FMC->CR|= BV_IFEN)		//__bita(FMCR,12)
#define	FMCR_IFEN_CLR			(FMC->CR&=~BV_IFEN)		//__bita(FMCR,12)
#define	FMCR_BBLOCK_SET			(FMC->CR|= BV_BBLOCK)			//__bita(FMCR,7)
#define	FMCR_BBLOCK_CLR			(FMC->CR&=~BV_BBLOCK)			//__bita(FMCR,7)
#define	FMCR_MAS_SET				(FMC->CR|= BV_MAS)			//__bita(FMCR,7)
#define	FMCR_MAS_CLR				(FMC->CR&=~BV_MAS)			//__bita(FMCR,7)
#define	FMCR_4KEN_SET			(FMC->CR|= BV_4KEN)		//__bita(FMCR,6)
#define	FMCR_4KEN_CLR			(FMC->CR&=~BV_4KEN)		//__bita(FMCR,6)
#define	FMCR_1KEN_SET			(FMC->CR|= BV_1KEN)		//__bita(FMCR,5)
#define	FMCR_1KEN_CLR			(FMC->CR&=~BV_1KEN)		//__bita(FMCR,5)
#define	FMCR_PMOD_SET			(FMC->CR|= BV_PMOD)		//__bita(FMCR,4)
#define	FMCR_PMOD_CLR			(FMC->CR&=~BV_PMOD)		//__bita(FMCR,4)
#define	FMCR_WADCK_SET			(FMC->CR|= BV_WADCK)		//__bita(FMCR,3)
#define	FMCR_WADCK_CLR			(FMC->CR&=~BV_WADCK)		//__bita(FMCR,3)
#define	FMCR_PGM_SET			(FMC->CR|= BV_PGM)			//__bita(FMCR,2)
#define	FMCR_PGM_CLR			(FMC->CR&=~BV_PGM)			//__bita(FMCR,2)
#define	FMCR_ERS_SET			(FMC->CR|= BV_ERS)			//__bita(FMCR,1)
#define	FMCR_ERS_CLR			(FMC->CR&=~BV_ERS)			//__bita(FMCR,1)
#define	FMCR_HVEN_SET			(FMC->CR|= BV_HVEN)		//__bita(FMCR,0)
#define	FMCR_HVEN_CLR			(FMC->CR&=~BV_HVEN)		//__bita(FMCR,0)


/**
********************************************************************************************************
* @ Name : Init
*
* @ Parameter
*		addr		device base address
*		clk			clock frequency (Hz)
*		func		function code (1=erase, 2=program, 3=verify)
*
* @ return value
*		0 = success
*		1 = error 
*
********************************************************************************************************
*/
void	__SET_OTP(unsigned long otp)
{
	FMC->MR = 0x5A;
	FMC->MR = 0xA5;
	if(otp==0) {
		FMC->MR = 0;
	}
	else {
		FMC->MR = (otp<<28);// | BV_OTPE;
		FMCR_IFEN_SET;
		FMCR_4KEN_CLR;
		//FMCR_1KEN_CLR;
		//FMCR_BBLOCK_SET;
	}
}

int Init (unsigned long addr, unsigned long clk, unsigned long func)
{
	__disable_irq();
	WDT->CR = (0x5A69 << 16)
            |(0x25<<10)
            |(0x1A<<4)
            |0;
	
	//---------------------------------------------------------------------------------
	// 1. ALL clock enable 
	//---------------------------------------------------------------------------------
	SCU->CSCR = (0xA507UL<<16)
            |(0x8<<12)
            |(0x8<<8)
            |(0x8<<4)   // 48Mhz   
            |(0x8<<0); 
    
	SCU->SCCR = (0x570AUL<<16)
            |(0x02);   // HSIOSC enable
            
	return (0);
}


/**
********************************************************************************************************
* @ Name : UnInit
*
* @ Parameter
*		func		function code (1=erase, 2=program, 3=verify)
*
* @ return value
*		0 = success
*		1 = error 
*
********************************************************************************************************
*/
int UnInit (unsigned long func)
{
	return (0);
}

void	__WAIT(void)
{
	while(1) {
		// wait until MR_BUSYE is "0"
		if (FMMR_BUSY==0) 
			break; 
	}
}

void	__FLASH_ALL_ENABLE(void)
{
	FMC->MR = 0x66;
	FMC->MR = 0x99;
	FMC->WPROT = 0x00000000;	// Remove FLASH write protection
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

/**
********************************************************************************************************
* @ Name : EraseChip 
*
*
* @ return value
*		0 = success
*		1 = error (time-out error)
*
********************************************************************************************************
*/
int EraseChip (void)
{
#if 0					//Erase Lock
	__SET_OTP(0x08);
	EraseSector(0);
	__SET_OTP(0x00);
#endif	
	
	__FLASH_ALL_ENABLE();

	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x5A;FMC->MR = 0xA5;

	FMCR_PMOD_SET;
	FMCR_MAS_SET;
	FMCR_ERS_SET;
	FMCR_WADCK_SET; 
	FMCR_HVEN_SET; 
	
	__WAIT();					// Chip Erase
	
	FMCR_HVEN_CLR;
	FMC->BUSY=0x80; // WRBUSY flag clear
	FMCR_ERS_CLR;
	FMCR_MAS_CLR; 
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x81;FMC->MR = 0x28;	
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	return (0); 									   // Success
}



/**
********************************************************************************************************
* @ Name : EraseSector
*
*
* @ Description

*
********************************************************************************************************
*/
int EraseSector (int sel, unsigned long addr)
{
	__FLASH_ALL_ENABLE();
	
	FMC->AR = addr>>2;

	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
// flash mode entry 
	FMC->MR = 0x5A;FMC->MR = 0xA5;
	
	FMCR_PMOD_SET;
	FMCR_ERS_SET;	
	if (sel!=0){
		FMCR_IFEN_SET;   // FMCR_IFEN_CLR
	}
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;
	
	__WAIT();
	
	FMCR_HVEN_CLR;
	FMC->BUSY=0x80; // WRBUSY flag clear
	FMCR_ERS_CLR;	
	if (sel!=0){
		FMCR_IFEN_CLR;   // FMCR_IFEN_CLR
	}
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x81;FMC->MR = 0x28;	
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	return (0); 
}

/**
********************************************************************************************************
* @ Name : EraseSector1k
*
*
* @ Description

*
********************************************************************************************************
*/
int EraseSector1k (int sel, unsigned long addr)
{
	__FLASH_ALL_ENABLE();
	
	FMC->AR = addr>>2;

	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
// flash mode entry 
	FMC->MR = 0x5A;FMC->MR = 0xA5;
	
	FMCR_PMOD_SET;
	FMCR_ERS_SET;	
	FMCR_1KEN_SET;
	if (sel!=0){
		FMCR_IFEN_SET;   // FMCR_IFEN_CLR
	}
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;
	
	__WAIT();
	
	FMCR_HVEN_CLR;
	FMC->BUSY=0x80; // WRBUSY flag clear
	FMCR_1KEN_CLR;
	FMCR_ERS_CLR;	
	if (sel!=0){
		FMCR_IFEN_CLR;   // FMCR_IFEN_CLR
	}
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x81;FMC->MR = 0x28;	
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	return (0); 
}

/**
********************************************************************************************************
* @ Name : EraseSector4k
*
*
* @ Description

*
********************************************************************************************************
*/
int EraseSector4k (int sel, unsigned long addr)
{
	__FLASH_ALL_ENABLE();
	
	FMC->AR = addr>>2;

	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
// flash mode entry 
	FMC->MR = 0x5A;FMC->MR = 0xA5;
	
	FMCR_PMOD_SET;
	FMCR_ERS_SET;
	FMCR_4KEN_SET;
	if (sel!=0){
		FMCR_IFEN_SET;   // FMCR_IFEN_CLR
	}
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;
	
	__WAIT();
	
	FMCR_HVEN_CLR;
	FMC->BUSY=0x80; // WRBUSY flag clear
	FMCR_4KEN_CLR;
	FMCR_ERS_CLR;	
	if (sel!=0){
		FMCR_IFEN_CLR;   // FMCR_IFEN_CLR
	}
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x81;FMC->MR = 0x28;	
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	return (0); 
}

/**
********************************************************************************************************
* @ Name : ProgramPage
*
*
* @ Parameters
*		addr				start address of flash memory to be written (in terms of byte)
*		size				write size (in terms of byte)
*		buf				start address of buffer 
*
*
* @ return value
*		0 = success
*		1 = error (time-out error, alignment error)
*
********************************************************************************************************
*/
int ProgramPage (int sel,unsigned long addr, unsigned long size, unsigned char *buf)
{
	int	i;
	unsigned long *ptr;
	
	//---------------------------------------------------------------------------------
	// check address and size alignment
	//---------------------------------------------------------------------------------
	if((addr & 0x03) || (size & 0x03)) {
		return (1); 
	}

	ptr = (unsigned long*)buf;

	__FLASH_ALL_ENABLE();
	
	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;	

	// flash mode entry 
	FMC->MR = 0x5A;FMC->MR = 0xA5;

	FMCR_PMOD_SET;
	if (sel!=0){
		FMCR_IFEN_SET;   // FMCR_IFEN_CLR
	}
	FMC->AR = addr>>2;
	FMCR_PGM_SET;

	for(i=0;i<size;i+=4) {
		FMC->DR = *ptr++;
		FMCR_WADCK_SET;
		FMCR_HVEN_SET;
		
		__WAIT();
		
		FMCR_HVEN_CLR;
		FMC->BUSY=0x80; // WRBUSY flag clear
	}
	
	FMCR_PGM_CLR;
	if (sel!=0){
		FMCR_IFEN_CLR;   // FMCR_IFEN_CLR
	}	
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x81;FMC->MR = 0x28;	
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	return 0;
}



/**
********************************************************************************************************
* @ Name : Verify
*
*
* @ Parameters
*		addr				start address of flash memory to be written (in terms of byte)
*		size				write size (in terms of byte)
*		buf				start address of buffer 
*
*
* @ return value
*		(addr+size)		success
*		other value		error 
*
********************************************************************************************************
*/
unsigned long Verify (unsigned long addr, unsigned long size, unsigned char *buf)
{
	unsigned long	i;
	unsigned long	*src;
	unsigned long	*dst;
	unsigned long	result; 

	//---------------------------------------------------------------------------------
	// check address and size alignment
	//---------------------------------------------------------------------------------
	if((addr & 0x03) || (size & 0x03)) {
		return (1); 
	}

	//---------------------------------------------------------------------------------
	// Verify
	//---------------------------------------------------------------------------------
	result = addr + size; 

	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x5A;
	FMC->MR = 0xA5;

//	FMCR_PVER_SET;					//PGM-level Verify

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}
//	FMCR_PVER_CLR;
//	FMCR_EVER_SET;					//ERASE-level Verify

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}
//	FMCR_EVER_CLR;

	FMC->CR = 0x00000000;
	
	FMC->MR = 0x81;FMC->MR = 0x28;		
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;

	return (result);
}


//-------------------------------------------------------
int	flash_erase_sector(unsigned long addr)
{
	int	result;

	result = EraseSector(0, addr);
	
	return result;
}

int	flash_write_sector(unsigned long addr,FWORD *pdata)
{
	int	result;
	unsigned char	*fbuffer;

	fbuffer = (unsigned char *)(pdata);
	result = ProgramPage(0,addr,SECTOR_SIZE,fbuffer);
	
	return result;
}
