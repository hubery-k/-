#include "LED.h"
#include "RELAY.h"
#include "BUZZER.h"
#include "Nixie.h"
#include "DS18B20.h"
#include "KEY.h"
#include "Delay.h"
#include "Timer.h"

unsigned char code Nodot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code dot_SMG[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};


unsigned char PWM_workMode=1;		//Ĭ�Ϲ���״̬Ϊ1
unsigned char T_workMode=4;
unsigned char Work_Time;		//Ĭ�Ϲ���ʱ��Ϊ0
unsigned char workTime_flag;
unsigned char T;
unsigned char Compare;
unsigned int Time_Count;
unsigned char PWM_Count;
bit Interface;		//��ʾ���棨0-PWM���棬1-�¶Ƚ��棩
sbit Input=P3^4;


void Sys_Init(void)
{
	LED_Close(0XFF);
	RELAY_Close();
	BUZZER_Close();
	Nixie_NoDisplay();
	P2&=0X1F;		//����
}

void USE_DS18B20(void)
{
	DS18B20_ConvertT();
	T=DS18B20_ReadT();		//ת��Ϊ��������
}

void USE_KEY(void)
{
	KEY_Press();
	if(KeyNum==1)		//S4-ģʽ�л�����
	{
		PWM_workMode++;
		if(PWM_workMode==4){PWM_workMode=1;}
		KeyNum=0;
	}
	if(KeyNum==2)		//S5-��ʱʱ�䰴��
	{
		Work_Time+=60;
		workTime_flag++;
		if(workTime_flag==3){Work_Time=0;workTime_flag=0;}
		KeyNum=0;
	}
	if(KeyNum==3)		//S6-ֹͣ����
	{
		Work_Time=0;
		KeyNum=0;
	}
	if(KeyNum==4)		//S7-�����л�����
	{
		if(Interface==0){Interface=1;}
		else if(Interface==1){Interface=0;}
		KeyNum=0;
	}
}

void USE_SMG(void)
{
	if(Interface==0)
	{
		Nixie_Display(0,0XBF);
		Nixie_Display(1,Nodot_SMG[PWM_workMode]);
		Nixie_Display(2,0XBF);
		
		Nixie_Display(4,Nodot_SMG[0]);
		Nixie_Display(5,Nodot_SMG[Work_Time/100]);
		Nixie_Display(6,Nodot_SMG[(Work_Time/10)%10]);
		Nixie_Display(7,Nodot_SMG[Work_Time%10]);
	}
	else if(Interface==1)
	{
		Nixie_Display(0,0XBF);
		Nixie_Display(1,Nodot_SMG[T_workMode]);
		Nixie_Display(2,0XBF);
		
		Nixie_Display(5,Nodot_SMG[T/10]);
		Nixie_Display(6,Nodot_SMG[T%10]);
		Nixie_Display(7,0XC6);
	}
	Nixie_ALL();
}

void USE_LED(void)
{
	if(PWM_workMode==1&&Work_Time!=0){LED_Open(0X01);}
	else if(PWM_workMode==2&&Work_Time!=0){LED_Open(0X02);}
	else if(PWM_workMode==3&&Work_Time!=0){LED_Open(0X04);}
	else{LED_Close(0XFF);}
}
void USE_PWM(void)
{
	if(PWM_workMode==1){Compare=2;}
	else if(PWM_workMode==2){Compare=3;}
	else if(PWM_workMode==3){Compare=7;}
}

/*100us�Ķ�ʱ�жϺ���*/
void Timer0_Routine(void) interrupt 1
{
	if(Work_Time!=0)
	{
		Time_Count++;
		if(Time_Count==10000)		//�ﵽ1s
		{
			Work_Time--;
			Time_Count=0;
		}
	}
	PWM_Count++;
	if(PWM_Count<Compare)		//����Ϊ1ms
	{
		Input=1;
	}
	else
	{
		Input=0;
	}
	PWM_Count%=10;
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	DS18B20_ConvertT();		//��ʼ��ʱת��һ���¶ȣ���ֹ�ϵ��ȡ�¶����ݳ���
	Delay1000ms();
	while(1)
	{
		USE_DS18B20();
		USE_KEY();
		USE_SMG();
		USE_LED();
		USE_PWM();
	}
}