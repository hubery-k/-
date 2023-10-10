#include "UART.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xC7;		//设定定时初值
	TH1 = 0xFE;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	
	RI=0;
	ES=1;
	EA=1;
}


void Send_Byte(uc Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
void Send_String(char* str)
{
	while(*str != '\0')
	{
		Send_Byte(*str);
		str++;
	}
}
