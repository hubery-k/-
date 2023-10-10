#include "Nixie.h"
#include "Delay.h"


void Nixie_Init(unsigned char n)
{
	if(n==1)
	{
		P2=(P2&0x1f)|0xC0;//com��
		P0=0x00;//com����0
	}
	else if(n==2)
	{
		P2=(P2&0x1f)|0xE0;//�����
		P0=0xFF;//�������0
	}
}

void DisplaySMG_Bit(unsigned char pos,unsigned char value)
{
	Nixie_Init(1);
	P0 |= 0x01 << pos;
	Nixie_Init(2);
	P0 &= value;//�͵�ƽ��Ч
}

void Nixie_show(unsigned char pos,unsigned char value)
{
	DisplaySMG_Bit(pos,value);
	Delay1ms();//�ȶ���ʾ
}