#include "Nixie.h"
#include "Delay.h"

void Nixie_Init(unsigned char n)
{
	P2&=0X1F;		//锁存
	if(n==1)//Com端选-高电平选中
	{
		P0=0X00;
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)//段选-低电平选中
	{
		P0=0XFF;
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,value)
{
	Nixie_Init(1);
	P0|=0X01<<pos;
	Nixie_Init(2);
	P0&=value;
	Delay1ms();
}
void Nixie_NoDisplay(void)
{
	Nixie_Init(1);
	Nixie_Init(2);
}
void Nixie_ALL(void)//防止最后一位数码管不会特别亮
{
	Nixie_Init(1);
	P0=0XFF;
	Nixie_Init(2);
	P0=0XFF;
}