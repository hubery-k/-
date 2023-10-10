#include "AT24C02.h"
#include "iic.h"

#define AT24C02_ADDRESS 0XA0

void EEPROM_Write(uc addr,f dat)
{
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}

uc EEPROM_Read(uc addr)
{
	f dat;
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(addr);		//定位到数据存入地址
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS|0X01);
	IIC_WaitAck();
	dat=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return dat;
}