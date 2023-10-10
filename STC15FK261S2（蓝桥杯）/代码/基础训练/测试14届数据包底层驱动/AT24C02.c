#include "AT24C02.h"
#include "iic.h"

#define AT24C02_ADDRESS 0XA0

void AT24C02_Write(unsigned char Addr,Dat)
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

unsigned char AT24C02_Read(unsigned char Addr)
{
	unsigned char Dat;
	I2CStart();
	I2CSendByte(AT24C02_ADDRESS);
	I2CWaitAck();
	I2CSendByte(Addr);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(AT24C02_ADDRESS|0X01);
	I2CWaitAck();
	Dat=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	return Dat;
}
