#include "AT24C02.h"
#include "iic.h"

#define AT24C02_ADDRESS  0XA0
//地址范围由于定义的是unsigned char型，所以在0~255都可以
void AT24C02_WriteByte(unsigned char Addr,Date)
{
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(Addr);
	IIC_WaitAck();
	IIC_SendByte(Date);
	IIC_WaitAck();
	IIC_Stop();
}

unsigned char AT24C02_ReadByte(unsigned char Addr)
{
	unsigned char Data;
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(Addr);
	IIC_WaitAck();
	
	
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS|0x01);
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return Data;
}
