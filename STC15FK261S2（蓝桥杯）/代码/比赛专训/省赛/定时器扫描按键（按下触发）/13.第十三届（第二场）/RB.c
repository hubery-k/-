#include "RB.h"
/*Relay、Buzzer-P0输出高电平（经过达林顿管转换为低电平使其打开）打开低电平关闭*/
void RB_Init(void)
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(uc sw)
{
	RB_Init();
	P0|=sw;
	P2&=0X1F;
}