#include "Timer.h"


void Timer0Init(void)		//1微秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD |= 0x06;		//设置为外部触发（计数器）模式
	TL0 = 0xF4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0=1;
	EA=1;
}

void Timer2Init(void)		//100微秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x50;		//设置定时初值
	T2H = 0xFB;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	
	IE2|=0X04;
	EA=1;
	/*T2中断优先级固定为0级*/
}

