#include "KEY.h"

#define set(x) P4=(x>>3)|(x>>4);P3=x
#define get() ((P4&0X04)<<4)|((P4&0X10)<<3)|(P3&0X3F)

static uc KeyNum;
uc Trg,Cont;
void KEY_KBD()
{
	uc Dat;
	set(0x0F);
	Dat=get();
	set(0xF0);
	Dat=(get()|Dat)^0XFF;
	Trg=Dat&(Dat^Cont);
	Cont=Dat;
}

void KEY_Loop()
{
	KEY_KBD();
	AUXR &= ~0x10;		//关闭定时器2;
	switch(Trg^0XFF)
	{
		case 0x77:KeyNum=4;break;	//S4
		case 0XB7:KeyNum=8;break;	//S8
		case 0XD7:KeyNum=12;break;	//S12
		case 0XE7:KeyNum=16;break;	//S16
		case 0XBB:KeyNum=9;break;	//S9
	}
	AUXR |= 0x10;		//启动定时器2;
	P3=0XFF;
}

uc KEY()
{
	uc Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}