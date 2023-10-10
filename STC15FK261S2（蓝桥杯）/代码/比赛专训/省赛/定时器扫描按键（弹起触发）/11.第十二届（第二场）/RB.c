#include "RB.h"
#include "Define.h"

/*蜂鸣器、继电器-P0口输出高电平经过达林顿管转换为高电平吸合、鸣响，低电平断开、不响。*/
void RB_Init(void)
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(uc Open)
{
	RB_Init();
	P0|=Open;		//P0输出高电平吸合、鸣响
	P2&=0X1F;
}