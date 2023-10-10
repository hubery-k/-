#include "Timer.h"


//STC-SIP���ɵ�Timer0(12T,16λ�Զ���װģʽ)
void Timer0Init(void)		//1000΢��@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
	PT0=1;//��ֻ��һ���жϵ�ʱ�����ȼ����Բ�����
}

//STC-SIP���ɵ�Timer2(12T,16λ�Զ���װģʽ)
void Timer2Init(void)		//1000΢��@12.000MHz
{
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0x18;		//���ö�ʱ��ʼֵ
	T2H = 0xFC;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	IE2|=0X04;//ͷ�ļ���û��ET2��λ����
	EA=1;
	/*���ж����ȼ�����λ���ж����ȼ��̶�Ϊ0��*/
}


/*
void Timer0_Routine(void) interrupt 1
{
	
}
*/

/*
void Timer2_Routine(void) interrupt 12
{
	
}
*/
