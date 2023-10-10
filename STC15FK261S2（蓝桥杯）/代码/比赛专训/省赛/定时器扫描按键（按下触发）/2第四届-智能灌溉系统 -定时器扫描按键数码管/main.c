#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "AD.h"
#include "Clock.h"
#include "EEPROM.h"
#include "KEY.h"
#include "Timer.h"
#include "Delay.h"

/*本程序使用硬件：LED、蜂鸣器、继电器、数码管、AD(PCF8591)、EEPROM(AT24C02)、按键、DS1302时钟*/
typedef unsigned int uint;
typedef unsigned char uchar;

uchar code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
uchar code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

/*BCD码的好处在时间上对比16进制看起来很直观（8时30分0秒）*/
/*设置时间的数组不能加上code放入ROM中，因为读取的时间是一直更改的。放入ROM(程序存储区)中的数据将不能被更改*/
uchar Time[]={0X00,0X30,0X08};



uchar KeyNum;
float adc_vol;
uchar humidity;
uchar Hum_Limit=50;//湿度阈值初始为50%
uchar Red_HumLimit;
uchar temp;

bit Wor_State;
bit Buz_State;
bit Rel_State;
bit S6_State;
bit Buz_flag=1;		//蜂鸣器提醒功能（默认打开）
bit L1_State;
bit L2_State;

bit AD_flag;
bit C_flag;
bit Save_flag;

void Sys_Init(void)
{
	LED_Control(0X00);		//关闭所有LED
	RB_Control(0X00);//关闭继电器蜂鸣器
	P2&=0X1F;//锁存
}

void USE_DS1302(void)
{
	if(C_flag==1)
	{
		C_flag=0;
		Read_Time(Time);
	}
}

void USE_AD(void)
{
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_vol=AD_Read()*(5.0/255);//转换为0-5V电压值
		humidity=(adc_vol/5.0)*99;//转换为湿度值	（要将这句语句放入if中否则刷新太快出现问题）
	}
}

void USE_EEPROM(void)
{
	if(Save_flag==1)
	{
		Save_flag=0;
		AT24C02_WriteByte(0X01,Hum_Limit);//写周期最长是5ms，所以每次写入都要加一个5ms的延时
		Delay5ms();
	}
}

void USE_KEY(void)
{
	KeyNum=Key();
	
	if(KeyNum==7)//S7-系统工作状态切换按键(0-自动，1-手动)
	{
		Wor_State=~Wor_State;
	}
	
	/*S6、S5、S4手动工作状态下*/
	if(Wor_State==1)
	{
		if(KeyNum==6)//S6-关闭与打开蜂鸣器提醒功能(0-关闭，1-鸣响)
		{
			Buz_flag=~Buz_flag;
		}
		else if(KeyNum==5)//S5-打开灌溉系统
		{
			Rel_State=1;
		}
		else if(KeyNum==4)//S4-关闭灌溉系统
		{
			Rel_State=0;
		}
	}
	
	/*S6、S5、S4自动工作状态下*/
	else if(Wor_State==0)
	{
		if(KeyNum==6)//S6-湿度阈值调整（第一次按下进入湿度阈值调整界面，第二次按下保存新阈值到EEPROM中）
		{
			S6_State=~S6_State;		//(0-保存退回正常页面，1-湿度阈值调整页面)
			if(S6_State==0){Save_flag=1;}
		}
		else if(KeyNum==5)//S5-湿度阈值加1（在湿度阈值调整界面下有效）
		{
			if(S6_State==1){Hum_Limit++;}
		}
		else if(KeyNum==4)//S4-湿度阈值减1（在湿度阈值调整界面下有效）
		{
			if(S6_State==1){Hum_Limit--;}
		}
	}
}

void USE_SMG(void)
{
	//初始显示状态
	if(S6_State==0)
	{
		Nixie_SetBuf(0,Nodot_duanma[Time[2]/16]);
		Nixie_SetBuf(1,Nodot_duanma[Time[2]%16]);
		Nixie_SetBuf(2,0xBF);
		Nixie_SetBuf(3,Nodot_duanma[Time[1]/16]);
		Nixie_SetBuf(4,Nodot_duanma[Time[1]%16]);
		Nixie_SetBuf(5,0xFF);
		Nixie_SetBuf(6,Nodot_duanma[humidity/10]);
		Nixie_SetBuf(7,Nodot_duanma[humidity%10]);
	}
	
	
	//湿度阈值调整界面（自动工作模式下）
	if(S6_State==1)
	{
		Nixie_SetBuf(0,0xBF);
		Nixie_SetBuf(1,0xBF);
		
		Nixie_SetBuf(2,0xFF);
		Nixie_SetBuf(3,0xFF);
		Nixie_SetBuf(4,0xFF);
		Nixie_SetBuf(5,0xFF);

		Nixie_SetBuf(6,Nodot_duanma[Hum_Limit/10]);
		Nixie_SetBuf(7,Nodot_duanma[Hum_Limit%10]);
	}
}

void Work_State(void)
{
	//自动工作状态(系统上电默认自动工作状态)
	if(Wor_State==0)
	{
		L1_State=1;		//L1点亮
		L2_State=0;		//L2熄灭
		Buz_State=0;		//自动状态下关闭蜂鸣器
		if(humidity<Hum_Limit){Rel_State=1;}
		else{Rel_State=0;}
	}
	//手动工作状态
	else if(Wor_State==1)
	{
		L2_State=1;		//L2点亮
		L1_State=0;		//L1熄灭
		S6_State=0;		//手动状态下不显示阈值调整页面
		if(humidity<Hum_Limit&&Buz_flag==1){Buz_State=1;}
		else if(humidity>=Hum_Limit||Buz_flag==0){Buz_State=0;}
	}
}


void USE_LED(void)
{
	LED_Init();
	
	if(L1_State==1){L1=0;}		//L1亮
	else{L1=1;}
	if(L2_State==1){L2=0;}		//L1灭
	else{L2=1;}
	
	P2&=0x1F;
}

void USE_RB(void)
{
	RB_Init();
	
	if(Rel_State==1){Relay=1;}
	else{Relay=0;}
	if(Buz_State==1){Buzzer=1;}
	else{Buzzer=0;}
	
	P2&=0X1F;		//锁存	
}

/*1ms的定时中断函数*/
void Timer1_Routine(void) interrupt 3
{
	static unsigned char T1_Count1;
	static unsigned char T1_Count2;
	static unsigned char AD_Count;
	static unsigned char C_Count;
	
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
	if(AD_Count==50)		//达到50ms采集AD数据一次
	{
		AD_Count=0;
		AD_flag=1;
	}
	
	C_Count++;
	if(C_Count==50)		//达到50ms采集时间数据一次
	{
		C_Count=0;
		C_flag=1;
	}
}

void main(void)
{
	Sys_Init();
	Set_Time(Time);
	temp=AT24C02_Read(0X01);
	if(temp>0&&temp<100){Hum_Limit=temp;}	//防止初次上电读取奇怪的值
	else{Hum_Limit=50;}
	Timer1Init();
	while(1)
	{
		EA=0;
		USE_RB();
		USE_LED();
		EA=1;
		USE_AD();
		USE_EEPROM();
		USE_DS1302();
		USE_KEY();
		USE_SMG();
		Work_State();
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