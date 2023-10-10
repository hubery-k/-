#include "KEY.h"

static uc KeyNum;
#define set(x) P4=(x>>3)|(x>>4);P3=x
#define get() ((P4&0X10)<<3)|((P4&0x04)<<4)|(P3&0X3F)

uc Trg,Cont;
void KEY_KBD()
{
	uc Dat;
	set(0X0F);
	Dat=get();
	set(0XF0);
	Dat=(Dat|get())^0XFF;
	Trg=Dat&(Dat^Cont);
	Cont=Dat;
}

void KEY_Loop()
{
	KEY_KBD();
	switch(Trg^0XFF)
	{
		case 0XD7:KeyNum=12;break;
		case 0XDB:KeyNum=13;break;
		case 0XE7:KeyNum=16;break;
		case 0XEB:KeyNum=17;break;
	}
}

uc Key_Matrix()
{
	uc Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}