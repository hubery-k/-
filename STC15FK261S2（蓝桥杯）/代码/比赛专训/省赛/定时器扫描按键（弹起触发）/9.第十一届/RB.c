#include "RB.h"
/*继电器、蜂鸣器-P0口输出高电平通过达林顿管转换为低电平使继电器吸合、蜂鸣器鸣响*/
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