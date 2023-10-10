#include "HC573.h"

/*LED-P0������͵�ƽ����*/
void LED_Init(void)
{
	P2&=0X1F;
	P0=0XFF;
	P2=(P2&0X1F)|0X80;
}

/*BUZZER��RELAY-P0������ߵ�ƽͨ�����ֶٹ�ת��Ϊ�͵�ƽʹ�䵼ͨ*/
void RB_Init(void)
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

uc Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
/*SMG-��ѡ��P0����ߵ�ƽѡ�С���ѡ��P0����͵�ƽѡ��*/
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
	Nixie_Init(2);	//����
	
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