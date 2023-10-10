#include "Timer.h"
#include "KEY.h"
#include "LED.h"
#include "Nixie.h"


unsigned char code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};

unsigned char KeyNum;
unsigned int T1Count0;
bit flag;
void Sys_Init()
{
	LED_Control(0X00);
}

/*1ms�Ķ�ʱ�жϺ���*/
void Timer0_Routine(void) interrupt 1
{
	static unsigned char T0Count1;
	static unsigned char T0Count2;
	T0Count1++;T0Count2++;
	if(T0Count1==10)
	{
		T0Count1=0;
		KEY_Loop();
	}
	if(T0Count2==2)
	{
		T0Count2=0;
		Nixie_Loop();
	}
}

/*1ms�Ķ�ʱ�жϺ���*/
void Timer1_Routine(void) interrupt 3
{
	T1Count0++;
	if(T1Count0>1000){flag=1;}		//�жϰ��������ɿ��Ƿ񵽴�1s������1s����Ϊ������
	else if(T1Count0<1000){flag=0;}
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)
	{
		LED_Control(0X01);
	}
	if(KeyNum==5)
	{
		LED_Control(0X02);
	}
	if(KeyNum==6)
	{
		LED_Control(0X04);
	}
	if(KeyNum==7 && flag!=1)		//����û��1s
	{
		T1Count0=0;
		LED_Control(0X08);
		
	}
	if(KeyNum==7 && flag==1)		//���³���1s
	{
		T1Count0=0;
		LED_Control(0X00);
	}
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	
	while(1)
	{
		USE_KEY();
		Nixie_SetBuf(0,Nodot_SMG[T1Count0/10000]);
		Nixie_SetBuf(1,Nodot_SMG[T1Count0/1000%10]);
		Nixie_SetBuf(2,Nodot_SMG[T1Count0/100%10]);
		Nixie_SetBuf(3,Nodot_SMG[T1Count0/10%10]);
		Nixie_SetBuf(4,Nodot_SMG[T1Count0%10]);
	}
}