#include "BUZZER.h"

/*P0输出高电平通过达林顿管转换为低电平使蜂鸣器鸣响*/
void BUZZER_Init(void)
{
	P2&=0X1F;//锁存
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
	P2&=0X1F;//锁存
	P0&=~0X40;
	P2=(P2&0X1F)|0XA0;
}