#include "Nixie.h"

/*COM端-高电平选中 段码端-低电平选中*/
unsigned char Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

void Nixie_Init(unsigned char n)
{
	P2&=0X1F;
	if(n==1)		//COM端
	{
		P0=0X00;
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)		//段码端
	{
		P0=0XFF;
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,value)
{
	Nixie_Init(2);		//消隐
	
	Nixie_Init(1);
	P0|=0X01<<pos;
	Nixie_Init(2);
	P0&=value;
}

void Nixie_Loop(void)
{
	static unsigned char i=0;
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	if(i==8){i=0;}
}

void Nixie_SetBuf(unsigned char Location,Data)
{
	Nixie_Buf[Location]=Data;
}

