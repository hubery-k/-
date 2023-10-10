#include "BUZZER.h"
#include "UART.h"
#include "Timer.h"
#include "KEY.h"
#include "Nixie.h"
#include "Delay.h"


unsigned char code Nodot_SMG[16]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
unsigned char Send_Array[]="123\r\n";
unsigned int Distance=500;
unsigned char KeyNum;
bit Interface;
bit flag;

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-界面切换按键
	{
		Interface=~Interface;
		flag=1;
		
	}
}

void USE_SMG()
{
	if(Interface==1)
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
	else
	{
		Nixie_SetBuf(0,0XFF);		//标识符-U
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XFF);
		Nixie_SetBuf(6,0XFF);
		Nixie_SetBuf(7,0XFF);		
	}
}

void main(void)
{
	BUZZER_Init();
	UartInit();
	Timer0Init();
	while(1)
	{
		USE_SMG();
		USE_KEY();
		if(flag==1)
		{
			flag=0;
			UART_SendString(Send_Array);
		}
	}
}

void Timer0_Routine() interrupt 1
{
	static unsigned char Count1;
	static unsigned char Count2;
	Count1++;
	Count2++;
	if(Count1==2)
	{
		Count1=0;
		Nixie_Loop();
	}
	if(Count2==10)
	{
		Count2=0;
		KEY_Loop();
	}
}
