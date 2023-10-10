#include "Sonic.h"

sbit TX=P1^0;
sbit RX=P1^1;

void Sonic_Init(void)		//0微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x00;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1等待开始计时
}


void Sonic_SendWave()
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
	ui Distance;
	ui t;
	
	Sonic_SendWave();
	TR1=1;
	while(TF1==0&&RX==1);
	TR1=0;
	
	if(TF1==0)
	{
		t=(TH1<<8)|TL1;
		Distance=t*0.017;
	}
	else
	{
		TF1=0;
		Distance=999;
	}
	TH1=0;
	TL1=0;
	
	return Distance;
}