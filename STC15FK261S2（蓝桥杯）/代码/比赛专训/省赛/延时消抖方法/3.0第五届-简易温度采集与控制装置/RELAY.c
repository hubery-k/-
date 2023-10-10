#include "RELAY.h"

void RELAY_Init(void)
{
	P2&=0X1F;//锁存
	P0&=~0X10;
	P2=(P2&0X1F)|0XA0;
}
void RELAY_Open(void)
{
	RELAY_Init();
	P0|=0X10;//(P0口输出高电平)经过达林顿管转换为低电平吸合
	P2=(P2&0X1F)|0XA0;
	P2&=0X1F;//锁存
}

void RELAY_Close(void)
{
	RELAY_Init();
	P0&=~0X10;//(P0口输出低电平)经过达林顿管转换为高电平断开
	P2=(P2&0X1F)|0XA0;
	P2&=0X1F;//锁存
}