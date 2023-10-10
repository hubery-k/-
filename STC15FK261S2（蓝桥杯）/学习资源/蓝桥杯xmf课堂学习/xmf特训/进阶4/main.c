#include "reg52.h"
#include "iic.h"

sbit S4=P3^3;
sbit S5=P3^2;
sbit S6=P3^1;
unsigned char dat1=0,dat2=0,dat3=0;
unsigned char duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
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

void Delay(unsigned int t)
{
	while(t--);
}
//====================24C02=========================
void Write_24C02(unsigned char addr,dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}
unsigned char Read_24C02(unsigned char addr)
{
	unsigned char tmp=0;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	tmp=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return tmp;
}

//====================SMG=========================
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
	DisplaySMG_Bit(2,0xbf);
	Delay(500);
	DisplaySMG_Bit(5,0xbf);
	Delay(500);
	
	DisplaySMG_Bit(0,duanma[dat1/10]);
	Delay(500);
	DisplaySMG_Bit(1,duanma[dat1%10]);
	Delay(500);
	DisplaySMG_Bit(3,duanma[dat2/10]);
	Delay(500);
	DisplaySMG_Bit(4,duanma[dat2%10]);
	Delay(500);
	DisplaySMG_Bit(6,duanma[dat3/10]);
	Delay(500);
	DisplaySMG_Bit(7,duanma[dat3%10]);
	Delay(500);
	
	DisplaySMG_ALL();
}
//=====================duli_Key=========================
void duli_Key()
{
	if(S4==0)
	{
		Delay(100);
		if(S4==0)
		{
			dat1++;
			if(dat1>13)
			{
				dat1=0;
			}
			Write_24C02(0x00,dat1);
			while(S4==0)
			{
				DisplaySMG();
			}
		}
	}
	if(S5==0)
	{
		Delay(100);
		if(S5==0)
		{
			dat2++;
			if(dat2>13)
			{
				dat2=0;
			}
			Write_24C02(0x01,dat2);
			while(S5==0)
			{
				DisplaySMG();
			}
		}
	}
	if(S6==0)
	{
		Delay(100);
		if(S6==0)
		{
			dat3++;
			if(dat3>13)
			{
				dat3=0;
			}
			Write_24C02(0x02,dat3);//然后操作过后将dat1的数据写回0x01
			while(S6==0)
			{
				DisplaySMG();
			}
		}
	}
}

void Init_Sys()
{
	Sec_HC573(5);
	P0=0x00;
	Sec_HC573(4);
	P0=0xff;
	Sec_HC573(0);
	
	dat1=Read_24C02(0x00);
	dat2=Read_24C02(0x01);
	dat3=Read_24C02(0x02);
}

void main()
{
	Init_Sys();
	while(1)
	{
		DisplaySMG();
		duli_Key();
	}
}