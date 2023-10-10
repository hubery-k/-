#include "RELAY.h"


/*P0输出高电平通过达林顿管转换为低电平使继电器吸合*/
void RELAY_Init(void)
{
	P2&=0X1F;//锁存
	P0&=~0X10;
	P2=(P2&0X1F)|0XA0;
}
void RELAY_Open(void)
{
	RELAY_Init();
	P0|=0X10;
	P2=(P2&0X1F)|0XA0;
}

void RELAY_Close(void)
{
	P2&=0X1F;//锁存
	P0&=~0X10;
	P2=(P2&0X1F)|0XA0;
}