#include "KEY.h"

static unsigned char KeyNum;
unsigned char Key_GetState()
{
	unsigned char Num=0;
	if(KEY7==0){Num=7;};
	if(KEY6==0){Num=6;};
	if(KEY5==0){Num=5;};
	if(KEY4==0){Num=4;};
	
	return Num;
}


void Key_Loop()
{
	static unsigned char N_State,L_State;
	L_State=N_State;
	N_State=Key_GetState();
	
	if(L_State==7&&N_State==0){KeyNum=7;}
	if(L_State==6&&N_State==0){KeyNum=6;}
	if(L_State==5&&N_State==0){KeyNum=5;}
	if(L_State==4&&N_State==0){KeyNum=4;}
}

/*返回键值并清除按键状态*/
unsigned char Key()
{
	unsigned char Temp;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}	