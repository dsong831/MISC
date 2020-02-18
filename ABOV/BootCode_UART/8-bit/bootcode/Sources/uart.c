#include <intrins.h>
#include "MC97FG316.h"
#include "def.h"
#include "uart.h"

void uart_init(void)
{
	// USART0 initialize
	// Asynchronous Mode, No Parity, 8 bit
	UCTRL1 = (0<<UMSEL0)|(0<UPM0)|(3<<USIZE0);
	// Enable transmitter, Receiver and USART
	UCTRL2 = (1<<RXCIE)|(1<<TXE)|(1<<RXE)|(1<<USARTEN);
	UCTRL3 = 0x00;
	UBAUD = BAUD38400;	// fsclk/(16*(ubaud+1)) 16000000/(16*38400) -1= 25
}
void uputch(unsigned char ch)
{
	while( !(USTAT & 0x80));
	UDATA = ch;
}

unsigned char ugetch(void)
{
	char ch;

	while( (USTAT & 0x20) == 0x00);
	ch = UDATA;
	
	return ch;
}

unsigned char ugetchar(unsigned char *ch)
{
	if( (USTAT & 0x20) == 0x20)
	{
		*ch = UDATA;
		return 1;
	}
	*ch=0;
	
	return 0;
}
