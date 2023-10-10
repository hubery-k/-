#include "KEY.h"

#define set(x) P4=(x>>3|x>>4);P3=x
#define get() ((P4&0X10)<<3)|((P4&0X04)<<4)|(P3&0X3F)
unsigned char Trg=0,Cont=0;

void KBD()
{
	unsigned char dat;
	set(0x0F);
	dat=get();
	set(0xF0);
	dat=(dat|get())^0xff;
	Trg=dat&(dat ^ Cont);
	Cont=dat;
}

