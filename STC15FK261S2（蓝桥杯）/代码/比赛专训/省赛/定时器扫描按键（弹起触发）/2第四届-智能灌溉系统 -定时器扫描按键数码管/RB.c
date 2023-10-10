#include "RB.h"
/*（P0口输出高电平）通过达林顿管转换为低电平低电平吸合*/

void RB_Init(void)
{
	P2&=0X1F;//锁存
	P0=0X00;//（P0口输出低电平）通过达林顿管转换为高电平高电平断开
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(unsigned char Open)
{
	RB_Init();
	P0|=Open;
	P2&=0X1F;//锁存
}
