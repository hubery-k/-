#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "DS18B20.h"
#include "KEY.h"
#include "Delay.h"
#include "Timer.h"

unsigned char code Nodot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code dot_SMG[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

unsigned char KeyNum;
unsigned char PWM_workMode=1;		//默认工作状态为1
unsigned char T_workMode=4;
unsigned char Work_Time;		//默认工作时间为0
unsigned char workTime_flag;
unsigned char T;
unsigned char Compare;
unsigned int Time_Count;
unsigned char PWM_Count;
bit Interface;		//显示界面（0-PWM界面，1-温度界面）
sbit Input=P3^4;
bit T_flag;

void Sys_Init(void)
{
	LED_Control(0X00);
	RB_Control(0X00);
	P2&=0X1F;		//锁存
}

void USE_DS18B20(void)
{
	if(T_flag==1)
	{
		T_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();		//转换为整形数据
	}

}

void USE_KEY(void)
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-模式切换按键
	{
		PWM_workMode++;
		if(PWM_workMode==4){PWM_workMode=1;}
	}
	if(KeyNum==5)		//S5-定时时间按键
	{
		Work_Time+=60;
		workTime_flag++;
		if(workTime_flag==3){Work_Time=0;workTime_flag=0;}
	}
	if(KeyNum==6)		//S6-停止按键
	{
		Work_Time=0;
	}
	if(KeyNum==7)		//S7-界面切换按键
	{
		Interface=~Interface;
	}
}

void USE_SMG(void)
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XBF);
		Nixie_SetBuf(1,Nodot_SMG[PWM_workMode]);
		Nixie_SetBuf(2,0XBF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,Nodot_SMG[0]);
		Nixie_SetBuf(5,Nodot_SMG[Work_Time/100]);
		Nixie_SetBuf(6,Nodot_SMG[(Work_Time/10)%10]);
		Nixie_SetBuf(7,Nodot_SMG[Work_Time%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XBF);
		Nixie_SetBuf(1,Nodot_SMG[T_workMode]);
		Nixie_SetBuf(2,0XBF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,Nodot_SMG[T/10]);
		Nixie_SetBuf(6,Nodot_SMG[T%10]);
		Nixie_SetBuf(7,0XC6);
	}
}

void USE_LED(void)
{
	if(PWM_workMode==1&&Work_Time!=0){LED_Control(0X01);}
	else if(PWM_workMode==2&&Work_Time!=0){LED_Control(0X02);}
	else if(PWM_workMode==3&&Work_Time!=0){LED_Control(0X04);}
	else{LED_Control(0X00);}
}
void USE_PWM(void)
{
	if(PWM_workMode==1){Compare=2;}
	else if(PWM_workMode==2){Compare=3;}
	else if(PWM_workMode==3){Compare=7;}
}

/*100us的定时中断函数*/
void Timer0_Routine(void) interrupt 1
{
	if(Work_Time!=0)
	{
		Time_Count++;
		if(Time_Count==10000)		//达到1s
		{
			Work_Time--;
			Time_Count=0;
		}
	}
	PWM_Count++;
	if(PWM_Count<Compare)		//周期为1ms
	{
		Input=1;
	}
	else
	{
		Input=0;
	}
	PWM_Count%=10;
}

void Timer1_Routine(void) interrupt 3
{
	static unsigned char T1_Count1;
	static unsigned char T1_Count2;
	static unsigned char T_Count;
	T1_Count1++;T1_Count2++;
	if(T1_Count1==2)
	{
		T1_Count1=0;
		Nixie_Loop();
	}
	if(T1_Count2==10)
	{
		T1_Count2=0;
		KEY_Loop();
	}
	
	T_Count++;
	if(T_Count==100)		//100ms采集温度一次
	{
		T_Count=0;
		T_flag=1;
	}
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	DS18B20_ConvertT();		//初始化时转换一次温度，防止上电读取温度数据出错
	Delay1000ms();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_DS18B20();
		USE_KEY();
		USE_SMG();
		USE_PWM();
	}
}