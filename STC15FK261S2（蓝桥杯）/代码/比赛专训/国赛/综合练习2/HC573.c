#include "HC573.h"

void LED_Init()
{
	P2&=0X1F;
	P0=0XFF;
	P2=(P2&0X1F)|0X80;
}

/*蜂鸣器、继电器--P0输出高电平通过达林顿管转换为低电平使其导通*/
void RB_Init()
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}


uc Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

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
	P0|=0x01<<pos;
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