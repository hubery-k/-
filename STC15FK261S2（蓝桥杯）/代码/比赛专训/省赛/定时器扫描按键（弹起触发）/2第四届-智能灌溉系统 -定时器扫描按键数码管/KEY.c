#include "KEY.h"

static unsigned char KeyNum;
unsigned char Key_GetState()
{
	unsigned char Num=0;
	
	if(KEY7==0){Num=4;}
	if(KEY6==0){Num=3;}
	if(KEY5==0){Num=2;}
	if(KEY4==0){Num=1;}
	return Num;
}

void Key_Loop(void)
{
	static unsigned char N_State,L_State;
	L_State=N_State;
	N_State=Key_GetState();
	if(L_State==1&& N_State==0){KeyNum=1;}
	if(L_State==2&& N_State==0){KeyNum=2;}
	if(L_State==3&& N_State==0){KeyNum=3;}
	if(L_State==4&& N_State==0){KeyNum=4;}
}

unsigned char Key(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//每一次调用结束后清除按键状态
	return Temp;
}