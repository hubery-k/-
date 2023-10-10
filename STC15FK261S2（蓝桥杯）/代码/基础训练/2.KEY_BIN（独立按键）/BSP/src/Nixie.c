#include "Nixie.h"
#include "Delay.h"
unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
     0xbf,0x7f};
	

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

void Nixie_show(void)
{
	DisplaySMG_Bit(0,0XC7);
	Delay5ms();
	DisplaySMG_Bit(1,SMG_duanma[14]);
	Delay5ms();
	DisplaySMG_Bit(2,SMG_duanma[13]);
	Delay5ms();
}