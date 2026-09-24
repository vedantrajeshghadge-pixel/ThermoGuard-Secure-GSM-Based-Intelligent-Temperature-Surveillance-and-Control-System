//uart_defines.h

#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define BAUD  9600
#define DIVISOR (PCLK/(16*BAUD))

//defines for UxLCR sfr
#define _8BITS    3
#define WORD_LEN_SEL_BITS _8BITS
#define DLAB_BIT  7

//defines for UxLSR sfr
#define DR_BIT    0
#define TEMT_BIT  6

//defines for TXD0,RXD0 pin in PINSEL0 sfr
#define TXD0_PIN_EN 0x00000001
#define RXD0_PIN_EN 0x00000004
