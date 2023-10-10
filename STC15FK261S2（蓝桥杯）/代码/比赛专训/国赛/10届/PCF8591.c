#include "PCF8591.h"

#define PCF8591_ADDR 0X90
#define DA 0X40

void PCF8591_WriteDA(uc Dat)
{
	I2CStart();
	I2CSendByte(PCF8591_ADDR|0X00);
	I2CWaitAck();
	I2CSendByte(DA);
	I2CWaitAck();
	I2CSendByte(Dat);
	I2CWaitAck();
	I2CSendAck(1);
	I2CStop();
}
