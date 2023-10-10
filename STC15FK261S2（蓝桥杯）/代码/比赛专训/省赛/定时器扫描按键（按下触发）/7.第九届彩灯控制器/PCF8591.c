#include "PCF8591.h"

uc PCF8591_ReadAD()
{
	uc Dat;
	IIC_Start();
	IIC_SendByte(PCF8591_ADDRESS|0X00);
	IIC_WaitAck();
	IIC_SendByte(COMMAND_AIN3);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(PCF8591_ADDRESS|0x01);		//重新指定读写方向
	IIC_WaitAck();
	Dat=IIC_RecByte();
	IIC_SendAck(1);		//发送非应答主机表示结束
	IIC_Stop();
	return Dat;
}