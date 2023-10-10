#include "NE555.h"
#include "Timer.h"
/*�õ�������ʱ����һ������T0��ʱ��һ������T1����*/
/*����ʱ��ʹ��Ƶ����12Mhz��Ϊ12Tģʽʱ��һ������ʱ����1us*/
/*T0��Ĭ�����ȼ�����T1��Ĭ�����ȼ�*/
/*NE555�����źŵ�������P34���ţ���P34����Ҳ��T0��ʱ������*/
unsigned int F;
unsigned int count;
unsigned int temp;
void NE555_Init(void)
{
	Timer0Init();
	Timer1Init();
}

void Timer0_Routine(void) interrupt 1
{
	F++;
}

void Timer1_Routine(void) interrupt 3
{
	count++;
	if(count>=1000)
	{
		temp=F;
		count=0;
		F=0;
	}
}
