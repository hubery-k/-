#include "Nixie.h"
#include "Delay.h"


//数码管显示缓存区
unsigned char Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

void Nixie_SetBuf(unsigned char Location,Number)
{
	Nixie_Buf[Location]=Number;
}

void Nixie_Init(unsigned char n)
{
	P2&=0X1F;		//锁存
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

void DisplaySMG(unsigned char pos,unsigned char value)
{
	Nixie_Init(2);		//消隐
	
	Nixie_Init(1);
	P0 |= 0x01 << pos;
	Nixie_Init(2);
	P0 &= value;//低电平有效
}

void Nixie_Loop(void)
{
	static unsigned char i=0;
	DisplaySMG(i,Nixie_Buf[i]);
	i++;
	if(i>=8){i=0;}
}