#include "Timer.h"

void Timer0Init(void)		//0΢��@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x00;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0�ȴ���ʼ��ʱ
}

void Timer1Init(void)		//1����@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x18;		//���ö�ʱ��ֵ
	TH1 = 0xFC;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	
	ET1=1;
	EA=1;
}
