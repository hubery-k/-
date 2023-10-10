#include "LED.h"
#include "Delay.h"

void LED_Init()
{
	P2=(P2&0x1f)|0x80;
	P0=0XFF;
}

void LED_Run(void)
{
	unsigned char i;
	LED_Init();
	for(i=1;i<=8;i++)
	{
		P0=0xff<<i;
		Delay500ms();
	}
	
	for(i=1;i<=8;i++)
	{
		P0=~(0xff<<i);
		Delay500ms();
	}
}
