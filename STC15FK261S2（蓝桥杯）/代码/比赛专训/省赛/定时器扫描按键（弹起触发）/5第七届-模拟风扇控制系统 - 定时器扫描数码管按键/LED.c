#include "LED.h"

void LED_Init(void)
{
	P2&=0X1F;//����
	P0=0XFF;//LED P0�ڸߵ�ƽϨ��
	P2=(P2&0x1F)|0X80;//����λ��������λ���ֲ�������0X80
}

void LED_Control(unsigned char Light)
{
	LED_Init();
	P0&=~Light;//LED P0�ڵ͵�ƽ����
	P2&=0X1F;//����
}


