#include "reg52.h"


sbit S4=P3^3;
unsigned char duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

unsigned char num=28;
unsigned char t_S4=0;
bit F_Key=0;
void SelectHC573(unsigned char n)
{
	switch(n)
	{
		case 4:P2=(P2&0x1f)|0x80;break;
		case 5:P2=(P2&0x1f)|0xa0;break;
		case 6:P2=(P2&0x1f)|0xc0;break;
		case 7:P2=(P2&0x1f)|0xe0;break;
		case 0:P2=(P2&0x1f)|0x00;break;
	}
}

void Init_Sys()
{
	SelectHC573(4);
	P0=0xff;
	SelectHC573(5);
	P0=0x00;
	SelectHC573(0);
}

void Delay(unsigned char t)
{
	while(t--);
}
//=================定时器=======================
unsigned char count=0;

void Init_Timer0()
{
	TMOD=0X01;
	TH0=(65535-1000)/256;
	TL0=(65535-1000)%256;
	
	ET0=1;
	EA=1;
	TR0=1;
}

void Service_Timer0() interrupt 1
{
	TH0=(65535-10000)/256;
	TL0=(65535-10000)%256;
	
	count++;
	if(F_Key==1)
	{
		count++;
	}
}
//================SMG===========================
void DisplaySMG_ALL()
{
	SelectHC573(6);
	P0=0xff;
	SelectHC573(7);
	P0=0xff;
	SelectHC573(0);
}

void DisplaySMG_Bit(unsigned char pos,dat)
{
	SelectHC573(7);
	P0=0xff;
	
	SelectHC573(6);
	P0=0x01<<pos;
	SelectHC573(7);
	P0=dat;
	SelectHC573(0);
}

void DisplaySMG()
{
	DisplaySMG_Bit(6,duanma[num/10]);
	Delay(500);
	DisplaySMG_Bit(7,duanma[num%10]);
	Delay(500);
	
	
	DisplaySMG_ALL();
}
//===================按键=============================
void duli_KEY()
{
	if(S4==0)
	{
		Delay(100);
		if(S4==0)
		{
			F_Key=1;
			count=0;
			while(S4==0)
			{
				DisplaySMG();
			}
			F_Key=0;
			if(count>100)//长按
			{
				num=0;
			}
			else if(count<100)//短按
			{
				num++;
				if(num>99)
				{
					num=0;
				}
			}
		}
	}
}
void main()
{
	Init_Sys();
	Init_Timer0();
	while(1)
	{
		duli_KEY();
		DisplaySMG();
	}
	
}