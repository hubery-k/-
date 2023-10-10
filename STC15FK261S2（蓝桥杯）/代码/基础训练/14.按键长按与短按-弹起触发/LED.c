#include "LED.h"

void LED_Init()
{
	P2&=0x1f;
	P0=0XFF;
	P2=(P2&0x1f)|0x80;
}
	
void LED_Control(unsigned char Open)
{
	LED_Init();
	P2=(P2&0x1f)|0x80;
	P0&=~Open;
	P2&=0x1f;
}