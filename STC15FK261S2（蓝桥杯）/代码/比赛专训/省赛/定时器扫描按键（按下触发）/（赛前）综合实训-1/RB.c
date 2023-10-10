#include "RB.h"
/*蜂鸣器、继电器-P0口输出高电平通过达林顿管电平翻转为低电平使其打开*/
void RB_Init(void)
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