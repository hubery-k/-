#include "Uart.h"

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
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

void Uart_SendString(uc *str)
{
	while(*str != '\0')
	{
		Uart_SendByte(*str++);
	}
}