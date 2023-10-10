#include <STC15F2K60S2.H>
#include "LED.h"
#include "RB.h"
#include "KEY.h"
#include "Nixie.h"
#include "AD.h"
#include "Timer.h"
#include "Delay.h"

unsigned char code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

/*使用硬件：独立按键、数码管、AD、继电器、LED*/
unsigned char KeyNum;
float adc_vol;									//AD转换电压——变量
float F_Price;									//价格浮点数形式——变量
unsigned char W_Price;					//价格整数形式——变量
unsigned int Water_10ML;				//出水总量——变量
unsigned char count_2;						//中断计数——变量
unsigned char count_1;						//中断计数——变量
unsigned char count_0;						//中断计数——变量
unsigned char SMG_State;				//数码管显示状态——变量
bit Over;
bit RELAY_State;
bit LED_State;
bit AD_flag;




void Sys_Init(void)
{
	LED_Init();		//关闭所有LED
	RB_Init();//关闭继电器蜂鸣器
	P2&=0X1F;//锁存
}

/*按键应用函数*/
void USE_KEY(void)
{
	KeyNum=KEY();

		if(KeyNum==7)//s7
		{
			RELAY_State=1;		//继电器打开
			SMG_State=1;		//数码管出水状态
			TR0=1;	//开始计时
		}
		if(KeyNum==6||Over==1)//s6
		{
			RELAY_State=0;		//继电器关闭
			SMG_State=2;		//数码管停水状态
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
	Nixie_SetBuf(1,dot_duanma[0]);
	Nixie_SetBuf(2,Nodot_duanma[5]);
	Nixie_SetBuf(3,Nodot_duanma[0]);

	/*显示出水总量（出水时）和总价（停水时）*/
	if(SMG_State==1)//出水时
	{
		Nixie_SetBuf(4,Nodot_duanma[Water_10ML/1000]);
		Nixie_SetBuf(5,dot_duanma[(Water_10ML/100)%10]);
		Nixie_SetBuf(6,Nodot_duanma[(Water_10ML/10)%10]);
		Nixie_SetBuf(7,Nodot_duanma[Water_10ML%10]);
	}
	else if(SMG_State==2)//停水时
	{
		Nixie_SetBuf(4,Nodot_duanma[W_Price/1000]);
		Nixie_SetBuf(5,dot_duanma[(W_Price/100)%10]);
		Nixie_SetBuf(6,Nodot_duanma[(W_Price/10)%10]);
		Nixie_SetBuf(7,Nodot_duanma[W_Price%10]);
	}
}

/*AD应用转换函数*/
void USE_AD(void)
{
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_vol=AD_Read()*(5.0/255);
		if(adc_vol<1.25){LED_State=1;}
		else{LED_State=0;}
	}
}

void USE_LED(void)
{
	if(LED_State==1){LED_Control(0X01);}
	else{LED_Control(0X00);}
}

void USE_RB(void)
{
	if(RELAY_State==1){RB_Control(0X10);}
	else{RB_Control(0X00);}
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

unsigned char i;
/*定时中断函数1（1ms进入一次）*/
void Timer1_Routine(void) interrupt 3
{
	static unsigned char AD_Count;

	
	count_0++;
	count_1++;
	if(count_0==2)//2ms扫描显示数码管1次
	{
		count_0=0;
		Nixie_Loop();
	}
	if(count_1==10)//20ms扫描显示按键1次
	{
		count_1=0;
		KEY_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)		//达到50ms
	{
		AD_Count=0;
		AD_flag=1;
	}
}

/*界定范围函数*/
void Def_Scope(void)
{
	if(Water_10ML>=99)		//题目要求9999.这里为了测试写成了99
	{
		Over=1;
		TR0 = 0;
	}
}

void main()
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	while(1)
	{
		EA=0;		//防止中断影响
		USE_RB();
		USE_LED();
		EA=1;
		USE_AD();
		Def_Scope();
		USE_SMG();
		USE_KEY();
	}
}

/*出现的问题及解决方法：*/

/*
	1. 在操作LED、继电器、蜂鸣器的时候，正好碰到定时器中断，导致P0口数据不稳定，导致LED、继电器、蜂鸣器异常。
	解决方法：运行到LED和继电器蜂鸣器的时候不打开中断。
*/
/*
	2.中断会影响软件的模拟时序的数据传输。解决方法是在调用时序进行数据传输时将中断EA置0关闭，等待数据读取或写入成功再置1代开总中断。
最好的解决方法是：在中断中定义标志位，当达到一定时间后标志位置1，在主程序中做判断当标志位为1时读取数据。这样可以省去Delay的过程还不会与中断冲突。
*/