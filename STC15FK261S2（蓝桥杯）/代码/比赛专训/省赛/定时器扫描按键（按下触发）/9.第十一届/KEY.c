/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-03-22 11:28:18
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-03-23 21:50:08
 * @FilePath: \undefinedc:\Users\zckan\Desktop\单片机\STC15FK261S2（蓝桥杯）\14届再战\比赛专训\省赛\定时器扫描按键（按下触发）、数码管\9.第十一届\KEY.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "KEY.h"

static uc KeyNum;

#define set(x) P4=(x>>3)|(x>>4);P3=x
#define get() ((P4&0X10)<<3)|((P4&0X04)<<4)|(P3&0X3F)
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
	switch(Trg ^ 0xFF)
	{
		case 0XD7:KeyNum=12;break;		//s12
		case 0XDB:KeyNum=13;break;		//s13
		case 0XE7:KeyNum=16;break;		//s16
		case 0XEB:KeyNum=17;break;		//s17
	}
}

uc KEY_Matrix()
{
	uc Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}