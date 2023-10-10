#include "Nixie.h"
#include "LED.h"
#include "KEY.h"
#include "BUZZER.h"
#include "Delay.h"


void main(void)
{
	LED_Init();
	while(1)
	{
		KEY_Press();
		if(KeyNum==1)
		{
//			Nixie_show();
			LED1=0;
			LED2=1;
			LED3=1;
			LED4=1;
		}
		else if(KeyNum==2)
		{
			LED2=0;
			LED1=1;
			LED3=1;
			LED4=1;
		}
		else if(KeyNum==3)
		{
			LED3=0;
			LED1=1;
			LED2=1;
			LED4=1;
		}
		else if(KeyNum==4)
		{
			LED4=0;
			LED2=1;
			LED3=1;
			LED1=1;
		}
	}
}