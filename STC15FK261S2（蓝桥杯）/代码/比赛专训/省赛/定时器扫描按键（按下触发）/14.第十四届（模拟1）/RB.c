#include "RB.h"

void RB_Init()
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(uc sw)
{
	RB_Init();
	P0|=sw;
	P2&=0X1F;
}