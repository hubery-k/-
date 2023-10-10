#include "RB.h"

/*P0输出高电平通过达林顿管转换为低电平继电器吸合蜂鸣器鸣响。*/
void RB_Init(void)
{
	P2&=0X1F;		//锁存
	P0=0X00;		//P0输出低电平断开
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(unsigned char Open)
{
	RB_Init();
	P0|=Open;		//P0输出高电平吸合&鸣响
	P2&=0X1F;		//锁存
}