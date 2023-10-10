#include "Nixie.h"
#include "Delay.h"

void Nixie_Init(unsigned char n)
{
	P2&=0X1F;
	if(n==1)
	{
		P0=0x00;//com端清0
		P2=(P2&0x1f)|0xC0;//com端
	}
	else if(n==2)
	{
		P0=0xFF;//段码端清0
		P2=(P2&0x1f)|0xE0;//段码端
	}
}

void DisplaySMG(unsigned char pos,value)
{
	Nixie_Init(2);		//消隐
	
	Nixie_Init(1);
	P0 |= 0x01 << pos;
	Nixie_Init(2);
	P0 &= value;//低电平有效
	
	Delay1ms();//稳定显示
}
