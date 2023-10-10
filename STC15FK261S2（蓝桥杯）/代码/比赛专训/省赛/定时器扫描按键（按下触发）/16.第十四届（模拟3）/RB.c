#include "RB.h"
/*蜂鸣器与继电器是P0输出高电平经过达林顿管转换为低电平打开*/
void RB_Init()
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(uc Open)
{
	RB_Init();
	P0|=Open;
	P2&=0X1F;
}