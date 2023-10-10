#include "KEY.h"

static unsigned char KeyNum;
unsigned char Trg,Cont;

void BTN()
{
	unsigned char dat=P3^0xFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}



void KEY_Loop()
{
	BTN();
	if(Trg&0X01){KeyNum=7;}		//S7
	if(Trg&0X02){KeyNum=6;}		//S6
}

unsigned char KEY()
{
	unsigned char Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}