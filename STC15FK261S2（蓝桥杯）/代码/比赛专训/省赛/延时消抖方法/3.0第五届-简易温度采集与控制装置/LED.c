#include "LED.h"

void LED_Init(void)
{
	P2&=0X1F;//锁存
	P0=0XFF;	//全都不点亮
	P2=(P2&0X1F)|0X80;
}

void LED_Open(unsigned char Open)
{
	LED_Init();
	P0&=~Open;	//低电平点亮
	P2=(P2&0X1F)|0X80;//由于调用了的Init函数中有573通往LED。所以这句可写可不写

}

void LED_Close(unsigned char Close)
{
	LED_Init();
	P0|=Close;	//高电平熄灭
	P2=(P2&0X1F)|0X80;//可写可不写
}