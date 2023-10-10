#include "LED.h"

void LED_Init(void)
{
	P2&=0X1F;//锁存
	P0=0XFF;	//全都不点亮
	P2=(P2&0X1F)|0X80;
}

void LED_Control(unsigned char Open)
{
	LED_Init();
	P0&=~Open;	//低电平点亮
	P2&=0X1F;//锁存
}

