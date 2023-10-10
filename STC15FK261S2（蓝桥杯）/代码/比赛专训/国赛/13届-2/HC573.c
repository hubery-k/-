#include "HC573.h"

/*LED-P0口输出低电平点亮*/
void LED_Init()
{
	P2&=0X1F;
	P0=0XFF;
	P2=(P2&0X1F)|0X80;
}
/*蜂鸣器、继电器-P0口输出高电平通过达林顿管转换成低电平点亮*/
void RB_Init()
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

uc Nixie_Buf[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
void Nixie_Init(uc n)
{
	P2&=0X1F;
	if(n==1)
	{
		P0=0X00;
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)
	{
		P0=0XFF;
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(uc pos,val)
{
	Nixie_Init(2);
	
	Nixie_Init(1);
	P0|=0X01<<pos;
	Nixie_Init(2);
	P0&=val;
	
	P2&=0X1F;
}

void Nixie_Loop()
{
	static uc i;
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	i%=8;
}

void Nixie_SetBuf(uc pos,val)
{
	Nixie_Buf[pos]=val;
}