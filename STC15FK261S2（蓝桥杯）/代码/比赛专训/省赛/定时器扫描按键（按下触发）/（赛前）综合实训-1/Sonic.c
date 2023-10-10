#include "Sonic.h"
#include "Delay.h"

/*T1用作超声波计时*/
void Sonic_Init(void)		//0微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x00;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1等待开始计时
}

void Send_Wave()
{
	uc i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay12us();
		TX=0;
		Delay12us();
	}
}

ui Sonic_Work()
{
	ui t;
	ui Distance;
	
	Send_Wave();
	TR1=1;
	while(RX==1&&TF1==0);	//有一个条件不满足则退出循环
	TR1=0;
	if(TF1==0)
	{
		t=(TH1<<8|TL1);
		Distance=(ui)t*0.017;
		/*距离等于时间*速度（速度为340m/s -> 340*10^3(cm)/1*10^6(us)=0.034cm/us）。因为定时的时间是一个来回的时间所以再除以2*/
	}
	else if(TF1==1)
	{
		TF1=0;
		Distance=999;
	}
	TH1=0;
	TL1=0;
	return Distance;
}

