#include "LED.h"
#include "RB.h"
#include "Timer.h"
#include "NIXIE.h"
#include "KEY.h"
#include "Delay.h"
#include "DS18B20.h"
#include "DA.h"
unsigned char code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
unsigned char code dot_SMG[]={~0xBF,~0x86,~0xDB,~0xCF,~0xE6,~0xED,~0xFD,~0x87,~0xFF,~0xEF,~0xF7,~0xFC,~0xB9,~0xDE,~0xF9,~0xF1};

unsigned char Interface;
float T;
unsigned char T_Int;	//温度整数数据
unsigned char T_De;		//温度小数数据
float T_temp;		//温度临时变量
unsigned char KeyNum;
signed char T_parameter=25;		//温度参数初始值25（自定义范围：0~50）
unsigned int DAC_V;
bit Mode;
bit DS18B20_Flag;
void Sys_Init()
{
	LED_Control(0X00);	//关闭LED
	RB_Control(0X00);		//关闭蜂鸣器继电器
}

void USE_DS18B20(void)
{
	if(DS18B20_Flag==1)
	{
		DS18B20_Flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		T_Int=DS18B20_ReadT();
		T_De=(unsigned char)(DS18B20_ReadT()*100)%100;		//取出后两位温度数据
	}
}

void USE_DAC(void)
{
	if(Mode==0)		//模式1
	{
		if(T<T_parameter){DAC_Write(0);DAC_V=0;}
		else{DAC_Write(255);DAC_V=500;}
		
	}
	else if(Mode==1)		//模式2（根据y=kx -> k=3V/20°C 说明升高1°电压升高0.15V
	{
		if(T<40){DAC_Write((1.0/5.0)*255);DAC_V=100;}		//输出1.0V
		if(T>40){DAC_Write((4.0/5.0)*255);DAC_V=400;}		//输出4.0V
		if(T>=20&&T<=40)		//升高1°电压上升0.15V
		{
			T_temp=T-20;
			DAC_V=100+(T_temp*0.15)*100;
			DAC_Write((DAC_V/100.0/5)*255);
		}		
	}
}

/*1ms的定时中断函数*/
void Timer0_Routine() interrupt 1
{
	static unsigned char T0Count1;
	static unsigned char T0Count2;
	static unsigned char T_Count;
	
	T0Count1++;T0Count2++;
	if(T0Count1==2)
	{
		T0Count1=0;
		Nixie_Loop();
	}
	if(T0Count2==10)
	{
		T0Count2=0;
		KEY_Loop();
	}
	T_Count++;
	if(T_Count==100)		//100ms读取一次温度数据
	{
		T_Count=0;
		DS18B20_Flag=1;
	}
	
	
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-界面按键
	{
		Interface++;
		if(Interface==3){Interface=0;}
	}
	if(Interface==1)		//参数设置界面下
	{
		if(KeyNum==8)		//S8-参数界面下减按键，温度参数减1
		{
			T_parameter-=1;
			if(T_parameter<0){T_parameter=50;}
		}
		if(KeyNum==9)		//S9-参数界面下加按键，温度参数加1
		{
			T_parameter+=1;
			if(T_parameter>50){T_parameter=0;}
		}
	}
	if(KeyNum==5)		//S5-模式切换按键
	{
		Mode=~Mode;
	}
	
}

void USE_SMG()
{
	if(Interface==0)		//温度显示界面
	{
		Nixie_SetBuf(0,Nodot_SMG[12]);		//标识-C
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,Nodot_SMG[T_Int/10]);
		Nixie_SetBuf(5,dot_SMG[T_Int%10]);
		Nixie_SetBuf(6,Nodot_SMG[T_De/10]);
		Nixie_SetBuf(7,Nodot_SMG[T_De%10]);
	}
	else if(Interface==1)		//参数设置界面
	{
		Nixie_SetBuf(0,0X8C);		//标识-P
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XFF);
		Nixie_SetBuf(6,Nodot_SMG[T_parameter/10]);
		Nixie_SetBuf(7,Nodot_SMG[T_parameter%10]);
	}
	else if(Interface==2)		//DAC输出界面
	{
		Nixie_SetBuf(0,Nodot_SMG[10]);		//标识-A
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		
		Nixie_SetBuf(5,dot_SMG[DAC_V/100]);
		Nixie_SetBuf(6,Nodot_SMG[DAC_V/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[DAC_V%10]);
	}
}

void USE_LED()
{
	if(Mode==0&&Interface==0){LED_Control(0X03);}
	else if(Mode==0&&Interface==1){LED_Control(0X05);}
	else if(Mode==0&&Interface==2){LED_Control(0X09);}
	else{LED_Control(0X00);}
}


void main()
{
	Sys_Init();
	DS18B20_ConvertT();
	Delay750ms();		//温度数据转换最高需要750ms时间
	Timer0Init();
	while(1)
	{
		EA=0;	
		USE_LED();
		EA=1;
		USE_DAC();
		USE_SMG();
		USE_DS18B20();
		USE_KEY();
	}
}
/*调用LED函数时关闭总中断，调用结束后打开是为了解决LED微微亮问题*/