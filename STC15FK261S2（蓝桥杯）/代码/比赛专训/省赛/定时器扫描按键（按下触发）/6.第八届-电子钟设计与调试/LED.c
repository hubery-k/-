#include "LED.h"

/*P0����͵�ƽ�������ߵ�ƽϨ��*/
void LED_Init(void)
{
	P2&=0X1F;
	P0=0XFF;		//ȫ��Ϩ�𣨵͵�ƽ������
	P2=(P2&0X1F)|0X80;
}

void LED_Control(unsigned char Open)
{
	LED_Init();
	P0&=~Open;
	P2&=0X1F;
}
