#include "KEY.h"
#include "BUZZER.h"
#include "Timer.h"
#include "LED.h"

unsigned char Compare;
void main(void)
{
	BUZZER_Init();
	Timer0Init();
	LED_Init();
	while(1)
	{
		if(MatrixKey()==1){Compare=0;}
		if(MatrixKey()==2){Compare=20;}
		if(MatrixKey()==3){Compare=60;}
		if(MatrixKey()==4){Compare=100;}
	}
}

void Timer0_Routine(void) interrupt 1
{
	static unsigned char Count;
	Count++;
	if(Count<Compare)
	{
		LED1=0;
	}
	else if(Count>=Compare)
	{
		LED1=1;
	}
	Count%=100;
}