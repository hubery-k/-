#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "AT24C02.h"
#include "PCF8591.h"
#include "Delay.h"
uc code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[]={~0xbf,~0x86,~0xdb,~0xcf,~0xe6,~0xed,~0xfd,~0x87,~0xff,~0xef,~0xf7,~0xfc,~0xb9,~0xde,~0xf9,~0xf1};

uc KeyNum;		//按键键值
uc Interface;		//显示界面变量
f adc_v;		//采集到的电压值
ui SMG_v;		//用来在数码管上显示（电压值*100）
f V_parameter=3.0;		//电压参数
uli Num;		//计数值
ui T1_Count0;		//定时器1计数变量（用于L1）
uc faults;		//用来计无效按键次数
bit AD_flag;		//AD采集数据标志位
bit L1_flag;		//LED1标志位
bit Save_flag;		//AT24C02保存电压参数标志位
uc State;
f temp;

/*系统初始化函数*/
void Sys_Init()
{
	LED_Control(0X00);		//关闭LED
	RB_Control(0X00);		//关闭蜂鸣器、继电器
}

/*PCF8591应用函数-主要用于AD数据采集*/
void USE_PCF8591()
{
	/*AD数据采集与转换*/
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=AD_Read()*(5.0/255);
	}
	SMG_v=adc_v*100;
	
	/*L1点亮与熄灭*/
	if(adc_v<V_parameter){TR1=1;}
	else if(adc_v>=V_parameter){TR1=0;L1_flag=0;T1_Count0=0;}
	
	/*Num计数触发*/
	switch(State)
	{
		case 0:if(adc_v>V_parameter){State=1;} break;
		case 1:if(adc_v<V_parameter){State=0;Num++;} break;
	}
}

/*AT24C02应用函数-用于保存电压参数（掉电不丢失）*/
void USE_AT24C02()
{
	if(Save_flag==1)
	{
		Save_flag=0;
		EEPROM_Write(0X00,V_parameter*10);
		Delay5ms();		//写入最大需要5ms
	}
}

/*LED应用函数*/
void USE_LED()
{
	LED_Init();
	if(L1_flag==1){L1=0;}
	else{L1=1;}
	
	if(Num%2!=0){L2=0;}
	else{L2=1;}
	
	if(faults>=3){L3=0;}
	else{L3=1;}
	
	P2&=0X1F;		//锁存
}

/*按键应用函数*/
void USE_KEY()
{
	KeyNum=KEY_Matrix();
	
	/*有效操作*/
	if(KeyNum==12)		//S12-显示界面切换按键
	{
		Interface++;
		if(Interface==3){Interface=0;}
		if(Interface==2){Save_flag=1;}		//从参数界面退出，电压参数*10倍保存
		faults=0;
	}
	if(Interface==2)		//在计数界面下清零有效
	{
		if(KeyNum==13)		//S13-计数清零按键
		{
			Num=0;
			faults=0;
		}
	}
	if(Interface==1)		//在参数设置界面下S16和S17有效
	{
		if(KeyNum==16)		//S16-电压参数加按键（每次增加0.5V）
		{
			V_parameter+=0.5;
			if(V_parameter>5.0){V_parameter=0;}
			faults=0;
		}	
		if(KeyNum==17)		//S17-电压参数减按键（每次减小0.5V）
		{
			V_parameter-=0.5;
			if(V_parameter<0){V_parameter=5.0;}
			faults=0;
		}
	}
	
	/*误操作*/
	if(Interface!=1)
	{
		if(KeyNum==16||KeyNum==17){faults+=1;}
	}
	if(Interface!=2)
	{
		if(KeyNum==13){faults+=1;}
	}
}

void USE_SMG()
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XC1);		//标识符-U
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[SMG_v/100]);
		Nixie_SetBuf(6,Nodot_SMG[SMG_v/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[SMG_v%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0X8C);		//标识符-P
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[(ui)(V_parameter*100)/100]);
		Nixie_SetBuf(6,Nodot_SMG[(ui)(V_parameter*100)/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[(ui)(V_parameter*100)%10]);
	}
	else if(Interface==2)
	{
		Nixie_SetBuf(0,0X89);		//标识符-N（显示不了N只能显示H）
		if(Num>999999){Nixie_SetBuf(1,Nodot_SMG[Num/1000000]);}
		else{Nixie_SetBuf(1,0XFF);}
		
		if(Num>99999){Nixie_SetBuf(2,Nodot_SMG[Num/100000%10]);}
		else{Nixie_SetBuf(2,0XFF);}
		
		if(Num>9999){Nixie_SetBuf(3,Nodot_SMG[Num/10000%10]);}
		else{Nixie_SetBuf(3,0XFF);}
		
		if(Num>999){Nixie_SetBuf(4,Nodot_SMG[Num/1000%10]);}
		else{Nixie_SetBuf(4,0XFF);}
		
		if(Num>99){Nixie_SetBuf(5,Nodot_SMG[Num/100%10]);}
		else{Nixie_SetBuf(5,0XFF);}
		
		if(Num>9){Nixie_SetBuf(6,Nodot_SMG[Num/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}

		Nixie_SetBuf(7,Nodot_SMG[Num%10]);
	}
}


void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count0;
	static uc T0_Count1;
	static uc AD_Count;
	
	T0_Count0++;T0_Count1++;
	if(T0_Count0==2)
	{
		T0_Count0=0;
		Nixie_Loop();
	}
	if(T0_Count1==10)
	{
		T0_Count1=0;
		KEY_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)
	{
		AD_flag=1;
	}
}

void Timer1_Routine(void) interrupt 3
{
	T1_Count0++;
	if(T1_Count0>5000)		//大于5s
	{
		T1_Count0=0;
		L1_flag=1;
		TR1=0;		//停止定时
	}
}

void main()
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	temp=EEPROM_Read(0X00)/10.0;//除以10的时候必须要加上.0
	if(temp>=0&&temp<=5.0){V_parameter=temp;}
	else{V_parameter=3.0;}
	
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_AT24C02();
		USE_PCF8591();
		USE_KEY();
		USE_SMG();
	}
}
/*调用LED函数时关闭中断是为了让中断不影响LED的运行，否则LED运行状况异常会乱闪*/