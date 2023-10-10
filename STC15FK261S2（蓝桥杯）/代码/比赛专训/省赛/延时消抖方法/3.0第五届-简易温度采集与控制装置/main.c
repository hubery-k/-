#include "LED.h"
#include "RELAY.h"
#include "BUZZER.h"
#include "Nixie.h"
#include "KEY.h"
#include "Ds18B20.h"
#include "Timer.h"
#include "Delay.h"

/*使用到的硬件：LED、继电器、数码管、矩阵键盘、DS18B20温度传感器*/
unsigned char code Nodot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code dot_SMG[]={0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10};

unsigned char T_Int;
unsigned char T_Max=30;		//系统默认上限30（最大99）
unsigned char T_Min=20;		//系统默认下限20（最小0）
unsigned char T_Scope;		//温度区间（0~2）
unsigned char Interface;	//显示界面
unsigned char T_Limit[4];		//定义输入温度阈值数组
unsigned char temp;

bit L2_State;
bit L1_State;
bit RELAY_State;

void Sys_Init(void)
{
	LED_Close(0XFF);
	RELAY_Close();
	BUZZER_Close();
	Nixie_NoDisplay();
	P2=P2&0X1F;//锁存
}

void USE_DS18B20(void)
{
	/*题目中要求温度数据保存为整数数据*/
	/*且温度阈值设置范围为0~99，所以不再进行正负温度判断*/
	DS18B20_Convert();
	T_Int=DS18B20_ReadT();
	
	if(T_Int<T_Min&&T_Max>T_Min){T_Scope=0;}
	else if(T_Int<=T_Max&&T_Int>=T_Min&&T_Max>T_Min){T_Scope=1;}
	else if(T_Int>T_Max&&T_Max>T_Min){T_Scope=2;}
	else if(T_Max<T_Min){T_Scope=10;}//表示温度数据出现问题	
}

/*目前遇到的问题：把该矩阵按键函数放在KEY.c文件中，传递过来使用一次后KeyNum变量状态不能清除。按键使用点击两次后会出现问题*/
void USE_MatrixKEY(void)
{
	MatrixKey();

	if(KeyNum==10)		//设置按键按下
	{
		if(Interface==0)
		{
			Interface=1;		//进入参数设置界面
		}		
		else if(Interface==1)
		{
			if(temp!=0)
			{
				T_Max=T_Limit[0]*10+T_Limit[1];		//保存温度数据
				T_Min=T_Limit[2]*10+T_Limit[3];
				
				temp=0;
				T_Limit[0]=T_Limit[1]=T_Limit[2]=T_Limit[3]=0;		//保存完温度数据也将输入参数清除
			}		
			if(T_Max<T_Min)//输入温度数据出错，不返回温度显示界面，L2常亮
			{
				Interface=1;	
				L2_State=1;
				L1_State=0;					
			}
			else		//返回温度显示界面
			{
				Interface=0;
				L2_State=0;
			}		
		}		
		KeyNum=20;		//没有键值对应20，此处是为了清除按键按下状态
	}
	
	if(Interface==1)		//在参数设置界面下
	{
		if(KeyNum<10)		
		{
			if(temp<4)
			{
				T_Limit[temp++]=KeyNum;		//输入温度上下限
			}
			KeyNum=20;
		}
		
		if(KeyNum==11)		//清除按键按下
		{
			T_Limit[0]=T_Limit[1]=T_Limit[2]=T_Limit[3]=0;
			temp=0;
			KeyNum=20;
		}
	}
}

void USE_SMG(void)
{
	if(Interface==0)
	{
		Nixie_Display(0,0XBF);
		Nixie_Display(1,Nodot_SMG[T_Scope]);
		Nixie_Display(2,0XBF);
		
		Nixie_Display(6,Nodot_SMG[T_Int/10]);
		Nixie_Display(7,Nodot_SMG[T_Int%10]);
	}
	else if(Interface==1)
	{
		Nixie_Display(0,0XBF);
		Nixie_Display(5,0XBF);
		if(temp==1)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
		}
		else if(temp==2)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
			Nixie_Display(2,Nodot_SMG[T_Limit[1]]);
		}
		else if(temp==3)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
			Nixie_Display(2,Nodot_SMG[T_Limit[1]]);
			Nixie_Display(6,Nodot_SMG[T_Limit[2]]);
		}
		else if(temp==4)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
			Nixie_Display(2,Nodot_SMG[T_Limit[1]]);
			Nixie_Display(6,Nodot_SMG[T_Limit[2]]);
			Nixie_Display(7,Nodot_SMG[T_Limit[3]]);
		}
		else if(temp==0)
		{
			Nixie_Display(1,0XFF);
			Nixie_Display(2,0XFF);
			Nixie_Display(6,0XFF);
			Nixie_Display(7,0XFF);
		}
	}
	
	Nixie_All();		//防止SMG最后一位过于亮
}

void USE_LED(void)
{
	if(L2_State==1){LED_Open(0X02);}
	else{LED_Close(0X02);}
	
	if(L1_State==1){LED_Open(0X01);}
	else{LED_Close(0X01);}
}

void USE_RELAY(void)
{
	if(RELAY_State==1){RELAY_Open();}
	else{RELAY_Close();}
}

/*定时器0中断函数（10ms进一次中断）*/
void Timer0_Routine(void) interrupt 1
{
	static unsigned char count;
	
	if(T_Scope==0)
	{
		count++;
		if(count==80)
		{
			count=0;
			RELAY_State=0;
			L1_State=~L1_State;
		}
	}
	else if(T_Scope==1)
	{
		count++;
		if(count==40)
		{
			count=0;
			RELAY_State=0;
			L1_State=~L1_State;
		}
	}
	else if(T_Scope==2)
	{
		count++;
		if(count==20)
		{
			count=0;
			RELAY_State=1;
			L1_State=~L1_State;
		}
	}
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	DS18B20_Convert();//上电第一次先转换一次数据，防止一上电数码管不能立马显示正常的温度数据
	Delay1000ms();//等待温度转换完成再上电显示
	while(1)
	{
		USE_DS18B20();
		USE_SMG();
		USE_MatrixKEY();
		USE_LED();
		USE_RELAY();
		if(L1_State==1){LED_Open(0X01);}
		else{LED_Close(0X01);}
	}
}