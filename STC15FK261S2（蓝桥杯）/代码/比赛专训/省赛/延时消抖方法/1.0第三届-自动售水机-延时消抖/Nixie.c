#include "Nixie.h"
#include "Delay.h"
void Nixie_Init(unsigned char n)
{
	P2&=0X1F;//����
	
	if(n==1)
	{
		P0=0x00;//com����0
		P2=(P2&0x1f)|0xC0;//com��
		
	}
	else if(n==2)
	{
		P0=0xFF;//�������0
		P2=(P2&0x1f)|0xE0;//�����
	}
}

void Nixie_Display(unsigned char pos,unsigned char value)
{
	Nixie_Init(1);
	P0 |= 0x01 << pos;
	Nixie_Init(2);
	P0 &= value;//�͵�ƽ��Ч
	Delay1ms();//�ȶ���ʾ
}


void Nixie_NoDisplay()
{
	P2&=0X1F;//����
	//��ѡ
	P0=0x00;	//����ѡ��
	P2=(P2&0X1F)|0XC0;
	
	
	P2&=0X1F;//����
	//λѡ
	P0=0xFF;	//������
	P2=(P2&0X1F)|0XE0;
}