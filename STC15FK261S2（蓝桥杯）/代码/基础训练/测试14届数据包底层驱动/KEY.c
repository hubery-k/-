#include "KEY.h"

static unsigned char KeyNum;
unsigned char Trg,Cont;
void BTN()
{
	unsigned char Dat;
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

unsigned char KEY()
{
	unsigned char Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}