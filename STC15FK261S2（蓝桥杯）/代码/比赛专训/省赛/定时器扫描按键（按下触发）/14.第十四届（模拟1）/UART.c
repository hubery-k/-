#include "UART.h"

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T
	T2L = 0xCC;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	
	RI=0;
	//开中断
	ES=1;
	EA=1;
}


void UART_SendByte(uc Byte)
{
	SBUF=Byte;
	while(TI==0);//等待数据发送完成TI由硬件置1
	TI=0;//软件清零
}


void UART_SendString(char *Str)
{
	while(*Str != '\0')
	{
		UART_SendByte(*Str);
		Str++;
	}
}
//重定义putchar函数(为了使用printf函数)
char putchar(char ch)
{
    SBUF = ch;
    while(TI == 0);
    TI = 0;
    return ch;
}
