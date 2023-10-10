#include "Timer.h"

/*
//自己写的
void Timer0_Init(void)
{
	TMOD=(TMOD&0XF0)|0X01;//高四位保持不变，低四位赋值为0001
	TH0=(65535-64535)/256;
	TL0(65535-64535)%256;
	TF0=0;//清除中断标志
	ET0=1;
	EA=1;
	PT0=1;//当只有一个中断的时候优先级可以不配置
	TR0=1;//定时器0开始工作
}*/

//STC-SIP生成的
void Timer0Init(void)		//1000微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9C;		//设置定时初始值
	TH0 = 0xFF;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=1;//当只有一个中断的时候优先级可以不配置
}

/*
void Timer0_Routine(void) interrupt 1
{
	
}*/
