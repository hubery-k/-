#include "BUZZER.h"

/*P0口输出高电平通过达林顿管转换为低电平蜂鸣器鸣响*/
void BUZZER_Init(void)
{
	P2&=0X1F;		//锁存
	P0&=~0X40;
	P2=(P2&0X1F)|0XA0;
}

void BUZZER_Open(void)
{
	BUZZER_Init();
	P0|=0X40;
	P2=(P2&0X1F)|0XA0;
}

void BUZZER_Close(void)
{
	BUZZER_Init();
	P0&=~0X40;
	P2=(P2&0X1F)|0XA0;
}