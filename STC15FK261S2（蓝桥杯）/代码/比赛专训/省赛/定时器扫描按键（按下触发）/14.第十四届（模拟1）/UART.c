#include "UART.h"

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR &= 0xFB;		//��ʱ��2ʱ��ΪFosc/12,��12T
	T2L = 0xCC;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	
	RI=0;
	//���ж�
	ES=1;
	EA=1;
}


void UART_SendByte(uc Byte)
{
	SBUF=Byte;
	while(TI==0);//�ȴ����ݷ������TI��Ӳ����1
	TI=0;//�������
}


void UART_SendString(char *Str)
{
	while(*Str != '\0')
	{
		UART_SendByte(*Str);
		Str++;
	}
}
//�ض���putchar����(Ϊ��ʹ��printf����)
char putchar(char ch)
{
    SBUF = ch;
    while(TI == 0);
    TI = 0;
    return ch;
}
