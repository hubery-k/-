#include "LED.h"

void LED_Init(void)
{
	P2&=0X1F;//����
	P0=0XFF;//�ߵ�ƽ������
	P2=(P2&0X1F)|0X80;
}
void LED_Open(unsigned char Open)
{
	LED_Init();
	P0&=~Open;//�͵�ƽ����
	P2=(P2&0X1F)|0X80;
}

void LED_Close(unsigned char Close)
{
	P2&=0X1F;//����
	P0|=Close;//�ߵ�ƽ������
	P2=(P2&0X1F)|0X80;
}