#include "AD.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01		//光敏电阻
#define COMMAND_AIN3 0X03		//可调电阻


unsigned char AD_Read(void)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//发送从机写设备地址
	IIC_WaitAck();//从机接收后等待从机应答
	IIC_SendByte(COMMAND_AIN1);//写入控制字（该字节写入控制寄存器）
	IIC_WaitAck();//从机接收后等待从机应答
	IIC_Stop();
	
	/*因为要切换读写方向所以要重新另起一个时序*/
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS|0X01);//发送从机设备地址
	IIC_WaitAck();//从机接收后等待从机应答
	Data=IIC_RecByte();
	IIC_SendAck(1);//发送非应答
	IIC_Stop();
	return Data;	
}