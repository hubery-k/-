#include "Sonic.h"

sbit TX=P1^0;
sbit RX=P1^1;

void Soinc_Init()
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x00;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0等待开始计时
	//不需要开启中断
}

void Sonic_SendWave()
{
	uc i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay13us();
		TX=0;
		Delay13us();
	}
}

ui Sonic_Work()
{
	ui Distance;
	ui t;
	
	Sonic_SendWave();
	TR0=1;
	while(RX==1&&TF0==0);//接收到信号或溢出则跳出循环（有一个不满足跳出循环）
	TR0=0;
	if(TF0==0)
	{
		t=(TH0<<8)|TL0;
		Distance=t*0.017;
	}
	else if(TF0==1)
	{
		TF0=0;
		Distance=999;
	}
	TH0=0;
	TL0=0;
	return Distance;
}