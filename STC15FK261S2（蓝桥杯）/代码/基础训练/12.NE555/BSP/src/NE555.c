#include "NE555.h"
#include "Timer.h"
/*用到两个定时器。一个用于T0计时，一个用于T1计数*/
/*当定时器使用频率是12Mhz且为12T模式时，一个脉冲时间是1us，外部接收一个脉冲就会溢出触发中断。在中断中利用变量计脉冲的个数*/
/*T0定时中断用于计脉冲的个数，T1定时中断用于计时。为了查看达到1s产生多少个脉冲。周期T=f/1*/
/*NE555输入信号的引脚是P34引脚，且P34引脚也是T0定时器引脚*/
unsigned int F;
unsigned int count;
unsigned int temp;
void NE555_Init(void)
{
	Timer0Init();
	Timer1Init();
}

void Timer0_Routine(void) interrupt 1
{
	F++;
}

void Timer1_Routine(void) interrupt 3
{
	count++;
	if(count>=1000)		//定时1s
	{
		temp=F;
		count=0;
		F=0;
	}
}
