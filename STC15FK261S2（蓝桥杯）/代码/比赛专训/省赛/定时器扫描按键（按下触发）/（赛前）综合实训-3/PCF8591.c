#include "PCF8591.h"

uc PCF8591_ReadAD(bit Channel)
{
	uc Dat;
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0x00);
	I2CWaitAck();
	if(Channel==0){I2CSendByte(AD_AIN1|DA);}
	else{I2CSendByte(AD_AIN3|DA);}
	I2CWaitAck();
	I2CStop();
	
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0x01);
	I2CWaitAck();
	Dat=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	
	return Dat;
}

void PCF8591_WriteDA(uc Data)
{
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0x00);
	I2CWaitAck();
	I2CSendByte(DA);
	I2CWaitAck();
	I2CSendByte(Data);
	I2CWaitAck();
	I2CSendAck(1);
	I2CStop();
}