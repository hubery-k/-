#include "KEY.h"
#include "Delay.h"

static unsigned char KeyNum;


unsigned char Key_GetState()
{
	unsigned char Num=0;
	
	P3|=0X3F;//为了方便判断按键按下先都给赋成1。
	P4|=0X14;
	P30=0;
	if(P44==0){Num=1;}
	if(P42==0){Num=2;}
	if(P35==0){Num=3;}

	P3|=0X3F;
	P4|=0X14;
	P31=0;
	if(P44==0){Num=4;}
	if(P42==0){Num=5;}
	if(P35==0){Num=6;}

	P3|=0X3F;
	P4|=0X14;
	P32=0;
	if(P44==0){Num=7;}
	if(P42==0){Num=8;}
	if(P35==0){Num=9;}

	P3|=0X3F;
	P4|=0X14;
	P33=0;
	if(P44==0){Num=10;}
	if(P42==0){Num=11;}//设置按键
	if(P35==0){Num=12;}//清除按键

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
	if(L_State==5&& N_State==0){KeyNum=5;}
	if(L_State==6&& N_State==0){KeyNum=6;}
	if(L_State==7&& N_State==0){KeyNum=7;}
	if(L_State==8&& N_State==0){KeyNum=8;}
	if(L_State==9&& N_State==0){KeyNum=9;}
	if(L_State==10&& N_State==0){KeyNum=10;}
	if(L_State==11&& N_State==0){KeyNum=11;}
	if(L_State==12&& N_State==0){KeyNum=12;}
}

unsigned char MatrixKey(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//每一次调用结束后清除按键状态
	return Temp;
}

/*三行的矩阵按键在后台中会有些问题所以不适用*/