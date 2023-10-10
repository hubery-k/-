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

	if(++Count>=1000)	//定时器分频，1s
	{
		Count=0;
		Sec++;			//1秒到，Sec自增
		if(Sec>=60)
		{
			Sec=0;		//60秒到，Sec清0，Min自增
			Min++;
			if(Min>=60)
			{
				Min=0;	//60分钟到，Min清0，Hour自增
				Hour++;
				if(Hour>=24)
				{
					Hour=0;	//24小时到，Hour清0
				}
			}
		}
	}
}