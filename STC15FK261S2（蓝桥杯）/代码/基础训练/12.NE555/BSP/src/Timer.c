#include "Timer.h"

/*1个外部脉冲，1us时间进入中断*/
void Timer0Init(void)		//1微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x06;		//设置8位自动重载，外部脉冲计数模式
	TL0 = 0xFF;		//设置定时初始值
	TH0 = 0xFF;		//设置定时重载值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0用作计数
	ET0=1;
	EA=1;
	PT0=1;//当只有一个中断的时候优先级可以不配置
}

void Timer1Init(void)		//1000微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初始值
	TH1 = 0xFC;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1=1;
	EA=1;
	PT1=1;//当只有一个中断的时候优先级可以不配置
}




/*
void Timer0_Routine(void) interrupt 1
{
	
}*/
