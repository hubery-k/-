#include "RB.h"
/*蜂鸣器、继电器-P0口输出高电平继电器吸合、蜂鸣器鸣响*/
void RB_Init()
{
	P2&=0X1F;		//锁存
	P0=0X00;		//关闭蜂鸣器、继电器
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(uc Open)
{
	RB_Init();
	P0|=Open;
	P2&=0X1F;
}