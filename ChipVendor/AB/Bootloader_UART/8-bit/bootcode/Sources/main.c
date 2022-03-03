#include <intrins.h>

#include "MC97FG316.h"
#include "def.h"
#include "flash.h"
#include "uart.h"

#define APPLICATION_START	0x800

void SysInit(void)
{
	SCCR = 0x14;
	_nop_();
	SCCR = 0x04;
	
	BODR = 0x03;
}

void Port_Initial(void)
{
	P0          = _0000_0000; 
	P0IO        = _0000_0000;       
    P0OD        = _0000_0000;       
	P0PU        = _0000_0000;       
	P0DB        = _0000_0000;		

                                    
	P1          = _0000_0000;                                           
	P1IO        = _0000_0000;       
    P1OD        = _0000_0000;       
	P1PU        = _0000_0000;       
    P1DB        = _0000_0000;       

                                    
	P2          = _0000_0000;                                           
	P2IO        = _0000_0000;       
    P2OD        = _0000_0000;       
	P2PU        = _0000_0000;       
    P2DB        = _0000_0000;     
	
	
	P3          = _0000_0000;       
	P3IO        = _0000_0000;       
    P3OD        = _0000_0000;       
	P3PU        = _0000_0000;       
    P3DB        = _0000_0000; 

                                    // b7   b6   b5   b4   b3   b2   b1   b0
    PSR0        = _0000_0000;       // 
    PSR1        = _0000_0000;       //
	PSR2        = _0000_0000;       // 
}

void DelayMS(unsigned int t)			// Delay Time about 1 ms;
{
	unsigned int i;
	
	do{
		for (i=0; i<700; i++); // @16Mhz
	} while(t--);
}

unsigned int PageAddress;
unsigned int RealPageAddress;

void GetPageNumber(void)
{
	unsigned char PageAddressHigh;
	unsigned char PageAddressLow;
	
	PageAddressHigh = ugetch();
	PageAddressLow = ugetch();

	RealPageAddress = ((unsigned int)PageAddressHigh)*0x100;
	RealPageAddress += PageAddressLow;

	PageAddress = ((unsigned long)RealPageAddress) << NSHIFTPAGE;
	
	uputch('!');
}

/*****************************************************************************/
//hsj unsigned char idata rxdata[PageByte];	//[0]~[31]
char WriteFlashPage(void)
{
	unsigned int i;
	unsigned char CheckSum;
	unsigned char tt;
	
	unsigned char idata rxdata[PageByte];	//[0]~[31]	//** Must be idata


	CheckSum = 0;
	
	for (i=0;i<PageByte;i++)
	{ 
		rxdata[i] = ugetch();

	}	
	tt=ugetch();
	

	for (i=0;i<PageByte;i++)
	{ 
		CheckSum += rxdata[i];
	}
		
	if (CheckSum != tt)
	{
		return 0;
	}

	if (PageAddress<0x800) // 0x0000~0x0800 is bootcode area, so we do not erase and write 
		return 1;

	Flash_SetID(FLASH_ERASE_SECURITY_ID);	
	if (Flash_Erase(PageAddress))
		return 0; //fail

	Flash_SetID(FLASH_WRITE_SECURITY_ID);	
	if (Flash_Write(PageAddress,rxdata))
		return 0; //fail		
	
	return 1;
}

DEV_INFO wrt;

void main(void)
{
	unsigned char ch;
	char cnt;
	void (*go_addr)();

	cli();

	SysInit();
	Port_Initial();
	uart_init();
	
	uputch('>'); //start protocol
	for (cnt=0;cnt<100;cnt++)
	{
		ugetchar(&ch);
		if (ch=='<') break;
			
		DelayMS(1); // about 1msec
	}
	
	if (cnt<100) // program update start
	{
		wrt.pagesize=PageSize;
		wrt.deviceid.word=DeviceID;
		wrt.flashsize=FlashSize;
		wrt.bootsize=BootSize;	
		
		uputch(wrt.pagesize);
		uputch(wrt.deviceid.byte[0]);
		uputch(wrt.deviceid.byte[1]);
		uputch(wrt.flashsize);
		uputch(wrt.bootsize);
		
		uputch('!');

		while (1)
		{
			GetPageNumber();

			if (RealPageAddress==(unsigned int)0xffff) 
			{
				_nop_();
				goto EXIT;
			}

			if (WriteFlashPage())
			{
				uputch('!');
			}
			else 
			{
				uputch('@');
			}
		}
		
EXIT:
		;										
	}
	
	go_addr = (void(code *)())(APPLICATION_START);
	go_addr();
}

