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
	TR1=0;	//���ð�������ʱ�ȹرն�ʱ��1�������봮�ڷ��ͳ�ͻ����ʱ��1�������ɴ��ڲ����ʣ�
	switch(Trg^0XFF)
	{
		case 0X77:KeyNum=4;break;//s4
		case 0X7B:KeyNum=5;break;//s5
		case 0XD7:KeyNum=12;break;//s12
	}
	TR1=1;
	P3=0XFF;	//ÿ�ε��ý�������Ҫ��P3���¸�λ
}

uc KEY()
{
	uc Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}