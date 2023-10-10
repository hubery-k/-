#include "PCF8591.h"

#define PCF8591_ADDRESS 0X90
#define AD_AIN1 0X01		//光敏
#define AD_AIN3 0X03		//RB2

uc AD_Read()
{
	uc Dat;
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS);
	I2CWaitAck();
	I2CSendByte(AD_AIN3);
	I2CWaitAck();
	I2CStop();
	
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0X01);
	I2CWaitAck();
	Dat=I2CReceiveByte();
	I2CSendAck(1);	//主机发送非应答表示结束
	I2CStop();
	
	return Dat;
}