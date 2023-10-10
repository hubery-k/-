#include "PCF8591.h"


uc Read_AD(bit Channel)
{
	uc Dat;
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0X00);
	I2CWaitAck();
	if(Channel==0){I2CSendByte(AD_AIN1);}
	else{I2CSendByte(AD_AIN3);}
	I2CWaitAck();
	I2CStop();
	
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0X01);
	I2CWaitAck();
	Dat=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	
	return Dat;
}