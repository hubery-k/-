#include "KEY.h"

static uc KeyNum;

uc KEY_GetState()
{
	uc Num=0;
	if(KEY7==0){Num=7;}
	if(KEY6==0){Num=6;}
	if(KEY5==0){Num=5;}
	if(KEY4==0){Num=4;}
	
	return Num;
}

void Key_Loop()
{
	static uc L_State,N_State;		//需要记住状态（单纯定义为局部变量调用完会消失）
	L_State=N_State;
	N_State=KEY_GetState();
	
	if(L_State==7 && N_State==0){KeyNum=7;}
	if(L_State==6 && N_State==0){KeyNum=6;}
	if(L_State==5 && N_State==0){KeyNum=5;}
	if(L_State==4 && N_State==0){KeyNum=4;}
}

uc KEY_BIN()
{
	uc Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}