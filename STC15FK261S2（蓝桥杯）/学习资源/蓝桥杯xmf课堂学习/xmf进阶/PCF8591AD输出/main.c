#include "reg52.h"
#include "iic.h"

sbit S4=P3^3;

unsigned char stat=1;

float adc_volt=0;
unsigned char adc_value=0;
unsigned int smg_volt=0;

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
//=========================PCF8591====================
void Read_pcf()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	if(stat==1)
	{
		IIC_SendByte(0x01);
	}
	else if(stat==3)
	{
		IIC_SendByte(0x03);
	}
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	adc_value=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	
	adc_volt=adc_value*(5.0/255);
	smg_volt=adc_volt*100;
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
	DisplaySMG_Bit(1,duanma[stat]);
	Delay(500);
	DisplaySMG_Bit(2,0xbf);
	Delay(500);
	
	DisplaySMG_Bit(5,duanma[smg_volt/100]);
	Delay(500);
	DisplaySMG_Bit(6,duanma[(smg_volt/10)%10]);
	Delay(500);
	DisplaySMG_Bit(7,duanma[smg_volt%10]);
	Delay(500);
	DisplaySMG_ALL();
}

//======================duli_Key=====================
unsigned char S4_stat=0;
void duli_Key()
{
	if(S4==0)
	{
		Delay(100);
		if(S4==0)
		{
			if(S4_stat==0)
			{
				stat=3;
				S4_stat=1;
			}
			else if(S4_stat==1)
			{
				stat=1;
				S4_stat=0;
			}
			while(S4==0)
			{
				DisplaySMG();
			}
		}
	}
}
void Init_Sys()
{
	Sec_HC573(4);
	P0=0xff;
	Sec_HC573(5);
	P0=0x00;
	Sec_HC573(0);
}
void main()
{
	Init_Sys();
	
	while(1)
	{
		Read_pcf();
		duli_Key();
		DisplaySMG();
	}
}