#include "LED.h"
#include "RB.h"
#include "DS18B20.h"
#include "CLOCK.h"
#include "Timer.h"
#include "Nixie.h"
#include "Delay.h"
#include "KEY.h"
unsigned char code Donot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code not_SMG[]={0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10};
unsigned char Time[]={0X50,0X59,0X23};		//�룬�֣�ʱ(BCD���ʾ)


unsigned char Col_T[10];		//�洢�ɼ�����10���¶�����
unsigned char Show_state;		//������ʾ��0-�������ý���  1-ʱ����ʾ���棩
unsigned char Interval=1;		//�ɼ�ʱ������1s��5s��30s��60s��
unsigned char Col_Interval;		//ȷ�ϲɼ�ʱ��
unsigned char i;
unsigned char index;		//�ɼ������¶ȵ�����ֵ��0-9��

unsigned char KeyNum;
bit Col_flag;		//�ռ��¶ȱ�־��ÿ�ռ�һ���¶ȳɹ��������
bit Remind_symbol;		//��ʾ������ʾ״̬
bit Col_state;		//�ռ��¶�״̬��1-��ʼ�ռ�  0-ֹͣ�ռ���
bit LED_flag;		//LED1״̬��0-Ϩ��  1-������
bit LED_state;		//LED2״̬��0-Ϩ��  1-������

void Iniy_Sys(void)
{
	LED_Control(0X00);
	RB_Control(0X00);
	P2&=0X1F;//����
}

void USE_DS18B20(void)
{
	if(Col_flag==1)		//Col_flag��־λ���ж�����1�����Զ�ʱ�жϳ��򲻻������¶ȵĲɼ�
	{
		DS18B20_ConvertT();
		Col_T[i]=DS18B20_ReadT();
		i++;
		Col_flag=0;
	}
}

void USE_KEY(void)
{
	KeyNum=KEY();
	
	if(KeyNum==4)		//S4����
	{
		KeyNum=0;
		if(Show_state==0)
		{
			if(Interval==1){Interval=5;}
			else if(Interval==5){Interval=30;}
			else if(Interval==30){Interval=60;}
			else if(Interval==60){Interval=1;}
		}
	}
	if(KeyNum==5)		//S5����
	{
		if(Show_state==0)
		{
			Show_state=1;		//����ʱ����ʾ����
			Col_state=1;		//��ʼ�ռ��¶�
		}
	}
	if(KeyNum==6)		//S6����
	{
		if(Show_state==2)
		{
			LED_flag=0;
			index++;
			if(index>9){index=0;}
		}
	}
	if(KeyNum==7)		//S7����
	{
		LED_flag=0;
		Show_state=0;		//�����������ҳ��
		Interval=1;		//�ָ�ΪĬ�ϲɼ�ʱ����
	}
}

void USE_SMG(void)
{
	if(Show_state==0)		//�������ý���
	{
		Nixie_SetBuf(0,0XFF);	
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		
		Nixie_SetBuf(5,0XBF);
		Nixie_SetBuf(6,Donot_SMG[Interval/10]);
		Nixie_SetBuf(7,Donot_SMG[Interval%10]);
	}
	else if(Show_state==1)		//ʱ����ʾ����
	{
		Nixie_SetBuf(0,Donot_SMG[Time[2]/16]);		//����16����Ϊ��BCD��
		Nixie_SetBuf(1,Donot_SMG[Time[2]%16]);
		Nixie_SetBuf(3,Donot_SMG[Time[1]/16]);
		Nixie_SetBuf(4,Donot_SMG[Time[1]%16]);
		Nixie_SetBuf(6,Donot_SMG[Time[0]/16]);
		Nixie_SetBuf(7,Donot_SMG[Time[0]%16]);
		if(Remind_symbol==0)
		{
			Nixie_SetBuf(2,0XBF);
			Nixie_SetBuf(5,0XBF);
		}
		else
		{
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(5,0XFF);
		}
	}
	else if(Show_state==2)		//�¶Ȳɼ���ʾ����
	{
		Nixie_SetBuf(0,0XBF);
		Nixie_SetBuf(1,Donot_SMG[index/10]);		
		Nixie_SetBuf(2,Donot_SMG[index%10]);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XBF);
		Nixie_SetBuf(6,Donot_SMG[Col_T[index]/10]);		//��ʾ�ɼ����¶�����
		Nixie_SetBuf(7,Donot_SMG[Col_T[index]%10]);
	}
}

void USE_LED(void)
{
	if(LED_state==1)
	{
		LED_Control(0X01);
	}
	else{LED_Control(0X00);}
	
}

/*1ms���жϺ���*/
void Timer0_Routine(void) interrupt 1
{
	static unsigned int T0_count;
	static unsigned int T0_count1;
	static unsigned char smg_count;
	static unsigned char key_count;
	static unsigned int led_cnt;
	
		/*ɨ������ܺͰ���*/
	smg_count++;
	key_count++;
	if(smg_count==2)
	{
		smg_count=0;
		Nixie_Loop();
	}
	if(key_count==10)
	{
		key_count=0;
		Key_Loop();
	}
	
	if(Show_state==1)
	{
		T0_count++;
		if(T0_count==1000)		//�ﵽ1s
		{
			Remind_symbol=~Remind_symbol;
			T0_count=0;
		}
	}

	if(Col_flag==0)		//��ֹһ���¶�����û�вɼ��꿪ʼ�˵ڶ��βɼ�
	{
		if(Col_state==1)
		{
			T0_count1++;
			if(T0_count1==(Interval*1000))		//�ﵽ�ռ�1���¶ȵ�ʱ��
			{
				if(Col_Interval==Interval*10)	//�ռ�10����ɺ�
				{
					Col_state=0;		//ֹͣ�ռ�
					Col_Interval=0;		//�ռ����ʱ������
					i=0;		//�����������
					Show_state=2;		//�ռ����¶Ƚ����¶Ȳɼ���ʾ����
					LED_flag=1;
				}
				T0_count1=0;
				Col_flag=1;//�ռ��¶ȱ�־
				Col_Interval+=Interval;
			}
		}
	}
	
	if(LED_flag==1)
	{
		led_cnt++;
		if(led_cnt<500)
		{LED_state=1;}
		else {LED_state=0;}
		if(led_cnt==1000)led_cnt=0;
	}else {LED_state=0;}
}

void main(void)
{
	Iniy_Sys();
	Timer0Init();
	Write_time(Time);
	DS18B20_ConvertT();//�ϵ��һ����ת��һ�����ݣ���ֹһ�ϵ�����ܲ���������ʾ�������¶�����
	Delay1000ms();//�ȴ��¶�ת��������ϵ���ʾ
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		Read_time(Time);
		USE_KEY();
		USE_DS18B20();
		USE_SMG();
	}
}
/*����LED�������жϹرգ��жϻ�Ӱ��LED����ʾ*/
/*
	�������¶Ȳɼ��Ѿ����жϳ����γ�ǰ��̨�����Ҳɼ�һ���¶ȵ�ʱ�����100ms�����Ϊ0.1s���������жϳ��򲻻�Ӱ��onewireʱ������ݴ��䡣
	�������жϲ�û��ǡ��û��Ӱ��ʱ��Ķ�ȡ������ȡʱ���������ʱֻ�����ж����ñ�־λ�ﵽ50ms��ȡһ��ʱ��ֵ��
*/