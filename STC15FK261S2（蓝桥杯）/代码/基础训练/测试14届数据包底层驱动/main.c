#include "Nixie.h"
#include "KEY.h"
#include "DS18B20.h"
#include "Timer.h"
#include "Delay.h"
#include "PCF8591.h"
#include "CLOCK.h"
#include "AT24C02.h"

unsigned char code dot_SMG[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};
unsigned char code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
unsigned char Time[3]={0X50,0X59,0X23};		//23时59分50秒

float adc_v;
unsigned int smg_v;
float V_p=3.0;		//电压参数
float T;
unsigned char T_Int;
unsigned char T_Dec;
unsigned char KeyNum;
unsigned char Interface;		//显示界面
unsigned char Num;
bit T_flag;
bit AD_flag;
bit C_flag;
bit Col_flag;		//AD采集标志
bit Save_flag;


void Sys_Init()
{
	P2=(P2&0X1F)|0X80;P0=0XFF;//关闭LED
	P2=(P2&0X1F)|0XA0;P0=0X00;//关闭蜂鸣器、继电器
	P2&=0X1F;
}

void USE_PCF8591()
{
	if(AD_flag==1)
	{
		/*0-AD采集光敏通道 1-AD采集RB2电位器通道*/
		AD_flag=0;
		adc_v=AD_Read(Col_flag)*(5.0/255);
		smg_v=adc_v*100;
	}
}


void USE_DS18B20()
{
	if(T_flag==1)
	{
		T_flag=0;
		Convert_T();
		T=Read_T();
		T_Int=T;
		T_Dec=(unsigned char)(T*100)%100;
	}
}

void USE_DS1302()
{
	if(C_flag==1)
	{
		C_flag=0;
		Read_Time(Time);
	}
}

void USE_AT24C02()
{
	if(Save_flag==1)
	{
		Save_flag=0;
		AT24C02_Write(0X00,Num);
		Delay5ms();	//写字节最大需要5ms
	}
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-界面切换
	{
		Interface++;
		if(Interface==4){Interface=0;}
		Num+=1;
	}
	if(KeyNum==5)		//S5-切换AD收集通道
	{
		if(Interface==1){Col_flag=~Col_flag;}		//在AD显示界面才可切换AD收集通道
	}
	if(KeyNum==6)
	{
		Save_flag=1;
	}
	if(KeyNum==7)
	{
		if(Interface==3){Num=0;}
	}
}

void USE_SMG()
{
	/*显示温度数据（小数点后两位）*/
	if(Interface==0)
	{
		Nixie_SetBuf(0,Nodot_SMG[12]);		//标识符-C
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,Nodot_SMG[T_Int/10]);
		Nixie_SetBuf(5,dot_SMG[T_Int%10]);
		Nixie_SetBuf(6,Nodot_SMG[T_Dec/10]);
		Nixie_SetBuf(7,Nodot_SMG[T_Dec%10]);
	}
	/*显示通过AD采集的数据（小数点后两位）*/
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XC1);		//标识符-U
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[smg_v/100]);
		Nixie_SetBuf(6,Nodot_SMG[smg_v/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[smg_v%10]);
	}
	/*显示DS1302实时时钟时间数据*/
	else if(Interface==2)
	{
		Nixie_SetBuf(0,Nodot_SMG[Time[2]/16]);
		Nixie_SetBuf(1,Nodot_SMG[Time[2]%16]);
		Nixie_SetBuf(2,0XBF);
		Nixie_SetBuf(3,Nodot_SMG[Time[1]/16]);
		Nixie_SetBuf(4,Nodot_SMG[Time[1]%16]);
		Nixie_SetBuf(5,0XBF);
		Nixie_SetBuf(6,Nodot_SMG[Time[0]/16]);
		Nixie_SetBuf(7,Nodot_SMG[Time[0]%16]);
	}
	else if(Interface==3)
	{
		Nixie_SetBuf(0,0XC8);		//标识符-n
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,Nodot_SMG[Num/100]);
		Nixie_SetBuf(6,Nodot_SMG[Num/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[Num%10]);
	}
	
}
void Timer0_Routine(void) interrupt 1
{
	static unsigned char T0_Count1;
	static unsigned char T0_Count2;
	static unsigned char T_Count;
	static unsigned char AD_Count;
	static unsigned char C_Count;
	
	T0_Count1++;T0_Count2++;
	if(T0_Count1==2)
	{
		T0_Count1=0;
		Nixie_Loop();
	}
	if(T0_Count2==10)
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
	
	AD_Count++;
	if(AD_Count==50)
	{
		AD_Count=0;
		AD_flag=1;
	}
	
	C_Count++;
	if(C_Count==50)
	{
		C_Count=0;
		C_flag=1;
	}
}


void main()
{
	Sys_Init();
	Timer0Init();
	Set_Time(Time);
	Num=AT24C02_Read(0X00);	//首次上电可能会随机读取一个数值（无所谓，只是测试。）
	Convert_T();
	Delay750ms();
	while(1)
	{
		USE_DS18B20();
		USE_PCF8591();
		USE_DS1302();
		USE_AT24C02();
		USE_SMG();
		USE_KEY();
	}
}