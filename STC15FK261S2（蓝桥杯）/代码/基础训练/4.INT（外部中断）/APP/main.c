#include "LED.h"
#include "KEY.h"
#include "BUZZER.h"
#include "INT.h"
#include "Delay.h"

unsigned char flag=0;
unsigned char flag1=0;
void main(void)
{
	BUZZER_Init();
	LED_Init();
	INT0_Init();
	INT1_Init();
	while(1)
	{
		/*
		//��ѯ��ʽ
		if(IE0==1)��
		{
			Delay20ms();
			IE0=0;//�ж�û����Ӧ���ֶ���0
			LED1=~LED1;
		}*/
		if(flag==1)
		{
			Delay20ms();//��������
			flag=0;
			LED1=~LED1;
		}
		
		if(flag1==1)
		{
			Delay20ms();//��������
			flag1=0;
			LED2=~LED2;
		}
	}
}
void INT0_Routine(void) interrupt 0
{
		flag=1;
}
void INT1_Routine(void) interrupt 2
{
		flag1=1;
}