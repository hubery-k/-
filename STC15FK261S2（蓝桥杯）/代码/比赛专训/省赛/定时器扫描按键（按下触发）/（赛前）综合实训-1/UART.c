#include "UART.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xC7;		//�趨��ʱ��ֵ
	T2H = 0xFE;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	
	RI=0;		//��������жϱ�־λ
	ES=1;		//�򿪴����ж�
	EA=1;		//�����ж�
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
