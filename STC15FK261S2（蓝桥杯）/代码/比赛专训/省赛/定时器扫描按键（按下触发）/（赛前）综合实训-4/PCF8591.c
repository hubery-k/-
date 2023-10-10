#include "PCF8591.h"

uc PCF8591_ReadAD(bit Channel)
{
	uc Data;
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0X00);
	I2CWaitAck();
	if(Channel==0){I2CSendByte(AD_AIN1|DA);}
	else{I2CSendByte(AD_AIN3|DA);}
	I2CWaitAck();
	I2CStop();
	
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0X01);
	I2CWaitAck();
	Data=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	
	return Data;
}

void PCF8591_WriteDA(uc Dat)
{
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0X00);
	I2CWaitAck();
	I2CSendByte(AD_AIN3|DA);
	I2CWaitAck();
	I2CSendByte(Dat);
	I2CWaitAck();
	I2CSendAck(1);
	I2CStop();
}