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

void Key_Loop(void)
{
	BTN();
	if(Trg&0X01){KeyNum=7;}		//S7
	if(Trg&0X02){KeyNum=6;}		//S6
	if(Trg&0X04){KeyNum=5;}		//S5
	if(Trg&0X08){KeyNum=4;}		//S4
}

unsigned char Key(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//每一次调用结束后清除按键状态
	return Temp;
}
