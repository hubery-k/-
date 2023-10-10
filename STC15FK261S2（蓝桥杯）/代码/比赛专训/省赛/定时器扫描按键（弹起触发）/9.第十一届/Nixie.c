#include "Nixie.h"

uc Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

/*Com��ѡ-P0������ߵ�ƽѡ�� ��ѡ-P0������͵�ƽѡ��*/
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

void Nixie_Display(uc pos,value)
{
	Nixie_Init(2);		//����
	
	Nixie_Init(1);
	P0|=0X01<<pos;
	Nixie_Init(2);
	P0&=value;
}

void Nixie_Loop()
{
	static uc i=0;		//����Ϊ��̬��������Ϊ��Ҫ��ס״̬
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	if(i==8){i=0;}
}

void Nixie_SetBuf(uc Location,Data)
{
	Nixie_Buf[Location]=Data;
}