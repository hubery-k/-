#include "KEY.h"

#define set(x) P4=(x>>3)|(x>>4);P3=x
#define get() ((P4&0X04)<<4)|((P4&0X10)<<3)|(P3&0X3F)

static uc KeyNum;
uc Cont,Trg;
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
	TR1=0;	//调用按键函数时先关闭定时器1，以免与串口发送冲突（定时器1用于生成串口波特率）
	switch(Trg^0XFF)
	{
		case 0X77:KeyNum=4;break;//s4
		case 0X7B:KeyNum=5;break;//s5
		case 0XD7:KeyNum=12;break;//s12
	}
	TR1=1;
	P3=0XFF;	//每次调用结束后都需要对P3重新复位
}

uc KEY()
{
	uc Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}