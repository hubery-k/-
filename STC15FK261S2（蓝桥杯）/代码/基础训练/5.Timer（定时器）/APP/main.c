#include "Delay.h"
#include "KEY.h"
#include "BUZZER.h"
#include "Timer.h"
#include "Nixie.h"

unsigned char Hour;
unsigned char Min;
unsigned char Sec;
void main(void)
{
	BUZZER_Init();
	//Timer0Init();
	Timer2Init();
	while(1)
	{
		Nixie_show(0,Hour/10);
		Nixie_show(1,Hour%10);
		Nixie_show(2,16);
		Nixie_show(3,Min/10);
		Nixie_show(4,Min%10);
		Nixie_show(5,16);
		Nixie_show(6,Sec/10);
		Nixie_show(7,Sec%10);
	}
}

void Timer2_Routine(void) interrupt 12
{
	static unsigned int Count;

	if(++Count>=1000)	//��ʱ����Ƶ��1s
	{
		Count=0;
		Sec++;			//1�뵽��Sec����
		if(Sec>=60)
		{
			Sec=0;		//60�뵽��Sec��0��Min����
			Min++;
			if(Min>=60)
			{
				Min=0;	//60���ӵ���Min��0��Hour����
				Hour++;
				if(Hour>=24)
				{
					Hour=0;	//24Сʱ����Hour��0
				}
			}
		}
	}
}