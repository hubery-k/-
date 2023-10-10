#include "KEY.h"

/*¶ÀÁ¢°´¼ü*/
//static uc KeyNum;
//uc Trg,Cont;

//void BTN()
//{
//	uc Dat;
//	Dat=P3^0XFF;
//	Trg=Dat&(Dat^Cont);
//	Cont=Dat;
//}

//void KEY_Loop()
//{
//	BTN();
//	if(Trg&0X01){KeyNum=7;}
//	if(Trg&0X02){KeyNum=6;}
//	if(Trg&0X04){KeyNum=5;}
//	if(Trg&0X08){KeyNum=4;}
//}

//uc KEY()
//{
//	uc Temp;
//	Temp=KeyNum;
//	KeyNum=0;
//	return Temp;
//}

/*¾ØÕó°´¼ü*/


#define set(x) P4=(x>>3)|(x>>4);P3=x
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
	AUXR &= ~0x10;
	switch(Trg^0XFF)
	{
		case 0X7E:KeyNum=7;break;
		case 0X7D:KeyNum=6;break;
		case 0X7B:KeyNum=5;break;
		case 0X77:KeyNum=4;break;
		
		case 0XBE:KeyNum=11;break;
		case 0XBD:KeyNum=10;break;
		case 0XBB:KeyNum=9;break;
		case 0XB7:KeyNum=8;break;
	}
	AUXR |= 0x10;
	P3=0XFF;
}

uc KEY()
{
	uc Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}