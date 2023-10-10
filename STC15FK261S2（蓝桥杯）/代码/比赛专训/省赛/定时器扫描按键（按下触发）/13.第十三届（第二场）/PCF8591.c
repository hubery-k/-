#include "PCF8591.h"
#include "iic.h"
#include "Delay.h"

#define PCF8591_ADDRESS 0X90
#define AD_AIN1 0X01		//光敏电阻通道
#define AD_AIN3 0X03		//电位器RB2通道
#define DA 0X40

uc AD_Read()
{
	uc Data;
	
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS);
	I2CWaitAck();
	I2CSendByte(AD_AIN3|DA);
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

void DA_Write(uc Data)
{
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS);
	I2CWaitAck();
	I2CSendByte(DA);
	I2CWaitAck();
	I2CSendByte(Data);
	I2CWaitAck();
	I2CSendAck(1);
	I2CStop();
}