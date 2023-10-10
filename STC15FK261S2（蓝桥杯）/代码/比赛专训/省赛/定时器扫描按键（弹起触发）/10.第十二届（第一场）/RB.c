#include "RB.h"

/*蜂鸣器继电器由P0口输出高电平经过达林顿管转换为低电平导通（鸣响、吸合）高电平断开*/
void RB_Init(void)
{
	P2&=0X1F;		//锁存
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(unsigned char Open)
{
	RB_Init();
	P0=Open;
	P2&=0X1F;		//锁存
}