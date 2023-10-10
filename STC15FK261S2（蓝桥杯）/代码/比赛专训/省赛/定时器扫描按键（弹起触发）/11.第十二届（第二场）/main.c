#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "ADC.h"

uc code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[]={~0xbf,~0x86,~0xdb,~0xcf,~0xe6,~0xed,~0xfd,~0x87,~0xff,~0xef,~0xf7,~0xfc,~0xb9,~0xde,~0xf9,~0xf1};
uc KeyNum;
f adc_value;		//采集的电压值数据
ui adc_IntValue;
uc Interface;		//界面
uc Serial_Num=1;		//编号(默认值为1)
f Save_V;		//保存的电压数据
ui Save_F;		//保存的频率数据
ui PressCount;		//S7按下时间计数
uli F;		//频率
uli F_temp;  //临时变量
uli T;		//周期

bit AD_Flag;		//AD采集标志位（1-采集，0-不采集）
bit PressTime_flag;		//S7按键长短按判断标志位（1-长按，0-断案）
bit LED_enable;		//LED使能（0-使能 1-失能）
void Sys_Init()
{
	LED_Control(0X00);		//关闭所有LED
	RB_Control(0X00);		//关闭蜂鸣器继电器
}

void USE_PCF8591()
{
	if(AD_Flag==1)		//50ms采集一次AD数据
	{
		AD_Flag=0;
		if(Serial_Num==1)
		{
			adc_value=ADC_Read('1')*(5.0/255);
		}
		else if(Serial_Num==3)
		{
			adc_value=ADC_Read('3')*(5.0/255);
		}
	}

	adc_IntValue=adc_value*100;
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-界面切换
	{
		Interface++;
		if(Interface==3){Interface=0;}
		if(Interface==2){Serial_Num=1;}		//从周期界面进入电压界面都显示通道1
	}
	if(KeyNum==5)		//S5-电压通道切换按键
	{
		if(Interface==2)		//在电压界面下
		{
			if(Serial_Num==1){Serial_Num=3;}
			else if(Serial_Num==3){Serial_Num=1;}
		}
	}
	if(KeyNum==6)		//S6-通道3电压缓存按键（任何界面下都有效）
	{
		if(Serial_Num==3)
		{
			Save_V=adc_value;		//保存通道3当前电压值
		}
	}
	if(KeyNum==7 && PressTime_flag==1)		//S7-频率缓存按键-长按键功能（任何界面下都有效）
	{
		Save_F=F;		//保存当前频率值
		LED_enable=~LED_enable;		//S7长按LED禁用与启用
		PressCount=0;		//清除按下按键时间计数值
		PressTime_flag=0;
	}
	if(KeyNum==7 && PressTime_flag==0)		//S7-频率缓存按键-短按键功能（任何界面下都有效）
	{
		Save_F=F;
		PressCount=0;		//清除按下按键时间计数值
	}
}

void USE_SMG()
{
	if(Interface==0)		//频率显示界面
	{
		Nixie_SetBuf(0,Nodot_SMG[15]);		//标识-F
		if(F>999999){Nixie_SetBuf(1,Nodot_SMG[F/1000000]);}
		else{Nixie_SetBuf(1,0XFF);}
		
		if(F>99999){Nixie_SetBuf(2,Nodot_SMG[F/100000%10]);}
		else{Nixie_SetBuf(2,0XFF);}
		
		if(F>9999){Nixie_SetBuf(3,Nodot_SMG[F/10000%10]);}
		else{Nixie_SetBuf(3,0XFF);}
		
		if(F>999){Nixie_SetBuf(4,Nodot_SMG[F/1000%10]);}
		else{Nixie_SetBuf(4,0XFF);}
		
		if(F>99){Nixie_SetBuf(5,Nodot_SMG[F/100%10]);}
		else{Nixie_SetBuf(5,0XFF);}
		
		if(F>9){Nixie_SetBuf(6,Nodot_SMG[F/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}
		
		Nixie_SetBuf(7,Nodot_SMG[F%10]);
	}
	else if(Interface==1)		//周期显示界面
	{
		Nixie_SetBuf(0,0XC8);		//标识-倒过来的U
		if(T>999999){Nixie_SetBuf(1,Nodot_SMG[T/1000000]);}
		else{Nixie_SetBuf(1,0XFF);}
		
		if(T>99999){Nixie_SetBuf(2,Nodot_SMG[T/100000%10]);}
		else{Nixie_SetBuf(2,0XFF);}
		
		if(T>9999){Nixie_SetBuf(3,Nodot_SMG[T/10000%10]);}
		else{Nixie_SetBuf(3,0XFF);}
		
		if(T>999){Nixie_SetBuf(4,Nodot_SMG[T/1000%10]);}
		else{Nixie_SetBuf(4,0XFF);}
		
		if(T>99){Nixie_SetBuf(5,Nodot_SMG[T/100%10]);}
		else{Nixie_SetBuf(5,0XFF);}
		
		if(T>9){Nixie_SetBuf(6,Nodot_SMG[T/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}
		
		Nixie_SetBuf(7,Nodot_SMG[T%10]);
	}
	else if(Interface==2)		//电压显示界面
	{
		Nixie_SetBuf(0,0XC1);		//标识-U
		Nixie_SetBuf(1,0XBF);		//标识- -
		Nixie_SetBuf(2,Nodot_SMG[Serial_Num]);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[adc_IntValue/100]);
		Nixie_SetBuf(6,Nodot_SMG[adc_IntValue/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[adc_IntValue%10]);
	}

}

void USE_LED()
{
	if(LED_enable==0)
	{
		LED_Init();
		if(Serial_Num==3 && adc_value>Save_V){L1=0;}		//通道3当前电压值超过保存的电压值
		else{L1=1;}
		if(F>Save_F){L2=0;}		//当前频率值超过保存的频率值
		else{L2=1;}
			
		if(Interface==0){L3=0;L4=1;L5=1;}		//频率界面下
		if(Interface==1){L4=0;L3=1;L5=1;}		//周期界面下
		if(Interface==2){L5=0;L4=1;L3=1;}		//电压界面下
		P2&=0X1F;		//锁存
	}
	else if(LED_enable==1)
	{
		LED_Control(0X00);		//关闭所有LED
	}
}
/*外部触发中断函数*/
void Timer0_Routine() interrupt 1
{
	F_temp++;	//外部接收一个脉冲就会溢出触发中断
}

/*1ms的定时中断函数*/
void Timer1_Routine() interrupt 3
{
	static uc T1_Count0;		//数码管扫描时间计数
	static uc T1_Count1;		//按键消抖时间计数
	static uc AD_Count;		//AD采集时间计数
	static ui F_Count;		//（NE555）频率采集时间计数
	T1_Count0++;T1_Count1++;
	if(T1_Count0==2)
	{
		T1_Count0=0;
		Nixie_Loop();
	}
	if(T1_Count1==10)
	{
		T1_Count1=0;
		KEY_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)		//50ms采集一次AD数据
	{
		AD_Count=0;
		AD_Flag=1;
	}
	
	F_Count++;
	if(F_Count==1000)		//达到1s
	{
		F=F_temp;
		T=1000000/F;		//(1000000单位为us。因为T的单位为us)
		F_temp=0;
		F_Count=0;
	}

}
/*1ms的定时中断函数*/
void Timer2_Routine() interrupt 12
{
	PressCount++;
	if(PressCount>1000){PressTime_flag=1;AUXR&=~0x10;}		//关闭T2定时器必须要对但单独位进行与操作直接AUXR|=0X00;会出现问题且影响其他定时器
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	while(1)
	{
		EA=0;		
		USE_LED();
		EA=1;
		USE_KEY();
		USE_SMG();
		USE_PCF8591();
	}
}

/*为了防止中断干扰LED的显示，所以在调用LED函数的时候关闭中断*/

/*以下程序测试S7按键按下时间*/
//		Nixie_SetBuf(0,Nodot_SMG[PressCount/1000]);
//		Nixie_SetBuf(1,Nodot_SMG[PressCount/100%10]);
//		Nixie_SetBuf(2,Nodot_SMG[PressCount/10%10]);
//		Nixie_SetBuf(3,Nodot_SMG[PressCount%10]);