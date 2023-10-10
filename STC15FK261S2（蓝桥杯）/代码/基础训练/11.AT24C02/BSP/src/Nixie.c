#include "Nixie.h"
#include "Delay.h"


void Nixie_Init(unsigned char n)
{
	if(n==1)
	{
		P2=(P2&0x1f)|0xC0;//com端
		P0=0x00;//com端清0
	}
	else if(n==2)
	{
		P2=(P2&0x1f)|0xE0;//段码端
		P0=0xFF;//段码端清0
	}
}

void DisplaySMG_Bit(unsigned char pos,unsigned char value)
{
	Nixie_Init(1);
	P0 |= 0x01 << pos;
	Nixie_Init(2);
	P0 &= value;//低电平有效
}

void Nixie_show(unsigned char pos,unsigned char value)
{
	DisplaySMG_Bit(pos,value);
	Delay1ms();//稳定显示
}