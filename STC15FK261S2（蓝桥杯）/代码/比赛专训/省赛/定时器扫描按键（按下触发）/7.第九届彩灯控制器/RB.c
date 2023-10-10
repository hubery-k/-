#include "RB.h"

/*P0口输出高电平通过达林顿管转换为低电平使蜂鸣器鸣响继电器吸合*/
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
