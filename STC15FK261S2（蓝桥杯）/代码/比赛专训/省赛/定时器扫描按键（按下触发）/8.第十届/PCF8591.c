#include "PCF8591.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01		//光敏
#define COMMAND_AIN3 0X03		//电位器RB2
#define COMMAND_DA 0X40

uc AD_Read()
{
	uc Data;
	IIC_Start();
	IIC_SendByte(PCF8591_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(COMMAND_AIN3);
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

void DA_Write(uc Data)
{
	IIC_Start();
	IIC_SendByte(PCF8591_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(COMMAND_AIN3|COMMAND_DA);		//选择DA转换且打开了AIN0（AD的0通道。由于使用的是DAC功能所以开哪个AD通道都无所谓）
	IIC_WaitAck();
	IIC_SendByte(Data);		//写入数值
	IIC_WaitAck();
	IIC_SendAck(1);		//主机发送非应答表示结束
	IIC_Stop();
}

