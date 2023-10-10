#include "RELAY.h"
#include "Delay.h"

void RELAY_Init()
{
	P2=(P2&0x1f)|0xA0;
	P0&=~0X10;
}

void RELAY_ON(void)
{
	RELAY_Init();
	P0|=0x10;//输入高电平通过达林顿管输出低电平继电器吸合
	Delay500ms();
	P0&=~0X10;//关闭
}
