#include "UART.h"

void Uart_Init(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xC7;		//�趨��ʱ��ֵ
	T2H = 0xFE;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	
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