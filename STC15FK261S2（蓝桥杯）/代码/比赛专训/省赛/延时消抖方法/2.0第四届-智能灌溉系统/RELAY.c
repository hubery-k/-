#include "RELAY.h"

void RELAY_Init(void)
{
	P2&=0X1F;//锁存
	
	P0&=~0X10;//（P0口输出低电平）通过达林顿管转换为高电平高电平断开
	P2=(P2&0X1F)|0XA0;
}

void RELAY_Open(void)
{
	RELAY_Init();
	
	P0|=0X10;//P0口输出高电平通过达林顿管转换为低电平（低电平吸合）
	P2=(P2&0X1F)|0XA0;
}

void RELAY_Close(void)
{
	P2&=0X1F;//锁存
	
	P0&=~0X10;//P0口输出低电平通过达林顿管转换为高电平（高电平断开）
	P2=(P2&0X1F)|0XA0;
	
}