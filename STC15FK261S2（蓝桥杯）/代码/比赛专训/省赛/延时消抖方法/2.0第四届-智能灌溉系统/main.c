#include "LED.h"
#include "RELAY.h"
#include "BUZZER.h"
#include "Nixie.h"
#include "AD.h"
#include "Clock.h"
#include "EEPROM.h"
#include "KEY.h"
#include "Delay.h"

/*本程序使用硬件：LED、蜂鸣器、继电器、数码管、AD(PCF8591)、EEPROM(AT24C02)、按键、DS1302时钟*/


unsigned char code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

/*BCD码的好处在时间上对比16进制看起来很直观（8时30分0秒）*/
/*设置时间的数组不能加上code放入ROM中，因为读取的时间是一直更改的。放入ROM(程序存储区)中的数据将不能被更改*/
unsigned char Time[]={0X00,0X30,0X08};


float adc_vol;
unsigned char humidity;
unsigned char Hum_Limit=50;//湿度阈值初始为50%
unsigned char Red_HumLimit;
//unsigned int smg_vol;
bit Wor_State;
bit Buz_State;
bit Rel_State;
bit S6_State;
bit Buz_flag;
void Sys_Init(void)
{
	RELAY_Close();
	BUZZER_Close();
	LED_Close();
	Nixie_NoDisplay();
	P2=(P2&0x1F)|0X00;//所有外设都不选中
}

void USE_SMG(void)
{
	//初始显示状态
	if(S6_State==0)
	{
		Nixie_Display(0,Nodot_duanma[Time[2]/16]);
		Nixie_Display(1,Nodot_duanma[Time[2]%16]);
		Nixie_Display(2,0xBF);
		Nixie_Display(3,Nodot_duanma[Time[1]/16]);
		Nixie_Display(4,Nodot_duanma[Time[1]%16]);
		
		Nixie_Display(6,Nodot_duanma[humidity/10]);
		Nixie_Display(7,Nodot_duanma[humidity%10]);
		
	}
	
	
	//湿度阈值调整界面（自动工作模式下）
	if(S6_State==1)
	{
		Nixie_Display(0,0xBF);
		Nixie_Display(1,0xBF);
		Nixie_Display(6,Nodot_duanma[Hum_Limit/10]);
		Nixie_Display(7,Nodot_duanma[Hum_Limit%10]);
	}
}

void USE_AD(void)
{
	adc_vol=AD_Read()*(5.0/255);//转换为0-5V电压值
	humidity=(adc_vol/5.0)*99;//转换为湿度值
}

void USE_KEY(void)
{
	if(KeyNum==4)//S7-系统工作状态切换按键(0-自动，1-手动)
	{
		if(Wor_State==0){Wor_State=1;}
		else if(Wor_State==1){Wor_State=0;}
		KeyNum=0;//按键状态清除
	}
	
	/*S6、S5、S4手动工作状态下*/
	if(Wor_State==1)
	{
		if(KeyNum==3)//S6-关闭与打开蜂鸣器提醒功能(0-关闭，1-鸣响)
		{
			if(Buz_flag==0){Buz_flag=1;}
			else if(Buz_flag==1){Buz_flag=0;}
			KeyNum=0;//按键状态清除
		}
		else if(KeyNum==2)//S5-打开灌溉系统
		{
			Rel_State=1;
			KeyNum=0;
		}
		else if(KeyNum==1)//S4-关闭灌溉系统
		{
			Rel_State=0;
			KeyNum=0;
		}
	}
	
	/*S6、S5、S4自动工作状态下*/
	else if(Wor_State==0)
	{
		if(KeyNum==3)//S6-湿度阈值调整（第一次按下进入湿度阈值调整界面，第二次按下保存新阈值到EEPROM中）
		{
			if(S6_State==0){S6_State=1;}//(0-保存退回正常页面，1-湿度阈值调整页面)
			else if(S6_State==1){S6_State=0;}
			KeyNum=0;
		}
		else if(KeyNum==2)//S5-湿度阈值加1（在湿度阈值调整界面下有效）
		{
			if(S6_State==1)
			{
				Hum_Limit++;
			}
			KeyNum=0;
		}
		else if(KeyNum==1)//S4-湿度阈值减1（在湿度阈值调整界面下有效）
		{
			if(S6_State==1)
			{
				Hum_Limit--;
			}
			KeyNum=0;
		}
	}
}

void Work_State(void)
{
	//自动工作状态(系统上电默认自动工作状态)
	if(Wor_State==0)
	{
		LED_Open(0X01);//L1点亮
		Buz_State=0;//自动状态下关闭蜂鸣器
		if(humidity<Hum_Limit){Rel_State=1;}
		else{Rel_State=0;}
	}
	//手动工作状态
	else if(Wor_State==1)
	{
		LED_Open(0X02);//L2点亮
		S6_State=0;//手动状态下不显示阈值调整页面
		if(humidity<Hum_Limit&&Buz_flag==0){Buz_State=1;}
		else if(Buz_flag==1||humidity>=Hum_Limit){Buz_State=0;}
	}
}

void USE_EEPROM(void)
{
	if(S6_State==1)//写入数据
	{
		AT24C02_WriteByte(0X01,Hum_Limit);//写周期最长是5ms，所以每次写入都要加一个5ms的延时
		Delay5ms();
	}
	else if(S6_State==0)//读取数据
	{
		Red_HumLimit=AT24C02_Read(0X01);
		Hum_Limit=Red_HumLimit;
	}
}

void USE_BUZZER(void)
{
	if(Buz_State==1){BUZZER_Open();}
	else if(Buz_State==0){BUZZER_Close();}
}
void USE_RELAY(void)
{
	if(Rel_State==1){RELAY_Open();}
	else if(Rel_State==0){RELAY_Close();}
}


void main(void)
{
	Sys_Init();
	Set_Time(Time);
	while(1)
	{
		Read_Time(Time);
		KEY_Press();
		USE_KEY();
		USE_RELAY();
		USE_BUZZER();
		USE_AD();
		USE_SMG();
		Work_State();
		USE_EEPROM();
	}
}


	/*
	AD采集的数据通过数码管显示
	smg_vol=adc_vol*100;

	Nixie_Display(1,Nodot_duanma[][smg_vol/100]);
	Nixie_Display(2,dot_duanma[][(smg_vol/10)%10]);
	Nixie_Display(3,dot_duanma[][smg_vol%10]);
	*/