#include "KEY.h"

#define set(x) P4=(x>>4)|(x>>3);P3=x;
#define get() ((P4&0X04)<<4)|((P4&0X10)<<3)|(P3&0X3F)

static uc KeyNum;
uc Trg,Cont;
void KBD()
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
	KBD();
	TR1=0;
	switch(Trg^0xFF)
	{
		case 0XD7:KeyNum=12;break;//S12
		case 0XE7:KeyNum=16;break;//S16
		case 0XEB:KeyNum=17;break;//S17
	}
	TR1=1;
	P3=0XFF;
}

uc KEY()
{
	uc Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}