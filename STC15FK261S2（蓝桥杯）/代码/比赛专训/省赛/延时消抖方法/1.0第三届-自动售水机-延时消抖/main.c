#include <STC15F2K60S2.H>
#include "LED.h"
#include "RELAY.h"
#include "BUZZER.h"
#include "KEY.h"
#include "Nixie.h"
#include "AD.h"
#include "Timer.h"
#include "Delay.h"

unsigned char code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};
/*使用硬件：独立按键、数码管、AD、继电器、LED*/


float adc_vol;									//AD转换电压——变量
//unsigned int smg_vol;
float F_Price;									//价格浮点数形式——变量
unsigned char W_Price;					//价格整数形式——变量
unsigned int Water_10ML;				//出水总量——变量
unsigned char count_2;						//中断计数——变量
unsigned char count_1;						//中断计数——变量
unsigned char count_0;						//中断计数——变量
unsigned char SMG_State;				//数码管显示状态——变量
bit Over;

/*关闭所有外设*/
void Sys_Init(void)
{
	LED_Close();
	BUZZER_Close();
	RELAY_Close();
	Nixie_NoDisplay();
	P2&=0X1F;//锁存
}

/*按键应用函数*/
void USE_KEY(void)
{
	if(KeyNum==4)//S7(开水按钮)按下
	{
		RELAY_Open();		//继电器打开
		BUZZER_Close();
		SMG_State=1;		//数码管出水状态
		KeyNum=0;		//清除按键状态
		TR0=1;	//开始计时
	}
	else if(KeyNum==3||Over==1)//S6(停水按钮)按下
	{
		RELAY_Close();		//继电器关闭
		BUZZER_Close();
		
		SMG_State=2;		//数码管停水状态
		KeyNum=0;//清除按键状态
		
		count_2=0;
		Water_10ML=0;
		TR0=0;	//停止计时
		Over=0;
	}
}

/*数码管应用显示函数*/
void USE_SMG(void)
{
	/*显示费率*/
	Nixie_Display(1,dot_duanma[0]);
	Nixie_Display(2,Nodot_duanma[5]);
	Nixie_Display(3,Nodot_duanma[0]);
	
	/*显示出水总量（出水时）和总价（停水时）*/
	if(SMG_State==1)//出水时
	{
		Nixie_Display(4,Nodot_duanma[Water_10ML/1000]);
		Nixie_Display(5,dot_duanma[(Water_10ML/100)%10]);
		Nixie_Display(6,Nodot_duanma[(Water_10ML/10)%10]);
		Nixie_Display(7,Nodot_duanma[Water_10ML%10]);
	}
	else if(SMG_State==2)//停水时
	{
		Nixie_Display(4,Nodot_duanma[W_Price/1000]);
		Nixie_Display(5,dot_duanma[(W_Price/100)%10]);
		Nixie_Display(6,Nodot_duanma[(W_Price/10)%10]);
		Nixie_Display(7,Nodot_duanma[W_Price%10]);
	}
}

/*AD应用转换函数*/
void USE_AD(void)
{
	adc_vol=AD_Read()*(5.0/255);
	if(adc_vol<1.25){LED_Open(0X01);}
	else{LED_Close();}
	
	/*
	//通过数码管查看光敏电压值
	smg_vol=adc_vol*100;
	Nixie_Display(4,dot_duanma[smg_vol/100]);
	Nixie_Display(5,Nodot_duanma[smg_vol/10%10]);
	Nixie_Display(6,Nodot_duanma[smg_vol%10]);
	Nixie_Display(7,0XC1);
	*/
}

/*定时中断函数0（10ms进入一次）*/
void Timer0_Routine(void) interrupt 1
{
	count_2++;
	if(count_2>=10)
	{
		count_2=0;
		Water_10ML+=1;	//100ms-10ML
	}
		//价格转换
	F_Price=Water_10ML*0.005;
	W_Price=F_Price*100;

}


/*界定范围函数*/
void Def_Scope(void)
{
	if(Water_10ML>=9999)
	{
		Over=1;
		TR0 = 0;
	}
}

void main()
{
	Sys_Init();
	Timer0Init();

	while(1)
	{
		KEY_Press();
		USE_KEY();
		USE_SMG();
		USE_AD();
		Def_Scope();
	}
}
