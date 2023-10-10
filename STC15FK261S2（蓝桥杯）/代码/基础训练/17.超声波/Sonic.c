#include "Sonic.h"
#include "Delay.h"


void SendSignal()
{
	/*发送8个40HZ的方波。40HZ-25us*/
	unsigned char i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay13us();	//高电平延时13us
		TX=0;
		Delay13us();//低电平延时13us
	}
}

unsigned int Sonic()
{
	/*（遇障碍物回弹距离）=（当RX为高电平时的时间）时间*速度/2  时间通过定时器获取，速度是340m/s*/
	/*（RX=0 - 成功接收信号 RX=1 接收信号失败）（TX=0 - 发送成功 TX=1 - 发送失败）*/
	unsigned int t;
	unsigned int distance;
	SendSignal();	//发送8个方波脉冲
	TR0=1;	//定时器0开始计时
	while(RX==1 && TF0==0);//接收到信号或溢出则跳出循环（有一个不满足跳出循环）
	TR0=0;		//关闭定时器
	
	if(TF0==0)//定时器没有溢出说明RX为低电平
	{
		t=TH0<<8|TL0;
		distance=(unsigned int)t*0.017;	
		TH0=0;
		TL0=0;
	}
	else if(TF0==1)	//定时器溢出，时间为65535us，得出的距离超出超声波模块最大测量距离
	{
		TF0 = 0;  //清除溢出标志位
		distance = 999;
		TH0 = 0;
		TL0 = 0;
	}
	return distance;
}