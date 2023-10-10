#include "Timer.h"


//STC-SIP生成的Timer0(12T,16位自动重装模式)
void Timer0Init(void)		//1000微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=1;//当只有一个中断的时候优先级可以不配置
}

//STC-SIP生成的Timer2(12T,16位自动重装模式)
void Timer2Init(void)		//1000微秒@12.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x18;		//设置定时初始值
	T2H = 0xFC;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	IE2|=0X04;//头文件中没有ET2的位定义
	EA=1;
	/*无中断优先级控制位，中断优先级固定为0级*/
}


/*
void Timer0_Routine(void) interrupt 1
{
	
}
*/

/*
void Timer2_Routine(void) interrupt 12
{
	
}
*/
