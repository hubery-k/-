#include "LED.h"

void LED_Init(void)
{
	P2&=0X1F;//锁存
	P0=0XFF;//高电平不点亮
	P2=(P2&0X1F)|0X80;
}
void LED_Open(unsigned char Open)
{
	LED_Init();
	P0&=~Open;//低电平点亮
	P2=(P2&0X1F)|0X80;
}

void LED_Close(unsigned char Close)
{
	P2&=0X1F;//锁存
	P0|=Close;//高电平不点亮
	P2=(P2&0X1F)|0X80;
}