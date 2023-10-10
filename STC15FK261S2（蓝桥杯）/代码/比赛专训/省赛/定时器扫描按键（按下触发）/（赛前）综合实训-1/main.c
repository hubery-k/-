#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "PCF8591.h"
#include "DS18B20.h"
#include "CLOCK.h"
#include "AT24C02.h"
#include "Delay.h"
#include <stdio.h>
#include "UART.h"
#include "Sonic.h"

/*SMG段码:0-F*/
uc code Nodot_SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};	
uc code dot_SMG[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

/*BCD码转十进制：DEC=BCD/16*10+BCD%16*/
/*十进制转BCD码：BCD=DEC/10*16+DEC%10 （得出的结果是该BCD码以十进制形式表示。例如：20/10*16+20%10=32 二进制就是0010 0000 BCD码就是0X20）*/
uc Time[3]={0X50,0X59,0X23};	//23时59分50秒
uc Send_Buf[30];

uc KeyNum;
uc Interface;
f adc_v;
f T;
uc T_p=23;		//温度参数
uc Num;
uc Condition;
ui Distance;

bit Send_flag;
bit Channel;
bit AD_flag;
bit T_flag;
bit C_flag;
bit Write_flag;
bit Read_flag;
bit Sonic_flag;
bit LED_Enable=1;
bit SMG_Enable=1;
bit L7_flag;
bit L7_State;

/*硬件初始化函数*/
void Hardware_Init()
{
	LED_Init();		//关闭所有LED
	RB_Init();		//关闭蜂鸣器、继电器
	Timer0Init();	//定时器0初始化
	UartInit();		//串口（使用定时器2）初始化
	Sonic_Init();	//超声波（使用定时器1）初始化
	Write_Time(Time);	//DS1302写入时间
	DS18B20_ConvertT();	//开始温度转换
	Delay750ms();		//等待第一次程序运行前温度转换完成（最大750ms）
}

/*PCF8591使用函数*/
void USE_PCF8591()
{
	/*50ms读取一次电压数据*/
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=Read_AD(Channel)*(5.0/255);
	}
}

/*DS18B20使用函数*/
void USE_DS18B20()
{
	/*100ms读取一次温度数据*/
	if(T_flag==1)
	{
		T_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
	}
	/*温度数值先大于参数值后小于参数值则计数值加1*/
	switch(Condition)
	{
		case 0:if(T>T_p){Condition=1;}break;
		case 1:if(T<T_p){Condition=0;Num+=1;}break;
	}
	
	if(T>T_p){L7_flag=1;}
	else{L7_flag=0;}
}

/*DS1302使用函数*/
void USE_DS1302()
{
	/*50ms读取一次时间*/
	if(C_flag==1)
	{
		C_flag=0;
		Read_Time(Time);
	}
}

/*AT24C02使用函数*/
void USE_AT24C02()
{
	if(Write_flag==1)
	{
		Write_flag=0;
		AT24C02_Write(0X00,Num);	//写字节最大需要5ms时间
		Delay5ms();
	}
	if(Read_flag==1)
	{
		Read_flag=0;
		Num=AT24C02_Read(0X00);
	}
}

/*超声波使用函数*/
void USE_Sonic()
{
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance=Sonic_Work();
	}
}

/*串口使用函数*/
void USE_UART()
{
	if(Send_flag==1)
	{
		Send_flag=0;
		if(Interface==0)		//发送电压数据
		{
			sprintf(Send_Buf,"V:%.2f V\r\n",adc_v);
			Send_String(Send_Buf);
		}
		else if(Interface==1)		//发送温度数据
		{
			sprintf(Send_Buf,"T:%.2f ℃\r\n",T);
			Send_String(Send_Buf);
		}
		else if(Interface==2)		//发送当前时间数据
		{
			sprintf(Send_Buf,"%x h  ",Time[2]);
			Send_String(Send_Buf);
			sprintf(Send_Buf,"%x m  ",Time[1]);
			Send_String(Send_Buf);
			sprintf(Send_Buf,"%x s\r\n",Time[0]);
			Send_String(Send_Buf);
		}
		else if(Interface==3)		//发送超声波测距数据
		{
			sprintf(Send_Buf,"C:%d cm\r\n",Distance);
			Send_String(Send_Buf);
		}
	}
}

/*LED使用函数*/
void USE_LED()
{
	LED_Init();
	if(LED_Enable==1)
	{
		if(Interface==0){L1=0;L2=1;L3=1;L4=1;L5=1;}
		else if(Interface==1){L1=1;L2=0;L3=1;L4=1;L5=1;}
		else if(Interface==2){L1=1;L2=1;L3=0;L4=1;L5=1;}
		else if(Interface==3){L1=1;L2=1;L3=1;L4=0;L5=1;}
		else if(Interface==4){L1=1;L2=1;L3=1;L4=1;L5=0;}
		
		if(Channel==1){L6=0;}
		else{L6=1;}
		if(L7_State==1){L7=0;}
		else{L7=1;}
	}
	else{LED_Control(0XFF);}
	P2&=0X1F;
}

/*按键使用函数*/
void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)	//S4-界面切换
	{
		Interface++;
		if(Interface==5){Interface=0;}
	}
	if(KeyNum==5){if(Interface==0){Channel=~Channel;}}	//S8-AD采集通道切换
	if(KeyNum==6){LED_Enable=~LED_Enable;}
	if(KeyNum==7){SMG_Enable=~SMG_Enable;}
	
	if(KeyNum==8){Num=0;}		//Num清零
	if(KeyNum==9){Write_flag=1;}		//写入EEPROM
	if(KeyNum==10){Read_flag=1;}		//从EEPROM中读取
	if(KeyNum==11){Send_flag=1;}		//往上位机发送数据
}

/*数码管使用函数*/
void USE_SMG()
{
	if(SMG_Enable==1)
	{
		if(Interface==0)
		{
			Nixie_SetBuf(0,0XC1);	//标识符-U
			Nixie_SetBuf(1,Nodot_SMG[Channel]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,dot_SMG[((ui)(adc_v*100))/100]);
			Nixie_SetBuf(6,Nodot_SMG[((ui)(adc_v*100))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(adc_v*100))%10]);
		}
		else if(Interface==1)
		{
			Nixie_SetBuf(0,0XC8);	//标识符-n
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,Nodot_SMG[(uc)T/10]);
			Nixie_SetBuf(6,dot_SMG[(uc)T%10]);
			Nixie_SetBuf(7,Nodot_SMG[(uc)(T*10)%10]);
		}
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
			Nixie_SetBuf(0,Nodot_SMG[12]);	//标识符-C
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,Nodot_SMG[Distance/100]);
			Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
		else if(Interface==4)
		{
			/*SMG0-3显示Num数值*/
			Nixie_SetBuf(0,0XC8);
			Nixie_SetBuf(1,0XBF);
			if(Num>9){Nixie_SetBuf(2,Nodot_SMG[Num/10]);}
			else{Nixie_SetBuf(2,0XFF);}
			Nixie_SetBuf(3,Nodot_SMG[Num%10]);
			/*SMG4-7显示温度参数*/
			Nixie_SetBuf(4,0X8C);
			Nixie_SetBuf(5,0XBF);
			if(T_p>9){Nixie_SetBuf(6,Nodot_SMG[T_p/10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[T_p%10]);
		}
	}
	else
	{
		Nixie_SetBuf(0,0XFF);Nixie_SetBuf(1,0XFF);Nixie_SetBuf(2,0XFF);Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);Nixie_SetBuf(5,0XFF);Nixie_SetBuf(6,0XFF);Nixie_SetBuf(7,0XFF);
	}

}

void main(void)
{
	Hardware_Init();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_PCF8591();
		USE_DS18B20();
		USE_DS1302();
		USE_AT24C02();
		USE_KEY();
		USE_SMG();
		USE_Sonic();
		USE_UART();
	}
}

void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count1;
	static uc T0_Count2;
	static uc AD_Count;
	static uc T_Count;
	static uc C_Count;
	static uc Sonic_Count;
	static uc L7_Count;
	T0_Count1++;
	if(T0_Count1==2)
	{
		T0_Count1=0;
		Nixie_Loop();
	}
	T0_Count2++;
	if(T0_Count2==10)
	{
		T0_Count2=0;
		KEY_Loop();
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
	T_Count++;
	if(T_Count==100)
	{
		T_Count=0;
		T_flag=1;
	}
	Sonic_Count++;
	if(Sonic_Count==200)
	{
		Sonic_Count=0;
		Sonic_flag=1;
	}
	
	if(L7_flag==1)
	{
		L7_Count++;
		if(L7_Count==100)
		{
			L7_Count=0;
			L7_State=~L7_State;
		}
	}
	else{L7_Count=0;L7_State=0;}
}

/*注意事项：打开串口中断一定要编写串口中断函数。否则程序会卡死。具体原因目前没有找到*/
void Uart_Routine(void) interrupt 4
{
//	if(RI==1)
//	{
//		RI=0;
//	}
}
