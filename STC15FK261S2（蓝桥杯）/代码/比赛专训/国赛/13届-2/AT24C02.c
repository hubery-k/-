#include "AT24C02.h"

#define AT24C02_ADDR 0XA0

void AT24C02_WriteByte(uc Addr,Dat)
{
	I2CStart();
	I2CSendByte(AT24C02_ADDR|0X00);
	I2CWaitAck();
	I2CSendByte(Addr);
	I2CWaitAck();
	I2CSendByte(Dat);
	I2CSendAck(1);
	I2CStop();
}

uc AT24C02_ReadByte(uc Addr)
{
	uc Dat;
	
	I2CStart();
	I2CSendByte(AT24C02_ADDR|0X00);
	I2CWaitAck();
	I2CSendByte(Addr);
	I2CWaitAck();
	I2CStop();
	
	I2CStart();
	I2CSendByte(AT24C02_ADDR|0X01);
	I2CWaitAck();
	Dat=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	
	return Dat;
}