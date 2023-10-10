/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-03-22 23:09:44
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-03-23 23:19:33
 * @FilePath: \undefinedc:\Users\zckan\Desktop\单片机\STC15FK261S2（蓝桥杯）\14届再战\比赛专训\省赛\定时器扫描按键（按下触发）、数码管\10.第十二届（第一场）\KEY.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "KEY.h"

static unsigned char KeyNum;

#define set(x) P4=(x>>3)|(x>>4);P3=x;
#define get() ((P4&0X10)<<3)|((P4&0X04)<<4)|(P3&0X3F)

unsigned char Trg,Cont;
void KEY_BTN()
{
	unsigned char Dat;
	set(0X0F);
	Dat=get();
	set(0XF0);
	Dat=(Dat|get())^0XFF;
	Trg=Dat&(Dat^Cont);
	Cont=Dat;
}

void KEY_Loop()
{
	KEY_BTN();
	switch(Trg^0XFF)
	{
		case 0X7B:KeyNum=5;break;	//S5����
		case 0X77:KeyNum=4;break;	//S4����
		case 0XBB:KeyNum=9;break;	//S9����
		case 0XB7:KeyNum=8;break;	//S8����
	}
}

unsigned char KEY()
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//�������״̬
	return Temp;
}