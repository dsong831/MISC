#include "MC97FG316.h"
#include "def.h"

void SysInit(void)
{
	BODR = 0x03;
}

void Port_Initial(void)
{
	P0          = _0000_0000; 
	P0IO        = _1000_0000;       
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

void main(void)
{
	cli();

	SysInit();
	Port_Initial();
	
	sei();

	while(1)
	{
		P07 ^= 1;		
	}
}

