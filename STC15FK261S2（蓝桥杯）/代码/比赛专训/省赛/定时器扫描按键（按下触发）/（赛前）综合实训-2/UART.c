/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-04-03 18:50:20
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-04-07 21:36:12
 * @FilePath: \undefinedc:\Users\zckan\Desktop\单片机\STC15FK261S2（蓝桥杯）\14届再战\比赛专训\省赛\定时器扫描按键（按下触发）、数码管\综合实训-2\UART.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "UART.h"

void UartInit(void)		//9600bps@12.000MHz
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

void Uart_SendString(uc *str)
{
	while(*str !='\0')
	{
		Uart_SendByte(*str++);
	}
}