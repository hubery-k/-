#include "AD.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01		//光敏电阻
#define COMMAND_AIN3 0X03		//可调电阻

unsigned char AD_Read(void)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//写操作地址
	IIC_WaitAck();
	IIC_SendByte(COMMAND_AIN3); //写入控制字
	IIC_WaitAck();
	IIC_Stop();  
	
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS|0X01);//读操作地址
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);//发送非应答
	IIC_Stop();
	return Data;
}
