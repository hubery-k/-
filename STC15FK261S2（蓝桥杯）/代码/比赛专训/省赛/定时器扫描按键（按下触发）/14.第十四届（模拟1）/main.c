#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "Sonic.h"
#include "UART.h"
#include <stdio.h>

uc code Nodot_SMG[16]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
unsigned char Buf[50];
uc KeyNum;
ui Distance;
ui Distance_P=30;
bit Interface;
bit Sonic_flag;
bit L3_flag;
bit L3_State;
bit Send_flag=1;
void Sys_Init()
{
	LED_Init();
	RB_Init();
}

void USE_Sonic()
{
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance=Sonic_Work();
	}
	if(Distance>Distance_P){L3_flag=1;}
	else{L3_flag=0;}
}

void USE_UART()
{
	if(Send_flag==1)
	{
		Send_flag=0;
		sprintf(Buf,"Distance:%dcm\r\n",Distance);//Sprintf（包含在stdio.h中）将数据存入Buf数组中（Sprintf函数第一个参数是空数组，第二个要存入数组的数据）
		UART_SendString(Buf);
	}
}
void USE_LED()
{
	LED_Init();
	if(Interface==0){L1=0;L2=1;}
	else{L1=1;L2=0;}
		
	if(L3_State==1){L3=0;}
	else{L3=1;}
		
	P2&=0X1F;
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-界面切换按键
	{
		Interface=~Interface;
	}
	if(Interface==0)
	{
		if(KeyNum==8)		//S8-参数按键
		{
			Distance_P=Distance;
		}
	}
	if(Interface==1)
	{
		if(KeyNum==12)		//S12-参数加设置按键
		{
			Distance_P+=10;
		}
		if(KeyNum==16)		//S16-参数减设置按键
		{
			Distance_P-=10;
			if(Distance_P<=0){Distance_P=0;}
		}
	}
	if(KeyNum==9)
	{
		Send_flag=1;
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
		Nixie_SetBuf(5,Nodot_SMG[Distance/100]);
		Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XC1);		//标识符-U
		Nixie_SetBuf(1,Nodot_SMG[2]);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,Nodot_SMG[Distance_P/100]);
		Nixie_SetBuf(6,Nodot_SMG[Distance_P/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[Distance_P%10]);
	}
}
void main()
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	UartInit();
	
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_UART();
		USE_Sonic();
		USE_KEY();
		USE_SMG();
	}
}


void Timer1_Routine() interrupt 3
{
	static uc T0_Count1;
	static uc T0_Count2;
	static uc Sonic_Count;
	static uc L3_Count;
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
	
	Sonic_Count++;
	if(Sonic_Count==200)
	{
		Sonic_Count=0;
		Sonic_flag=1;
	}
	
	if(L3_flag==1)
	{
		L3_Count++;
		if(L3_Count==200)
		{
			L3_Count=0;
			L3_State=~L3_State;
		}
	}
	else{L3_Count=0;L3_State=0;}
}

/*注意事项：打开串口中断一定要编写串口中断函数。否则程序会卡死。具体原因目前没有找到*/
void Uart_Routine(void) interrupt 4
{
//	if(RI==1)
//	{
//		RI=0;
//	}
}