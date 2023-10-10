#include "AT24C02.h"

#define AT24C02_ADDR 0XA0


void AT24C02_WriteByte(uc addr,dat)
{
	I2CStart();
	I2CSendByte(AT24C02_ADDR|0x00);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	I2CSendByte(dat);
	I2CWaitAck();
	I2CSendAck(1);
	I2CStop();
}

uc AT24C02_ReadByte(uc addr)
{
	uc dat;
	I2CStart();
	I2CSendByte(AT24C02_ADDR|0x00);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	I2CStop();
	
	I2CStart();
	I2CSendByte(AT24C02_ADDR|0x01);
	I2CWaitAck();
	dat=I2CReceiveByte();
	I2CWaitAck();
	I2CSendAck(1);
	I2CStop();
	return dat;
}