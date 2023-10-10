/*
 * 问题描述：如何合理控制外设，比如，独立按键7 6 5 4,控制led 1 2 3 4怎么让他们互不影响，无关led不会微亮
 * Copyright (c) 2018 电子设计工坊 dianshe.taobao.com
 * All rights reserved
 */
#include "main.h"
/*板子的初始化，系统初始化调用此函数，关闭无关外设*/
void Board_Init()
{
	RnB_OFF;
	NIX_OFF;
	LED_OFF;
}
/*LED测试子函数，系统初始化调用*/
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
/*数码管测试，系统初始化调用*/
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
/*定时器的初始化，定时两毫秒,16位自动重装*/
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
void System_Init()//系统的初始化
{
	Board_Init();//板子外设的初始化
  LED_Test();//led的测试
  Nixie_Test();//数码管的测试
	EA=1;
	Timer0Init();//2ms定时器
	hour=23;
	min=59;
	sec=55;//初始化时间
}
/*将时间数据更新到数码管的显示数组中*/
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
/*按键驱动子函数，主循环中调,里面的键值码已经消抖过*/
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
/*本地控制组，按键控制LED7 LED8,LED6,LED5*/

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
		//主要功能 按键控制对应LED 按一下相应的led亮 再按一下灭 如此循环
		LED8_Judge();
		LED7_Judge();
		LED6_Judge();
		LED5_Judge();//问题就在这里，怎让led互不影响
		Key_Driver();//按键驱动
		Nixie_Refresh();//段选码更新
	}
}
/*定时器计时子函数，中断中调用，定时器驱动时钟*/
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
	
	Time_Driver();//计时
	Nixie_Dis();//数码管显示函数
	KeyScan();//按键扫描函数，用于消抖，获取稳定之后的键值

}