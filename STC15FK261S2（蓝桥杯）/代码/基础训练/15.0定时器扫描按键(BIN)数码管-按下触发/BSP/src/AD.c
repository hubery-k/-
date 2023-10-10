#include "AD.h"
#include "iiC.h"
unsigned char Data;
#define PCF8591_ADDRESS  0X90
#define COMMAND_AIN1 0X01					//光敏电阻
#define COMMAND_AIN3 0X03					//可调电阻

unsigned char AD_Read(void)
{
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//发送从机设备地址
	IIC_WaitAck();//从机接收后等待从机应答
	IIC_SendByte(COMMAND_AIN3);//写入控制字（该字节写入控制寄存器）
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
