#include "KEY.h"
#include "Delay.h"

static unsigned char KeyNum;

/**
  * @brief  获取按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,	0表示无按键按下
  */
unsigned char Key(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}

/**
  * @brief  获取当前按键的状态，无消抖及松手检测
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,	0表示无按键按下
  */
unsigned char Key_GetState()
{
	unsigned char Num=0;
	
	if(P30==0){Num=1;}
	if(P31==0){Num=2;}
	if(P32==0){Num=3;}
	if(P33==0){Num=4;}
	
	return Num;
}

/**
  * @brief  按键驱动函数，在中断中调用
  * @param  无
  * @retval 无
  */
void Key_Loop(void)
{
	static unsigned char N_State,L_State;
	L_State=N_State;				//按键状态更新
	N_State=Key_GetState();		//获取当前按键状态
	//如果上个时间点按键按下，这个时间点未按下，则是松手瞬间，以此避免消抖和松手检测
	if(L_State==1 && N_State==0)
	{
		KeyNum=1;
	}
	if(L_State==2 && N_State==0)
	{
		KeyNum=2;
	}
	if(L_State==3 && N_State==0)
	{
		KeyNum=3;
	}
	if(L_State==4 && N_State==0)
	{
		KeyNum=4;
	}
}


