#include "Nixie.h"

uc Nixie_Nuf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
void Nixie_Init(uc n)
{
	P2&=0X1F;
	if(n==1)
	{
		P0=0x00;
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)
	{
		P0=0xFF;
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(uc pos,val)
{
	Nixie_Init(2);		//ÏûÒþ
	
	Nixie_Init(1);
	P0|=0X01<<pos;
	Nixie_Init(2);
	P0&=val;
	
	P2&=0X1F;
}

void Nixie_Loop()
{
	static uc i;
	Nixie_Display(i,Nixie_Nuf[i]);
	i++;
	if(i==8){i=0;}
}

void Nixie_SetBuf(uc Loc,val)
{
	Nixie_Nuf[Loc]=val;
}