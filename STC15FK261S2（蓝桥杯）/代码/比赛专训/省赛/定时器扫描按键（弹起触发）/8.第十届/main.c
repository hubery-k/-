#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "KEY.h"
#include "Timer.h"
#include "PCF8591.h"

uc code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[]={~0xBF,~0x86,~0xDB,~0xCF,~0xE6,~0xED,~0xFD,~0x87,~0xFF,~0xEF,~0xF7,~0xFC,~0xB9,~0xDE,~0xF9,~0xF1};

uc KeyNum;		//按键键值
f adc_value;		//收集到的AD电压值
ui smg_adc;		//收集到的AD电压值（用于数码管显示）
uli F_temp;		//（NE555）收集频率临时变量
uli F;		//（NE555）频率变量
bit Interface;		//显示界面
bit LED_Enable;		//LED功能启用、禁用（0-启用 1-禁用）
bit SMG_Enable;		//数码管功能启用、禁用（0-启用 1-禁用）
bit AD_Flag;		//AD间隔采集标志
bit DA_Flag;		//DA模式切换标志

void Sys_Init()
{
	LED_Control(0X00);		//关闭所有LED
	RB_Control(0X00);
}

void USE_PCF8591()
{
	/*AD读取功能*/
	if(AD_Flag==1)		
	{
		AD_Flag=0;
		adc_value=AD_Read()*(5.0/255);
	}
	smg_adc=adc_value*100;
	
	
	/*DA写入功能*/
	if(DA_Flag==0)		
	{
		DA_Write(2/5.0*255);
	}
	else if(DA_Flag==1)
	{
		DA_Write(adc_value/5.0*255);
	}
}

void USE_KEY()
{
	KeyNum=KEY_BIN();
	if(KeyNum==4)		//S4-显示界面切换按键
	{
		Interface=~Interface;
	}
	if(KeyNum==5)		//S5-DAC输出模式切换
	{
		DA_Flag=~DA_Flag;
	}
	if(KeyNum==6)		//S6-LED功能启用、禁用按键
	{
		LED_Enable=~LED_Enable;
	}
	if(KeyNum==7)		//S7-数码管功能启用、禁用按键
	{
		SMG_Enable=~SMG_Enable;
	}
}

void USE_LED()
{
	if(LED_Enable==0)
	{
		LED_Init();
		if(Interface==0){L1=0;L2=1;}
		else if(Interface==1){L1=1;L2=0;}
		
		if(adc_value<1.5){L3=0;}
		else if(adc_value>=1.5 && adc_value<2.5){L3=1;}
		else if(adc_value>=2.5 && adc_value<3.5){L3=0;}
		else if(adc_value>=3.5){L3=1;}
		
		if(F<1000){L4=0;}
		else if(F>=1000 && F<5000){L4=1;}
		else if(F>=5000 && F<10000){L4=0;}
		else if(F>=10000){L4=1;}
		
		if(DA_Flag==0){L5=1;}
		else if(DA_Flag==1){L5=0;}
		
		P2&=0X1F;
	}
	else if(LED_Enable==1)
	{
		LED_Control(0X00);
	}
}

void USE_SMG()
{
	if(SMG_Enable==0)
	{
		if(Interface==0)
		{
			Nixie_SetBuf(0,0XC1);		//标识符-U
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,dot_SMG[smg_adc/100]);
			Nixie_SetBuf(6,Nodot_SMG[smg_adc/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[smg_adc%10]);
		}
		else if(Interface==1)
		{
			Nixie_SetBuf(0,Nodot_SMG[15]);		//标识符-F
			Nixie_SetBuf(1,0XFF);
			
			if(F>99999){Nixie_SetBuf(2,Nodot_SMG[F/100000]);}
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
	}
	else if(SMG_Enable==1)
	{
		Nixie_SetBuf(0,0XFF);
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XFF);
		Nixie_SetBuf(6,0XFF);
		Nixie_SetBuf(7,0XFF);
	}

}

/*外部脉冲计数模式的定时函数*/
void Timer0_Routine(void) interrupt 1
{
	F_temp++;
}

/*1ms的定时中断函数*/
void Timer1_Routine(void) interrupt 3
{
	static uc T1_Count1;		//数码管扫描显示
	static uc T1_Count2;		//按键扫描消抖
	static ui F_Count;
	static uc AD_Count;
	
	T1_Count1++;T1_Count2++;
	if(T1_Count1==2)
	{
		T1_Count1=0;
		Nixie_Loop();
	}
	if(T1_Count2==10)
	{
		T1_Count2=0;
		Key_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)
	{
		AD_Count=0;
		AD_Flag=1;
	}
	
	F_Count++;
	if(F_Count==1000)
	{
		F_Count=0;
		F=F_temp;
		F_temp=0;
	}
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_PCF8591();
		USE_KEY();
		USE_SMG();
	}
}