#include "Nixie.h"
#include "Delay.h"

void Nixie_Init(unsigned char n)
{
	if(n==1)//Com端口清除
	{
		P2&=0X1F;//锁存
		P0=0x00;//全都不选中（高电平选中）
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)//段选清除
	{
		P2&=0X1F;//锁存
		P0=0xFF;//全都不选中（低电平选中）
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,unsigned char value)
{
	Nixie_Init(1);
	P0|=0x01<<pos;
	Nixie_Init(2);
	P0&=value;
	Delay1ms();//稳定显示
	
	//P2&=0X1F;//锁存
}

void Nixie_All(void)//防止最后一位数码管不会特别亮
{
	Nixie_Init(1);
	P0=0XFF;
	Nixie_Init(2);
	P0=0XFF;
	P2&=0X1F;//锁存
}

void Nixie_NoDisplay(void)
{
	P2&=0X1F;//锁存
	P0=0x00;//全都不选中（高电平选中）
	P2=(P2&0X1F)|0XC0;
	
	P2&=0X1F;//锁存
	P0=0xFF;//全都不选中（低电平选中）
	P2=(P2&0X1F)|0XE0;
}