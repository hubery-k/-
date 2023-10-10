#include "reg52.h"
#include "iic.h"

sbit L1=P0^0;
sbit L2=P0^1;
sbit L3=P0^2;
sbit L4=P0^3;
sbit L5=P0^4;
sbit L6=P0^5;
sbit L7=P0^6;
sbit L8=P0^7;

sbit S4=P3^3;
sbit S5=P3^2;
void LED_Running();
unsigned char level=0;
unsigned char adc_value=0;
float adc_volt=0;
unsigned int tmp=0;
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
void Delay(unsigned int t)
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
//===================PCF8591==========================
void Read_PCF()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x01);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	adc_value=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	
	
	adc_volt=adc_value*(5.0/255);
	tmp=adc_volt*100;
}
//===========24C02====================================
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
	unsigned char temp=0;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	temp=IIC_RecByte(); 
	IIC_SendAck(1);
	IIC_Stop();
	return temp;
}
//=====================SMG============================
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
	DisplaySMG_Bit(0,0xbf);
	Delay(500);
	DisplaySMG_Bit(1,duanma[level]);
	Delay(500);
	DisplaySMG_Bit(2,0xbf);
	Delay(500);
	
	DisplaySMG_Bit(5,duanma[tmp/100]);
	Delay(500);
	DisplaySMG_Bit(6,duanma[(tmp/10)%10]);
	Delay(500);
	DisplaySMG_Bit(7,duanma[tmp%10]);
	Delay(500);
	DisplaySMG_ALL();
}

//==============KEY==================================
void duli_Key()
{
	if(S4==0)
	{
		Delay(100);
		if(S4==0)
		{
			Write_24C02(0x01,level);
			Delay(2000);//等待数据写入完成
			Write_24C02(0x02,tmp);
		}
		while(S4==0)
		{
			DisplaySMG();
			LED_Running();
		}
	}
	if(S5==0)
	{
		Delay(100);
		if(S5==0)
		{
			tmp=Read_24C02(0x02);
			level=Read_24C02(0x01);
		}
		while(S5==0)
		{
			DisplaySMG();
			LED_Running();
		}
	}
}
//=====================LED_Running===================
void LED_Running()
{
	Sec_HC573(4);
	P0=0xff;
	if(tmp>200)
	{
		level=1;
		L1=0;
	}
	else if(tmp>150)
	{
		level=2;
		L1=0;
		L2=0;
	}
	else if(tmp>100)
	{
		level=3;
		L1=0;
		L4=0;
	}
	else if(tmp>50)
	{
		level=4;
		L1=0;
		L6=0;
	}
	else if(tmp>0)
	{
		level=5;
		L1=0;
		L8=0;
	}
}
//=========================================
void main()
{
	Init_Sys();
	while(1)
	{
		Read_PCF();
		DisplaySMG();
		LED_Running();
		duli_Key();
	}
}