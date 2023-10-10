/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-03-21 23:01:14
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-03-22 11:12:38
 * @FilePath: \undefinedc:\Users\zckan\Desktop\单片机\STC15FK261S2（蓝桥杯）\14届再战\比赛专训\省赛\定时器扫描按键（按下触发）、数码管\6.第八届-电子钟设计与调试\KEY.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "KEY.h"

static unsigned char KeyNum;
unsigned char Trg,Cont;
void BTN()
{
	unsigned char Dat;
	Dat=(P3^0XFF);
	Trg=Dat&(Dat^Cont);
	Cont=Dat;
}


void Key_Loop()
{
	BTN();
	if(Trg&0X01){KeyNum=7;}		//S7
	if(Trg&0X02){KeyNum=6;}		//S6
	if(Trg&0X04){KeyNum=5;}		//S5
	if(Trg&0X08){KeyNum=4;}		//S4
}

/*���ؼ�ֵ���������״̬*/
unsigned char Key()
{
	unsigned char Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}	