#include "AT24C02.h"
#include "iic.h"

#define AT24C02_ADDRESS 0XA0

void AT24C02_Write(uc Addr,Dat)
{
	I2CStart();
	I2CSendByte(AT24C02_ADDRESS);
	I2CWaitAck();
	I2CSendByte(Addr);
	I2CWaitAck();
	I2CSendByte(Dat);
	I2CWaitAck();
	I2CStop();
}

uc AT24C02_Read(uc Addr)
{
	uc Dat;
	I2CStart();
	I2CSendByte(AT24C02_ADDRESS);
	I2CWaitAck();
	I2CSendByte(Addr);	//定位到该数据存储的地址
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(AT24C02_ADDRESS|0X01);
	I2CWaitAck();
	Dat=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	
	return Dat;
}