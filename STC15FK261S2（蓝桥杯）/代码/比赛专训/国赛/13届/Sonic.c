#include "Sonic.h"

/*T1用作超声波*/
void Sonic_Init(void)		//0微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x00;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1等待开始计时
}


/*发送8个方波脉冲*/
void Sonic_SendWave()
{
	uc i;
	for(i=0; i<8; i++)
	{
		TX=1;
		Delay12us();
		TX=0;
		Delay12us();
	}
}

ui Sonic_Work()
{
	ui Distance;
	ui t;
	Sonic_SendWave();
	TR1=1;
	while(RX==1&&TF1==0);
	TR1=0;
	
	if(TF1==0)
	{
		t=(TH1<<8)|TF1;
		Distance=(ui)(t*0.017)+3;
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