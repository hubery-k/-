#include "RB.h"
/*蜂鸣器、继电器-P0口输出高电平经过达林顿管反转为低电平吸合、鸣响。*/

void RB_Init(void)
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