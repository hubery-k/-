#include "Nixie.h"
#include "Delay.h"

void Nixie_Init(unsigned char n)
{
	if(n==1)//Com�˿����
	{
		P2&=0X1F;//����
		P0=0x00;//ȫ����ѡ�У��ߵ�ƽѡ�У�
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)//��ѡ���
	{
		P2&=0X1F;//����
		P0=0xFF;//ȫ����ѡ�У��͵�ƽѡ�У�
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,unsigned char value)
{
	Nixie_Init(1);
	P0|=0x01<<pos;
	Nixie_Init(2);
	P0&=value;
	Delay1ms();//�ȶ���ʾ
	
	//P2&=0X1F;//����
}

void Nixie_All(void)//��ֹ���һλ����ܲ����ر���
{
	Nixie_Init(1);
	P0=0XFF;
	Nixie_Init(2);
	P0=0XFF;
	P2&=0X1F;//����
}

void Nixie_NoDisplay(void)
{
	P2&=0X1F;//����
	P0=0x00;//ȫ����ѡ�У��ߵ�ƽѡ�У�
	P2=(P2&0X1F)|0XC0;
	
	P2&=0X1F;//����
	P0=0xFF;//ȫ����ѡ�У��͵�ƽѡ�У�
	P2=(P2&0X1F)|0XE0;
}