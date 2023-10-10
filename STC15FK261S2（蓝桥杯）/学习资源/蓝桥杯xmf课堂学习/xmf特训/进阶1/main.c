#include "reg52.h"

sbit L1=P0^0;
sbit L2=P0^1;
sbit L8=P0^7;


unsigned char code duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

unsigned char count_Ll2f=0;//L1、L2同时翻转计数
unsigned char count_L8f=0;//L8闪烁计数
bit L12_stat=0;
bit L8_stat=0;
unsigned char C_L12=0;
unsigned char C_L8=0;
//==================================================
void Select_HC573(unsigned char n,dat)
{
	P0=dat;
	switch(n)
	{
		case 4:P2=(P2&0x1f)|0x80;break;
		case 5:P2=(P2&0x1f)|0xa0;break;
		case 6:P2=(P2&0x1f)|0xc0;break;
		case 7:P2=(P2&0x1f)|0xe0;break;
		case 0:P2=(P2&0x1f)|0x00;break;//所有锁存器不选择
	}
	P2=(P2&0x1f)|0x00;//设置完成，自动关闭所有锁存器
}

void DisplaySMG_ALL()
{
	Select_HC573(6,0xff);
	Select_HC573(7,0xff);
}

void Delay(unsigned int t)
{
	while(t--);
}

//=====================定时器==========================

void Init_Timer0()
{
	TMOD=0X01;
	TH0=(65535-10000)/256;
	TL0=(65535-10000)%256;
	
	ET0=1;
	EA=1;
	TR0=1;
}

void Service_Timer0() interrupt 1
{
	TH0=(65535-10000)/256;
	TL0=(65535-10000)%256;
	count_Ll2f++;
	count_L8f++;
	
	if(count_Ll2f==100)//定时1s
	{
		L12_stat=~L12_stat;
		C_L12++;
		count_Ll2f=0;
		if(C_L12>(9*2))
		{
			C_L12=0;
		}
	}
	if(count_L8f==100)
	{
		L8_stat=~L8_stat;
		C_L8++;
		count_L8f=0;
		if(C_L8>(99*2))
		{
			C_L8=0;
		}
	}
	
}
//=======================SMG========================
void DisplaySMG_Bit(unsigned char pos,dat)
{
	Select_HC573(6,0x01<<pos);
	Select_HC573(7,dat);
	
	Select_HC573(6,0x01<<pos);
	Select_HC573(7,0xff);
}



void DisplaySMG()
{
	
	DisplaySMG_Bit(0,duanma[C_L12/2]);
	Delay(100);

	DisplaySMG_Bit(6,duanma[(C_L8/2)/10]);
	Delay(100);
	DisplaySMG_Bit(7,duanma[(C_L8/2)%10]);
	Delay(100);
	
	
	DisplaySMG_ALL();
}

//======================LED=========================
void LED_Running()
{	
	if(L8_stat==1)
	{
		Select_HC573(4,0x7f);
	}
	if(L12_stat==1)
	{
		Select_HC573(4,0xfc);
	}
	Delay(500);
}
//====================设备初始化==================
void Init_System()
{
	Select_HC573(0,0x00);
	Select_HC573(4,0xff);
	Select_HC573(5,0x00);
	DisplaySMG_ALL();
}
void main()
{
	Init_System();
	Init_Timer0();
	while(1)
	{
		LED_Running();
		DisplaySMG();
	}
}