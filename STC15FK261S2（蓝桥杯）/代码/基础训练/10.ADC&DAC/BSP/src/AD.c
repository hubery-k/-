#include "AD.h"
#include "iiC.h"

#define PCF8591_ADDRESS  0X90
#define COMMAND_DA 0X40		//DA-AIN0通道（蓝桥杯单片机上没有AIN0的AD通道）
#define COMMAND_AIN1 0X01					//AD-光敏电阻通道
#define COMMAND_AIN3 0X03					//AD-可调电阻通道

unsigned char AD_Read(void)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//发送从机设备地址
	IIC_WaitAck();//从机接收后等待从机应答
	IIC_SendByte(COMMAND_AIN3|COMMAND_DA);//写入控制字打开AD转换（该字节写入控制寄存器）
	IIC_WaitAck();//从机接收后等待从机应答
	IIC_Stop();
	
	
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS|0X01);//发送从机设备地址
	IIC_WaitAck();//从机接收后等待从机应答
	Data=IIC_RecByte();
	IIC_SendAck(1);//发送非应答
	IIC_Stop();
	return Data;	
}
/*当主控制器接受完数据后，应向被控制器发送非应答(1)，使被控制器释放数据线，以便主控制器发送停止信号而终止数据传输*/

void DA_Write(unsigned char Data)
{
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(COMMAND_AIN3|COMMAND_DA);
	IIC_WaitAck();
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_SendAck(1);
	IIC_Stop();
}