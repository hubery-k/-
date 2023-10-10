/*
 * ������������κ���������裬���磬��������7 6 5 4,����led 1 2 3 4��ô�����ǻ���Ӱ�죬�޹�led����΢��
 * Copyright (c) 2018 ������ƹ��� dianshe.taobao.com
 * All rights reserved
 */
#include "main.h"
/*���ӵĳ�ʼ����ϵͳ��ʼ�����ô˺������ر��޹�����*/
void Board_Init()
{
	RnB_OFF;
	NIX_OFF;
	LED_OFF;
}
/*LED�����Ӻ�����ϵͳ��ʼ������*/
void LED_Test()
{
	u8 i;
	u8 temp=0xfe;
	for(i=0;i<8;i++)
	{
		P0=temp;Y4;Y;
		temp<<=1;
		Delay500ms();
	}
	temp=0x80;
	for(i=0;i<8;i++)
	{
    P0=temp;Y4;Y;
		temp=(temp>>1)|0x80;
		Delay500ms();
	}
}
/*����ܲ��ԣ�ϵͳ��ʼ������*/
void Nixie_Test()
{
	u8 i;
	u8 temp=0x01;
	for(i=0;i<8;i++)
	{
	  P0=0X00;Y6;P0=temp;Y;
	  P0=0XFF;Y7;P0=seg_buff[i];Y;
		temp=(temp<<1)|1;
		Delay500ms();
	}
	temp=0xfe;
	for(i=0;i<8;i++)
	{
	  P0=0X00;Y6;P0=temp;Y;
		temp=temp<<1;
		Delay500ms();
	}
}
/*��ʱ���ĳ�ʼ������ʱ������,16λ�Զ���װ*/
void Timer0Init(void)	
{
	AUXR |= 0x80;	
	TMOD &= 0xF0;	
	TL0 = 0x9A;		
	TH0 = 0xA9;		
	TF0 = 0;		
	TR0 = 1;		
	ET0=1;
}
void System_Init()//ϵͳ�ĳ�ʼ��
{
	Board_Init();//��������ĳ�ʼ��
  LED_Test();//led�Ĳ���
  Nixie_Test();//����ܵĲ���
	EA=1;
	Timer0Init();//2ms��ʱ��
	hour=23;
	min=59;
	sec=55;//��ʼ��ʱ��
}
/*��ʱ�����ݸ��µ�����ܵ���ʾ������*/
void Nixie_Refresh()
{
	seg_buff[0]=seg[hour/10];
	seg_buff[1]=seg[hour%10];
	seg_buff[2]=seg[11];
	seg_buff[3]=seg[min/10];
	seg_buff[4]=seg[min%10];
	seg_buff[5]=seg[11];
	seg_buff[6]=seg[sec/10];
	seg_buff[7]=seg[sec%10];
}
/*���������Ӻ�������ѭ���е�,����ļ�ֵ���Ѿ�������*/
void Key_Driver()
{
	Key_Judge();
	if(key_flag)
	{
		key_flag=0;
		switch(key_val)
		{
			case 7:
				led5_flag=~led5_flag;
				break;
			case 6:
				led6_flag=~led6_flag;
				break;
			case 5:
				led7_flag=~led7_flag;
				break;
			case 4:
				led8_flag=~led8_flag;
				break;
		}
	}
}
/*���ؿ����飬��������LED7 LED8,LED6,LED5*/

void LED5_Judge()
{
	if(led5_flag)
	{
		LED5_ON;
	}
	else
	{
		LED_OFF;
	}
}

void LED6_Judge()
{
	if(led6_flag)
	{
		LED6_ON;
	}
	else
	{
		LED_OFF;
	}
}
void LED7_Judge()
{
	if(led7_flag)
	{
		LED7_ON;
	}
	else
	{
		LED_OFF;
	}
}
void LED8_Judge()
{
	if(led8_flag)
	{
		LED8_ON;
	}
	else
	{
		LED_OFF;
	}
}
void main()
{
	System_Init();
	while(1)
	{
		//��Ҫ���� �������ƶ�ӦLED ��һ����Ӧ��led�� �ٰ�һ���� ���ѭ��
		LED8_Judge();
		LED7_Judge();
		LED6_Judge();
		LED5_Judge();//��������������led����Ӱ��
		Key_Driver();//��������
		Nixie_Refresh();//��ѡ�����
	}
}
/*��ʱ����ʱ�Ӻ������ж��е��ã���ʱ������ʱ��*/
void Time_Driver()
{
	if(++s1_count==500)
	{
		s1_count=0;
		if(++sec==60)
		{
			sec=0;
			if(++min==60)
			{
				min=0;
				if(++hour==24)
				{
					hour=0;
					min=0;
					sec=0;
				}
			}
		}
	}
}
void TIMER0() interrupt 1
{
//	if(++led_count==50)
//	{
//		led_count=0;
//	}
//	Led_Judge();
	
	Time_Driver();//��ʱ
	Nixie_Dis();//�������ʾ����
	KeyScan();//����ɨ�躯����������������ȡ�ȶ�֮��ļ�ֵ

}