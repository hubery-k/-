#include "KEY.h"

static uc KeyNum;

uc KEY_GetState()
{
	uc Num=0;
	if(KEY7==0){Num=7;AUXR|=0x10;}		//S7按键按下打开定时器2开始计时
	if(KEY6==0){Num=6;}
	if(KEY5==0){Num=5;}
	if(KEY4==0){Num=4;}
	return Num;
}

void KEY_Loop()
{
	static uc L_State,N_State;
	L_State=N_State;
	N_State=KEY_GetState();
	if(L_State==4 && N_State==0){KeyNum=4;}
	if(L_State==5 && N_State==0){KeyNum=5;}
	if(L_State==6 && N_State==0){KeyNum=6;}
	if(L_State==7 && N_State==0){KeyNum=7;AUXR&=~0x10;}		//S7按键按下关闭定时器2停止计时
}

uc KEY()
{
	uc Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}