#include "LED.h"
#include "RB.h"
#include "DS18B20.h"
#include "CLOCK.h"
#include "Timer.h"
#include "Nixie.h"
#include "Delay.h"
#include "KEY.h"
unsigned char code Donot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code not_SMG[]={0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10};
unsigned char Time[]={0X50,0X59,0X23};		//秒，分，时(BCD码表示)


unsigned char Col_T[10];		//存储采集到的10个温度数据
unsigned char Show_state;		//界面显示（0-参数设置界面  1-时钟显示界面）
unsigned char Interval=1;		//采集时间间隔（1s、5s、30s、60s）
unsigned char Col_Interval;		//确认采集时间
unsigned char i;
unsigned char index;		//采集到的温度的索引值（0-9）

unsigned char KeyNum;
bit Col_flag;		//收集温度标志（每收集一次温度成功后清除）
bit Remind_symbol;		//提示符号显示状态
bit Col_state;		//收集温度状态（1-开始收集  0-停止收集）
bit LED_flag;		//LED1状态（0-熄灭  1-点亮）
bit LED_state;		//LED2状态（0-熄灭  1-点亮）

void Iniy_Sys(void)
{
	LED_Control(0X00);
	RB_Control(0X00);
	P2&=0X1F;//锁存
}

void USE_DS18B20(void)
{
	if(Col_flag==1)		//Col_flag标志位在中断中置1，所以定时中断程序不会扰乱温度的采集
	{
		DS18B20_ConvertT();
		Col_T[i]=DS18B20_ReadT();
		i++;
		Col_flag=0;
	}
}

void USE_KEY(void)
{
	KeyNum=KEY();
	
	if(KeyNum==4)		//S4按下
	{
		KeyNum=0;
		if(Show_state==0)
		{
			if(Interval==1){Interval=5;}
			else if(Interval==5){Interval=30;}
			else if(Interval==30){Interval=60;}
			else if(Interval==60){Interval=1;}
		}
	}
	if(KeyNum==5)		//S5按下
	{
		if(Show_state==0)
		{
			Show_state=1;		//进入时钟显示界面
			Col_state=1;		//开始收集温度
		}
	}
	if(KeyNum==6)		//S6按下
	{
		if(Show_state==2)
		{
			LED_flag=0;
			index++;
			if(index>9){index=0;}
		}
	}
	if(KeyNum==7)		//S7按下
	{
		LED_flag=0;
		Show_state=0;		//进入参数设置页面
		Interval=1;		//恢复为默认采集时间间隔
	}
}

void USE_SMG(void)
{
	if(Show_state==0)		//参数设置界面
	{
		Nixie_SetBuf(0,0XFF);	
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		
		Nixie_SetBuf(5,0XBF);
		Nixie_SetBuf(6,Donot_SMG[Interval/10]);
		Nixie_SetBuf(7,Donot_SMG[Interval%10]);
	}
	else if(Show_state==1)		//时钟显示界面
	{
		Nixie_SetBuf(0,Donot_SMG[Time[2]/16]);		//除以16是因为是BCD码
		Nixie_SetBuf(1,Donot_SMG[Time[2]%16]);
		Nixie_SetBuf(3,Donot_SMG[Time[1]/16]);
		Nixie_SetBuf(4,Donot_SMG[Time[1]%16]);
		Nixie_SetBuf(6,Donot_SMG[Time[0]/16]);
		Nixie_SetBuf(7,Donot_SMG[Time[0]%16]);
		if(Remind_symbol==0)
		{
			Nixie_SetBuf(2,0XBF);
			Nixie_SetBuf(5,0XBF);
		}
		else
		{
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(5,0XFF);
		}
	}
	else if(Show_state==2)		//温度采集显示界面
	{
		Nixie_SetBuf(0,0XBF);
		Nixie_SetBuf(1,Donot_SMG[index/10]);		
		Nixie_SetBuf(2,Donot_SMG[index%10]);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XBF);
		Nixie_SetBuf(6,Donot_SMG[Col_T[index]/10]);		//显示采集的温度数据
		Nixie_SetBuf(7,Donot_SMG[Col_T[index]%10]);
	}
}

void USE_LED(void)
{
	if(LED_state==1)
	{
		LED_Control(0X01);
	}
	else{LED_Control(0X00);}
	
}

/*1ms的中断函数*/
void Timer0_Routine(void) interrupt 1
{
	static unsigned int T0_count;
	static unsigned int T0_count1;
	static unsigned char smg_count;
	static unsigned char key_count;
	static unsigned int led_cnt;
	
		/*扫描数码管和按键*/
	smg_count++;
	key_count++;
	if(smg_count==2)
	{
		smg_count=0;
		Nixie_Loop();
	}
	if(key_count==10)
	{
		key_count=0;
		Key_Loop();
	}
	
	if(Show_state==1)
	{
		T0_count++;
		if(T0_count==1000)		//达到1s
		{
			Remind_symbol=~Remind_symbol;
			T0_count=0;
		}
	}

	if(Col_flag==0)		//防止一次温度数据没有采集完开始了第二次采集
	{
		if(Col_state==1)
		{
			T0_count1++;
			if(T0_count1==(Interval*1000))		//达到收集1次温度的时间
			{
				if(Col_Interval==Interval*10)	//收集10次完成后
				{
					Col_state=0;		//停止收集
					Col_Interval=0;		//收集间隔时间清零
					i=0;		//自增变量清除
					Show_state=2;		//收集完温度进入温度采集显示界面
					LED_flag=1;
				}
				T0_count1=0;
				Col_flag=1;//收集温度标志
				Col_Interval+=Interval;
			}
		}
	}
	
	if(LED_flag==1)
	{
		led_cnt++;
		if(led_cnt<500)
		{LED_state=1;}
		else {LED_state=0;}
		if(led_cnt==1000)led_cnt=0;
	}else {LED_state=0;}
}

void main(void)
{
	Iniy_Sys();
	Timer0Init();
	Write_time(Time);
	DS18B20_ConvertT();//上电第一次先转换一次数据，防止一上电数码管不能立马显示正常的温度数据
	Delay1000ms();//等待温度转换完成再上电显示
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		Read_time(Time);
		USE_KEY();
		USE_DS18B20();
		USE_SMG();
	}
}
/*调用LED函数将中断关闭，中断会影响LED的显示*/
/*
	本题中温度采集已经与中断程序形成前后台程序且采集一次温度的时间大于100ms（最低为0.1s），所以中断程序不会影响onewire时序的数据传输。
	本题中中断并没有恰好没有影响时间的读取。当读取时间出现问题时只能在中断中置标志位达到50ms读取一次时间值。
*/