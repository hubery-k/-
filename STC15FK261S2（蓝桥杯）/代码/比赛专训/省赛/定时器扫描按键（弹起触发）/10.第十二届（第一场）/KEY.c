#include "KEY.h"

static unsigned char KeyNum;

unsigned char KEY_GetState()
{
	unsigned char Num=0;
	
	P3|=0X3F;		//将按键使用到的P3口置1
	P4|=0X14;		//将按键使用到的P4口置1
	P32=0;
	if(P44==0){Num=5;}		//s5
	if(P42==0){Num=9;}		//s9
	
	P3|=0X3F;		//将按键使用到的P3口置1
	P4|=0X14;		//将按键使用到的P4口置1
	P33=0;
	if(P44==0){Num=4;}
	if(P42==0){Num=8;}
	
	return Num;
}

void KEY_Loop()
{
	static unsigned char N_State,L_State;		//定义静态变量记住按键状态
	L_State=N_State;
	N_State=KEY_GetState();
	
	if(L_State==4 && N_State==0){KeyNum=4;}
	if(L_State==5 && N_State==0){KeyNum=5;}
	if(L_State==8 && N_State==0){KeyNum=8;}
	if(L_State==9 && N_State==0){KeyNum=9;}
}

unsigned char KEY()
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//清除按键状态
	return Temp;
}