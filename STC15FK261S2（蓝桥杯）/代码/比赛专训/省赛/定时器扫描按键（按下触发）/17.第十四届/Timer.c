#include "Timer.h"

void Timer0Init(void)		//1微秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD |= 0x06;		//设置8位自动重载，外部脉冲计数模式
	TL0 = 0xF4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0=1;
	EA=1;
}

void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x20;		//设置定时初值
	TH1 = 0xD1;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	
	ET1=1;
	EA=1;
}



void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	AUXR &= ~0x10;		//定时器2等待开始计时
	
	IE2|=0X04;	//打开定时器2中断
	EA=1;
}
