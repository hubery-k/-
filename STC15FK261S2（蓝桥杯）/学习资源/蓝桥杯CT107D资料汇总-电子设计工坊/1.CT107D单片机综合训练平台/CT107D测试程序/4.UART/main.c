#include "at89x52.h"

unsigned char txbuffer1[] = "STC89C52RC UART TEST PROGRAM";
unsigned char txbuffer2[] = "============================";
unsigned char enter[] = {0x0A,0x0D};

void uart_tx(unsigned char *p,unsigned char length);

void main()
{
	SCON = 0x50;
	TMOD = 0x20;
	TH1 = 0xF3;
	TL1 = 0xF3;
	TR1 = 1;

	uart_tx(txbuffer2,28);
	uart_tx(enter,2);            //Enter
	uart_tx(enter,2);
	uart_tx(enter,2);
	uart_tx(txbuffer1,28);
	uart_tx(enter,2);            //Enter
	uart_tx(enter,2);
	uart_tx(enter,2);
	uart_tx(txbuffer2,28);
	uart_tx(enter,2);            //Enter
	uart_tx(enter,2);
	uart_tx(enter,2);

	while(1);
}

void uart_tx(unsigned char *p,unsigned char length)
{
	unsigned char i;
	for(i=0;i<length;i++)
	{
		SBUF = *(p+i);
		while(TI == 0);
		TI = 0;
	}
}