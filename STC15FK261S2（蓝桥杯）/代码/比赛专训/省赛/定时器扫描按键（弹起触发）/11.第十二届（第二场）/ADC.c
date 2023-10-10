#include "ADC.h"
#include "iic.h"

#define ADDRESS 0X90
#define AIN1 0X01		//光敏通道
#define AIN3 0X03		//电位器RB1通道

uc Data;

uc ADC_Read(uc AIN)
{
	IIC_Start();
	IIC_SendByte(ADDRESS);		//发送设备地址(写字节)
	IIC_WaitAck();
	if(AIN=='1'){IIC_SendByte(AIN1);}
	else if(AIN=='3'){IIC_SendByte(AIN3);}
			//写入要读取的AD通道
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(ADDRESS|0X01);		//发送设备地址(读字节)
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);		//主机发送非应答表示结束
	IIC_Stop();
	return Data;
}