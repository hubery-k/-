#include "UART.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xC7;		//设定定时初值
	T2H = 0xFE;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	
	RI=0;		//清除接收中断标志位
	ES=1;		//打开串口中断
	EA=1;		//打开总中断
}

void Send_Byte(uc Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

void Send_String(uc *str)
{
	while(*str != '\0')
	{
		Send_Byte(*str++);
	}
}
