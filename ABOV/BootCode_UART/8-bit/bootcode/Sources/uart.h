#ifndef _UART_H_
#define _UART_H_

extern void uart_init(void);
extern void uputch(unsigned char ch);
extern unsigned char ugetch(void);
extern unsigned char ugetchar(unsigned char *ch);

// UCTRLx1 reg
#define UMSEL0	6		// 0 : Aync, 1 : Sync, 3 : SPI mode 
#define UPM0	4		// 0 : no parity, 2 : even parity, 3: odd parity
#define USIZE0	1		// 0 : 5bit, 1 : 6bit, 2: 7bit, 3 : 8bit, 7 : 9bit
#define UDORD	2		// 0 : LSB first, 1 : MSB first
#define UCPHA	1		// 
#define UCPOL	0		//

// UCTRLx1 reg
#define UMSEL0	6		// 0 : Aync, 1 : Sync, 3 : SPI mode 
#define UPM0	4		// 0 : no parity, 2 : even parity, 3: odd parity
#define USIZE0	1		// 0 : 5bit, 1 : 6bit, 2: 7bit, 3 : 8bit, 7 : 9bit
#define UDORD	2		// 0 : LSB first, 1 : MSB first
#define UCPHA	1		// 
#define UCPOL	0		//

// UCTRLx2 reg
#define UDRIE			7
#define TXCIE			6
#define RXCIE			5
#define WAKEIE			4
#define TXE				3
#define RXE				2
#define USARTEN			1
#define U2X				0

// UCTRL3 reg
#define MASTER			7
#define LOOPS			6
#define DISXCK			5
#define SPISS			4

#define USBS			2
#define TX8				1
#define RX8				0

// USTAT reg
#define UDRE			7
#define TXC				6
#define RXC				5
#define WAKE			4
#define SOFTRST			3
#define DOR				2
#define FE				1
#define PE				0

//OSC 16MHz
#define BAUD38400 	25
#define BAUD115200 	7

#endif	// _UART_H_
