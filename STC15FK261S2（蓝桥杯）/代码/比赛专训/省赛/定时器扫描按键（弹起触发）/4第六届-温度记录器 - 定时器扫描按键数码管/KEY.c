#include "KEY.h"

static unsigned char KeyNum;


unsigned char Key_GetState()
{
	unsigned char Num=0;		//Num是局部变量每一次调用都会赋初值为12(由于12没有被使用)
	

	if(KEY7==0){Num=7;}
	if(KEY6==0){Num=6;}
	if(KEY5==0){Num=5;}
	if(KEY4==0){Num=4;}

	return Num;
}

void Key_Loop(void)
{
	static unsigned char N_State,L_State;
	L_State=N_State;
	N_State=Key_GetState();
	if(L_State==7&& N_State==0){KeyNum=7;}
	if(L_State==6&& N_State==0){KeyNum=6;}
	if(L_State==5&& N_State==0){KeyNum=5;}
	if(L_State==4&& N_State==0){KeyNum=4;}
}

unsigned char KEY(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//每一次调用结束后清除按键状态
	return Temp;
}
/*记住在Key函数中不能利用return来返回键值。例如，当第一次按下KeyNum=1，那么下次调用该函数时即使没有按键按下依然会返回键值KeyNum=1
	这里我使用全局变量返回键值*/