#include "LED.h"

void LED_Init(void)
{
	P2&=0X1F;//锁存
	P0=0XFF;//LED P0口高电平熄灭
	P2=(P2&0x1F)|0X80;//高三位清零其余位保持不变后或上0X80
}

void LED_Control(unsigned char Light)
{
	LED_Init();
	P0&=~Light;//LED P0口低电平点亮
	P2&=0X1F;//锁存
}


