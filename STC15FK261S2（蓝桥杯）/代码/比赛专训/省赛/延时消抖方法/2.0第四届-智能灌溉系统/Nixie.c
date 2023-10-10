#include "Nixie.h"
#include "Delay.h"

void Nixie_Init(unsigned char n)
{
	P2&=0X1F;//����
	
	if(n==1)//Com��ѡ
	{
		P0=0X00;//ȫ����ѡ�У��ߵ�ƽѡ�У�
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)//�����
	{
		P0=0XFF;//ȫ�����������͵�ƽ������
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,unsigned char value)
{
	Nixie_Init(1);//P2ָ��COM��
	P0|=0x01<<pos;
	Nixie_Init(2);//P2ָ������
	P0&=value;
	Delay1ms();//�ȶ���ʾ
}

void Nixie_NoDisplay(void)
{
	Nixie_Init(1);
	Nixie_Init(2);
}