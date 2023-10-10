#include "LED.h"

/*P0输出低电平点亮，高电平熄灭*/
void LED_Init(void)
{
	P2&=0X1F;
	P0=0XFF;		//全部熄灭（低电平点亮）
	P2=(P2&0X1F)|0X80;
}

void LED_Control(unsigned char Open)
{
	LED_Init();
	P0&=~Open;
	P2&=0X1F;
}
