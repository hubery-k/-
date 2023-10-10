#include "LED.h"

/*P0口输出低电平点亮*/
void LED_Init(void)
{
	P2&=0X1F;		//锁存
	P0=0XFF;
	P2=(P2&0X1F)|0X80;
}

void LED_Open(unsigned char Open)
{
	LED_Init();
	P0&=~Open;
	P2=(P2&0X1F)|0X80;
}

void LED_Close(unsigned char Close)
{
	LED_Init();
	P0|=Close;
	P2=(P2&0X1F)|0X80;
}

