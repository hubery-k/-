#include "Nixie.h"

/*Com端-高电平选中 段选-低电平选中*/
//数据缓冲区
uc Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

void Nixie_Init(uc n)
{
	P2&=0X1F;
	if(n==1)		//Com端选
	{
		P0=0X00;
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)		//段选
	{
		P0=0XFF;
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(uc pos,val)
{
	Nixie_Init(2);		//消隐
	
	Nixie_Init(1);
	P0|=0X01<<pos;
	Nixie_Init(2);
	P0&=val;
	
	P2&=0X1F;
}

void Nixie_Loop(void)
{
	static uc i=0;
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	if(i>8){i=0;}
}

void Nixie_SetBuf(uc Location,Data)
{
	Nixie_Buf[Location]=Data;
}