#include "UART.h"

void Uart_Init(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xC7;		//设定定时初值
	T2H = 0xFE;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	
	RI=0;
	ES=1;
	EA=1;
}

void Uart_SendByte(uc Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

void Uart_SendString(uc *Str)
{
	while(*Str != '\0')
	{
		Uart_SendByte(*Str++);
	}
}