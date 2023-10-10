#include "HC573.h"

/*LED-P0口输出低电平点亮*/
void LED_Init(void)
{
	P2&=0X1F;
	P0=0XFF;
	P2=(P2&0X1F)|0X80;
}

/*BUZZER、RELAY-P0口输出高电平通过达林顿管转换为低电平使其导通*/
void RB_Init(void)
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

uc Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
/*SMG-端选：P0输出高电平选中。段选：P0输出低电平选中*/
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
	Nixie_Init(2);	//消隐
	
	Nixie_Init(1);
	P0|=0X01<<pos;
	Nixie_Init(2);
	P0&=val;
	
	P2&=0X1F;
}

void Nixie_Loop(void)
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