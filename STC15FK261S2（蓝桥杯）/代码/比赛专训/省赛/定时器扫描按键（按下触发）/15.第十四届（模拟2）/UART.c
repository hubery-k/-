#include "UART.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xC7;		//�趨��ʱ��ֵ
	TH1 = 0xFE;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	
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
