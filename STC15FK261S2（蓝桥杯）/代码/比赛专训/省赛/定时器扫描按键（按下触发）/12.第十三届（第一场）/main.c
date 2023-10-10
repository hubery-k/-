#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "DS18B20.h"
#include "Delay.h"
#include "Clock.h"

uc code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[]={~0xBF,~0x86,~0xDB,~0xCF,~0xE6,~0xED,~0xFD,~0x87,~0xFF,~0xEF,~0xF7,~0xFC,~0xB9,~0xDE,~0xF9,~0xF1};
uc Time[3]={0X50,0X59,0X23};	//23时59分30秒


uc KeyNum;
f T;
uc T_Int;	//温度整数
uc T_De;	//温度小数（小数点后一位）
uc T_parameter=23;		//温度参数（范围：10~99）
uc Serial_Num=1;		//编号
uc Interface;		//显示界面
uc Count;


bit T_flag;		//温度采集标志
bit C_flag;		//时间采集标志
bit Mode;
bit L3_flag;
bit L1_State;
bit L2_State;
bit L3_State;
bit RB_flag;
bit RB_State;
bit Time_flag;

void Sys_Init()
{
	LED_Control(0X00);
	RB_Control(0X00);
}

void USE_DS18B20()
{
	if(T_flag==1)
	{
		T_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		T_Int=T;
		T_De=(uc)(T*10)%10;
	}
	
	if(Mode==0)		//温度控制模式下
	{
		L2_State=1;
		if(T>(f)T_parameter){RB_State=1;}//开继电器
		else{RB_State=0;}		//关继电器
	}
	
}

void USE_DS1302()
{
	if(C_flag==1)
	{
		C_flag=0;
		Read_Time(Time);
		if(Mode==1)		//时间模式下
		{
			L2_State=0;
			
			if(Time[0]==0X00 && Time[1]==0X00)	//达到整点
			{
				RB_State=1;		//开继电器
				L1_State=1;		//开L1
			}
		}
	}
}

void USE_LED()
{
	LED_Init();
	
	if(L1_State==1){L1=0;}
	else if(L1_State==0){L1=1;}
	
	if(L2_State==1){L2=0;}		//温度控制模式下亮
	else if(L2_State==0){L2=1;}
	
	if(L3_State==1){L3=0;}
	else if(L3_State==0){L3=1;}
	
	P2&=0X1F;
}

void USE_RB()
{
	RB_Init();
	if(RB_State==1){Relay=1;;RB_flag=1;}
	else if(RB_State==0){Relay=0;RB_flag=0;}
	
	
	if(RB_flag==1){L3_flag=1;}
	else{L3_flag=0;L3_State=0;}
	
	P2&=0X1F;
}

void USE_KEY()
{
	KeyNum=Key_Matrix();
	if(KeyNum==12)		//S12-界面切换
	{
		Interface++;
		if(Interface==3){Interface=0;}
		
		if(Interface==0){Serial_Num=1;}
		if(Interface==1){Serial_Num=2;}
		if(Interface==2){Serial_Num=3;}
	}
	
	if(KeyNum==13)		//S13-模式切换
	{
		Mode=~Mode;
	}
	
	if(Interface==2)		//参数设置界面下
	{
		if(KeyNum==16)		//S16-加（温度参数）
		{
			T_parameter+=1;
			if(T_parameter>99){T_parameter=10;}
		}
		if(KeyNum==17)		//S17-减（温度参数）
		{
			T_parameter-=1;
			if(T_parameter<10){T_parameter=99;}
		}
	}
	
//	if((Cont&0X14)&&Interface==1){Time_flag=1;}		//时间显示界面下
//	else if(Cont==0&&Interface==1){Time_flag=0;}
}

void USE_SMG()
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XC1);
		Nixie_SetBuf(1,Nodot_SMG[Serial_Num]);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,Nodot_SMG[T_Int/10]);
		Nixie_SetBuf(6,dot_SMG[T_Int%10]);
		Nixie_SetBuf(7,Nodot_SMG[T_De]);

	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XC1);
		Nixie_SetBuf(1,Nodot_SMG[Serial_Num]);
		Nixie_SetBuf(2,0XFF);
		if(Time_flag==0)
		{
			Nixie_SetBuf(3,Nodot_SMG[Time[2]/16]);
			Nixie_SetBuf(4,Nodot_SMG[Time[2]%16]);
			Nixie_SetBuf(5,0XBF);		//间隔符
			Nixie_SetBuf(6,Nodot_SMG[Time[1]/16]);
			Nixie_SetBuf(7,Nodot_SMG[Time[1]%16]);
		}
		else if(Time_flag==1)
		{
			Nixie_SetBuf(3,Nodot_SMG[Time[1]/16]);
			Nixie_SetBuf(4,Nodot_SMG[Time[1]%16]);
			Nixie_SetBuf(5,0XBF);		//间隔符
			Nixie_SetBuf(6,Nodot_SMG[Time[0]/16]);
			Nixie_SetBuf(7,Nodot_SMG[Time[0]%16]);
		}
	}
	else if(Interface==2)
	{
		Nixie_SetBuf(0,0XC1);
		Nixie_SetBuf(1,Nodot_SMG[Serial_Num]);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XFF);
		Nixie_SetBuf(6,Nodot_SMG[T_parameter/10]);
		Nixie_SetBuf(7,Nodot_SMG[T_parameter%10]);
	}
}
void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count1;
	static uc T0_Count2;
	static uc T_Count;
	static uc C_Count;
	
	T0_Count1++;T0_Count2++;
	if(T0_Count1==2)
	{
		T0_Count1=0;
		Nixie_Loop();
	}
	if(T0_Count2==20)
	{
		T0_Count2=0;
		KEY_Loop();
	}
	
	T_Count++;
	if(T_Count==100)
	{
		T_Count=0;
		T_flag=1;
	}
	
	C_Count++;
	if(C_Count==50)
	{
		C_Count=0;
		C_flag=1;
	}
}

/*1ms的定时中断函数*/
void Timer1_Routine() interrupt 3
{
	static ui T1_Count1;
	static ui T1_Count2;
	if(L1_State==1&&RB_State==1)
	{
		T1_Count1++;
		if(T1_Count1>5000)		//达到5s
		{
			T1_Count1=0;
			L1_State=0;
			RB_State=0;
		}		
	}
	
	if(L3_flag==1)
	{
		T1_Count2++;
		if(T1_Count2==100)		//达到0.1s
		{
			T1_Count2=0;
			L3_State=~L3_State;
		}
	}
}

void main()
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	Write_Time(Time);
	DS18B20_ConvertT();
	Delay750ms();
	while(1)
	{
		EA=0;
		USE_LED();
		USE_RB();
		EA=1;
		USE_DS1302();
		USE_DS18B20();
		USE_KEY();
		USE_SMG();
	}
}

/*偶尔出现的问题：在温度控制模式下继电器有时候会间断自吸合断开（不是由于LED的影响）*/