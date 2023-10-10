#include "reg52.h"


unsigned char count_t=0;
unsigned int count_f=0;
unsigned int dat=0;
unsigned char code duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void Sec_HC573(unsigned char n)
{
	switch(n)
	{
		case 4:P2=(P2&0X1F)|0X80;break;
		case 5:P2=(P2&0X1F)|0Xa0;break;
		case 6:P2=(P2&0X1F)|0Xc0;break;
		case 7:P2=(P2&0X1F)|0Xe0;break;
		case 0:P2=(P2&0X1F)|0X00;break;
	}
}

void Delay(unsigned char t)
{
	while(t--);
}
void Init_Sys()
{
	Sec_HC573(4);
	P0=0xff;
	Sec_HC573(5);
	P0=0x00;
	Sec_HC573(0);
}

//===================定时器====================
void INIT_Timer()//T0使用八位自动重装模式用于计数
{//T1用于定时
	TH0=0XFF;//八位重装两个值都赋一样的
	TL0=0XFF;
	
	TH1=(65535-50000)/256;
	TL1=(65535-50000)%256;
	
	TMOD=0X16;
	ET0=1;
	ET1=1;
	TR0=1;
	TR1=1;
	EA=1;
}

void T0_Service() interrupt 1
{
	count_f++;
}
void T1_Service() interrupt 3
{
	TH1=(65535-50000)/256;
	TL1=(65535-50000)%256;
	count_t++;
	if(count_t==20)//达到1s
	{
		dat=count_f;
		count_f=0;
		count_t=0;
	}
	
}

//===================SMG=======================
void DisplaySMG_ALL()
{
	Sec_HC573(6);
	P0=0xff;
	Sec_HC573(7);
	P0=0xff;
	Sec_HC573(0);
}

void DisplaySMG_Bit(unsigned char pos,dat)
{
	Sec_HC573(7);
	P0=0xff;
	
	Sec_HC573(6);
	P0=0x01<<pos;
	Sec_HC573(7);
	P0=dat;
	Sec_HC573(0);
}

void DisplaySMG()
{
	DisplaySMG_Bit(0,duanma[15]);//显示提示符F
	Delay(500);
	DisplaySMG_Bit(1,0xff);//显示提示符F
	Delay(500);
	DisplaySMG_Bit(2,0xff);//显示提示符F
	Delay(500);
	
	if(dat>9999)
	{
		DisplaySMG_Bit(3,duanma[dat/10000]);//显示提示符F
		Delay(500);
	}
	if(dat>999)
	{
		DisplaySMG_Bit(4,duanma[(dat/1000)%10]);//显示提示符F
		Delay(500);
	}
	if(dat>99)
	{
		DisplaySMG_Bit(5,duanma[(dat/100)%10]);//显示提示符F
		Delay(500);
	}
	if(dat>9)
	{
		DisplaySMG_Bit(6,duanma[(dat/10)%10]);//显示提示符F
		Delay(500);
	}
		DisplaySMG_Bit(7,duanma[dat%10]);//显示提示符F
		Delay(500);
	
	DisplaySMG_ALL();
}
void main()
{
	Init_Sys();
	INIT_Timer();
	while(1)
	{
		DisplaySMG();
	}
}