#include <STC15F2K60S2.H>
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
	//不需要开启中断
}

/*发送8个方波脉冲*/
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

	/*（遇障碍物回弹距离）=（当RX为高电平时的时间）时间*速度/2  时间通过定时器获取，速度是340m/s*/
	/*（RX=0 - 成功接收信号 RX=1 接收信号失败）（TX=0 - 发送成功 TX=1 - 发送失败）*/
ui Sonic_Work()
{
	ui Distance;
	ui t;
	
	Sonic_SendWave();
	TR1=1;
	while(RX==1&&TF1==0);//接收到信号或溢出则跳出循环（有一个不满足跳出循环）
	TR1=0;
	if(TF1==0)
	{
		t=(TH1<<8)|TL1;
		Distance=t*0.017;
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

