#include "LED.h"

/*LED-P0����͵�ƽ����*/
void LED_Init(void)
{
	P2&=0X1F;
	P0=0XFF;
	P2=(P2&0X1F)|0X80;
}

void LED_Control(uc Open)
{
	LED_Init();
	P0&=Open;
	P2&=0X1F;
}

	