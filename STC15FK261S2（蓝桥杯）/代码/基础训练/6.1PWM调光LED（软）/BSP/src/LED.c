#include "LED.h"
#include "Delay.h"


void LED_Init()
{
	P2=(P2&0x1f)|0x80;
	P0=0XFF;
}

