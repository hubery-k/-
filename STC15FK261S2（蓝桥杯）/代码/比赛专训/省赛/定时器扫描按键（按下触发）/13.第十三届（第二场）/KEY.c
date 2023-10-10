#include "KEY.h"

static uc KeyNum;
uc Trg,Cont;
void BTN()
{
	uc Dat;
	Dat=P3^0XFF;
	Trg=Dat&(Dat^Cont);
	Cont=Dat;
}

void KEY_Loop()
{
	BTN();
	if(Trg&0X01){KeyNum=7;}
	if(Trg&0X02){KeyNum=6;}
	if(Trg&0X04){KeyNum=5;}
	if(Trg&0X08){KeyNum=4;}
}

uc KEY()
{
	uc Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}