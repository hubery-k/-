#include "reg52.h"
#include "iic.h"

unsigned char dat1=0,dat2=0,dat3=0;

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
//=========================24C02====================
unsigned char tmp=0;
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
//================SMG==================================
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
//====================³õÊ¼»¯====================
void Init_Sys()
{
	Sec_HC573(4);
	P0=0xff;
	Sec_HC573(5);
	P0=0x00;
	Sec_HC573(0);
	
	dat1=Read_24C02(0x01);
	dat2=Read_24C02(0x03);
	dat3=Read_24C02(0x05);
	
	dat1+=1;dat2+=2;dat3+=3;
	if(dat1>10)
	{
		dat1=0;
	}
	if(dat2>20)
	{
		dat2=0;
	}
	if(dat3>30)
	{
		dat3=0;
	}
	
	
	Write_24C02(0x01,dat1);
	Delay(2000);
	Write_24C02(0x03,dat2);
	Delay(2000);
	Write_24C02(0x05,dat3);
	Delay(2000);
}
void main()
{
	Init_Sys();
	while(1)
	{
		DisplaySMG();
	}
}
