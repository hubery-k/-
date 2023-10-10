#include "Nixie.h"
#include "Delay.h"

void Nixie_Init(unsigned char n)
{
	P2&=0X1F;//锁存
	
	if(n==1)//Com端选
	{
		P0=0X00;//全都不选中（高电平选中）
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)//段码端
	{
		P0=0XFF;//全都不点亮（低电平点亮）
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,unsigned char value)
{
	Nixie_Init(1);//P2指向COM端
	P0|=0x01<<pos;
	Nixie_Init(2);//P2指向段码端
	P0&=value;
	Delay1ms();//稳定显示
}

void Nixie_NoDisplay(void)
{
	Nixie_Init(1);
	Nixie_Init(2);
}