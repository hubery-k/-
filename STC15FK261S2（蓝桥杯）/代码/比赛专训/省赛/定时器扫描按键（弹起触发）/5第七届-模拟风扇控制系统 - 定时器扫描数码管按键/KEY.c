#include "KEY.h"

/*尝试将按键放入定时器中*/
static unsigned char KeyNum;
unsigned char Key_GetState()
{
	unsigned char Num=0;		//Num是局部变量每一次调用都会赋初值为0
	
	if(KEY7==0){Num=7;}
	if(KEY6==0){Num=6;}		//如果按下则返回键值，松开则return返回0
	if(KEY5==0){Num=5;}
	if(KEY4==0){Num=4;}
	
	return Num;
}

void KEY_Loop(void)
{
	static unsigned char N_State,L_State;
	L_State=N_State;				//按键状态更新
	N_State=Key_GetState();		//获取当前按键状态
	//如果上个时间点按键按下，这个时间点未按下，则是松手瞬间，以此避免消抖和松手检测
	if(L_State==7 && N_State==0)		//上一个状态按下，这一个状态松开
	{
		KeyNum=7;
	}
	if(L_State==6 && N_State==0)
	{
		KeyNum=6;
	}
	if(L_State==5 && N_State==0)
	{
		KeyNum=5;
	}
	if(L_State==4 && N_State==0)
	{
		KeyNum=4;
	}
}

unsigned char KEY(void)		
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//每一次调用结束后清除按键状态
	return Temp;
}