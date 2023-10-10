#include "PCF8591.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define AD_AIN1 0X01		//光敏通道
#define AD_AIN3 0X03		//RB2电位器通道

uc AD_Read()
{
	uc Data;
	IIC_Start();
	IIC_SendByte(PCF8591_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(AD_AIN3);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(PCF8591_ADDRESS|0X01);
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);		//主机发送非应答表示结束
	IIC_Stop();
	return Data;
}	