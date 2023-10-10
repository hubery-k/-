#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "DS18B20.h"
#include "PCF8591.h"
#include "Delay.h"
#include <stdio.h>
#include "UART.h"
uc code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[]={~0xBF,~0x86,~0xDB,~0xCF,~0xE6,~0xED,~0xFD,~0x87,~0xFF,~0xEF,~0xF7,~0xFC,~0xB9,~0xDE,~0xF9,~0xF1};

uc Send_Buf[20];
char Receive_Buf;
bit Interface;
bit T_flag;
bit AD_flag;
bit Lock;
bit Send_flag;
bit L3_flag;
bit R_flag;
bit B_flag;

uc KeyNum;
f T;
uc T_Int;
f adc_v;
ui smg_v;
void Sys_Init()
{
	LED_Init();
	RB_Init();
}
void USE_DS18B20()
{
	if(T_flag==1)
	{
		T_flag=0;
		T_Convert();
		T=T_Read();
		T_Int=(uc)(T*10.0);
	}
	
	if(T>=28){R_flag=1;}
	else{R_flag=0;}
}

void USE_PCF8591()
{
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=AD_Read()*(5.0/255);
		smg_v=adc_v*100;
	}
	
	if(adc_v>3.6){B_flag=1;}
	else{B_flag=0;}
}

void USE_UART()
{
	if(Send_flag==1)
	{
		Send_flag=0;
		if(Interface==1)
		{
			sprintf(Send_Buf,"Voltagel:%.2fV\r\n",adc_v);
			Send_String(Send_Buf);
		}
		else
		{
			sprintf(Send_Buf,"TEMP:%.1f°C\r\n",T);
			Send_String(Send_Buf);
		}
	}
	
	if(Lock==0)//解锁状态下
	{
		if(Receive_Buf=='A'){Interface=0;}
		else if(Receive_Buf=='B'){Interface=1;}
		
	}
}

void USE_LED()
{
	LED_Init();
	if(Interface==0){L1=0;L2=1;}
	else{L1=1;L2=0;}
	
	if(L3_flag==1){L3=0;}
	else{L3=1;}
	
	P2&=0X1F;
}

void USE_RB()
{
	RB_Init();
	
	if(R_flag==1){RELAY=1;}
	else{RELAY=0;}
	
	if(B_flag==1){BUZZER=1;}
	else{BUZZER=0;}
	
	P2&=0X1F;
}
void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-锁定按键
	{
		Lock=1;
	}
	else if(KeyNum==5)		//S5-解锁按键
	{
		Lock=0;
	}
	else if(KeyNum==12)		//S12-发送按键
	{
		Send_flag=1;
	}
}

void USE_SMG()
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XC1);		//标识符U
		Nixie_SetBuf(1,Nodot_SMG[1]);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,Nodot_SMG[T_Int/100]);
		Nixie_SetBuf(6,dot_SMG[T_Int/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[T_Int%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XC1);		//标识符U
		Nixie_SetBuf(1,Nodot_SMG[2]);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[smg_v/100]);
		Nixie_SetBuf(6,Nodot_SMG[smg_v/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[smg_v%10]);
	}
}


void main(void)
{
	Sys_Init();
	Timer0Init();
	UartInit();
	T_Convert();
	Delay750ms();
	while(1)
	{
		EA=0;
		USE_LED();
		USE_RB();
		EA=1;
		USE_SMG();
		USE_KEY();
		USE_DS18B20();
		USE_PCF8591();
		USE_UART();
	}
}

void Timer0_Routine() interrupt 1
{
	static uc T0_Count1;
	static uc T0_Count2;
	static uc T_Count;
	static uc AD_Count;
	static uc L3_Count;
	
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
	
	if(Lock==1)
	{
		L3_Count++;
		if(L3_Count==100)
		{
			L3_Count=0;
			L3_flag=~L3_flag;
		}
	}
	else{L3_flag=0;}
}

void UART_Routine() interrupt 4
{
	uc temp;	//上锁时用于清除SBUF中接收到的数据
	if(Lock==0)
	{
		if(RI==1)
		{
			RI=0;
			Receive_Buf=SBUF;
			Send_Byte(Receive_Buf);
		}
	}
	else
	{
		RI=0;
		temp=SBUF;
	}
}