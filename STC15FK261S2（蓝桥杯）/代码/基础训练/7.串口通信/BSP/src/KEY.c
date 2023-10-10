#include "KEY.h"
#include "Delay.h"
#define set(x) P4=(x>>3)|(x>>4);P3=x
#define get() ((P4&0X04)<<4)|((P4&0X10)<<3)|(P3&0X3F)

static unsigned char KeyNum;
unsigned char Trg,Cont;
void KEY_KBD()
{
	unsigned char Dat;
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
	TR0=0;		//关闭生成波特率的定时器0
	/*由于串口1与按键的P30和P31功能冲突。且使用定时器0作为波特率生成器.
	所以要解决这一冲突需要在使用按键的时候关闭该定时器，使用结束后重新打开该定时器并对P3口复位*/
	switch(Trg^0XFF)
	{
		case 0x77:KeyNum=4;break;	//S4
		case 0XB7:KeyNum=8;break;	//S8
		case 0XD7:KeyNum=12;break;	//S12
		case 0XE7:KeyNum=16;break;	//S16
		case 0XBB:KeyNum=9;break;	//S9
	}
	TR0=1;	//打开生成波特率的定时器0
	P3=0XFF;		//对P3复位
}

unsigned char KEY()
{
	unsigned char Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}