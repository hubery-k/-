#include "LED.h"

void LED_Init(void)
{
	P2&=0X1F;//����
	P0=0XFF;	//ȫ��������
	P2=(P2&0X1F)|0X80;
}

void LED_Control(unsigned char Open)
{
	LED_Init();
	P0&=~Open;	//�͵�ƽ����
	P2&=0X1F;//����
}

