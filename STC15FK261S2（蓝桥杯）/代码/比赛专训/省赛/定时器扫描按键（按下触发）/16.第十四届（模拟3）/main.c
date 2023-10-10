#include "LED.h"
#include "RB.h"
#include "NIXIE.h"
#include "KEY.h"
#include "Timer.h"
#include "PCF8591.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

uc code Nodot_SMG[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0X88,0X83,0XC6,0XA1,0X86,0X8E};
uc code dot_SMG[]={0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10,0X08,0X03,0X46,0X21,0X06,0X0E};
uc Send_Buf[20];
uc Rec_Buf[10];
f adc_v;
ui smg_v;
uc KeyNum;
ui Noise_Dat;		//噪音数据
f F_Noise_Dat;
c Noise_P=5;		//噪音分贝参数（范围：0~90）

bit AD_flag;
bit Interface;
bit L8_flag;
bit L8_State;
bit Send_flag;
void Hardware_Init()
{
	LED_Init();	//关闭所有LED
	RB_Init();	//关闭蜂鸣器、继电器
	Timer0Init();
	UartInit();
}

void USE_PCF8591()
{
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=AD_Read()*(5.0/255);
		
		/*噪音数据处理：5v-90dB -> 1v-18dB*/
		if(adc_v<5.0)
		{
			F_Noise_Dat=adc_v*(90/5.0);
			Noise_Dat=(adc_v*(90/5.0))*10;
		}
		else if(adc_v>=5.0)
		{
			Noise_Dat=900;
		}
	}
	if((Noise_Dat/10)>Noise_P){L8_flag=1;}
	else{L8_flag=0;}
}

void USE_UART()
{
	if(Send_flag==1)
	{
		Send_flag=0;
		if(Interface==0)
		{
			if(strcmp(Rec_Buf,"Return")==0)
			{
				sprintf(Send_Buf,"Noise:%.1fdB\r\n",F_Noise_Dat);
				Uart_SendString(Send_Buf);
			}
		}
	}
}

void USE_LED()
{
	LED_Init();
	if(Interface==0){L1=0;L2=1;}
	else{L1=1;L2=0;}
	
	if(L8_State==1){L8=0;}
	else{L8=1;}
	
	P2&=0X1F;
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==12)
	{
		Interface=~Interface;
	}

	if(Interface==1)	//处于分贝参数显示界面
	{
		if(KeyNum==16)
		{
			Noise_P+=5;
		}
		if(KeyNum==17)
		{
			Noise_P-=5;
		}
		if(Noise_P>90){Noise_P=0;}
		if(Noise_P<0){Noise_P=90;}
	}
}
void USE_SMG()
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XC1);		//标识符-U
		Nixie_SetBuf(1,Nodot_SMG[1]);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		if(Noise_Dat>99){Nixie_SetBuf(5,Nodot_SMG[Noise_Dat/100]);}
		else{Nixie_SetBuf(5,0XFF);}
		if(Noise_Dat>9){Nixie_SetBuf(6,dot_SMG[Noise_Dat/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}
		Nixie_SetBuf(7,Nodot_SMG[Noise_Dat%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XC1);		//标识符-U
		Nixie_SetBuf(1,Nodot_SMG[2]);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XFF);
		if(Noise_Dat>9){Nixie_SetBuf(6,Nodot_SMG[Noise_P/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}
		Nixie_SetBuf(7,Nodot_SMG[Noise_P%10]);
	}
}
void main()
{
	Hardware_Init();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_PCF8591();
		USE_SMG();
		USE_KEY();
		USE_UART();
	}
}

void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count1;
	static uc T0_Count2;
	static uc AD_Count;
	static uc L8_Count;
	T0_Count1++;
	T0_Count2++;
	
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
	
	AD_Count++;
	if(AD_Count==50)
	{
		AD_Count=0;
		AD_flag=1;
	}
	
	if(L8_flag==1)
	{
		L8_Count++;
		if(L8_Count==100)
		{
			L8_Count=0;
			L8_State=~L8_State;
		}
	}
	else{L8_State=0;}
}

void Uart1_Routine(void) interrupt 4
{
	/*SBUF数据缓存器是?位的*/
	/*用串口调试助手给单片机以字符串的形式发送数据时，结尾是不默认添加'\0'的，所以需要手动添加\0作为字符串结尾。*/
	static uc i;
	uc temp;
	
	if(RI)
	{
		RI = 0;
		temp = SBUF;
		if(temp == '\r' || temp == '\n')   //结束符判断('\r'-回车符  '\n'-换行符)
		{
			Send_flag = 1;   //接收完成标志
			Rec_Buf[i] = '\0';
			i = 0;       //计数清零等待下一帧数据
		}
		else
		{
			Rec_Buf[i++] = temp;
		}
	}
}
