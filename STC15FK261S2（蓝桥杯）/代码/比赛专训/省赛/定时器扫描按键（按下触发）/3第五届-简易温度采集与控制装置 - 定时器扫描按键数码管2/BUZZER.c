#include "BUZZER.h"


void BUZZER_Init(void)
{
	P2&=0X1F;//锁存
	P0&=~0X40;
	P2=(P2&0X1F)|0XA0;
}
void BUZZER_Open(void)
{
	BUZZER_Init();
	P0|=0X40;//(P0口输出高电平)经过达林顿管转换为低电平鸣响
	P2=(P2&0X1F)|0XA0;
	P2&=0X1F;//锁存
}

void BUZZER_Close(void)
{
	BUZZER_Init();
	P0&=~0X40;//(P0口输出低电平)经过达林顿管转换为高电平关闭
	P2=(P2&0X1F)|0XA0;
	P2&=0X1F;//锁存
}