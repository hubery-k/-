#include "DA.h"
#include "iic.h"

#define ADDRESS 0X90
  

void DAC_Write(unsigned char Data)
{
	IIC_Start();
	IIC_SendByte(ADDRESS);		//设备地址加写控制
	IIC_WaitAck();
	IIC_SendByte(0X40);		//选择DA转换且打开了AIN0（AD的0通道。由于使用的是DAC功能所以开哪个AD通道都无所谓）
	IIC_WaitAck();
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_SendAck(1);	//主机发送非应答表示结束
	IIC_Stop();
}
