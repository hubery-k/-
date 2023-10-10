#include "AT24C02.h"

/*注意！！！：AT24C02一个地址最大只能存取256。想要存取超过256的数可以分两个地址存储*/
void AT24C02_WriteByte(uc Addr, Data)
{
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS|0X00);
	IIC_WaitAck();
	IIC_SendByte(Addr);
	IIC_WaitAck();
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_Stop();
}


uc AT24C02_ReadByte(uc Addr)
{
	uc Data;
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS|0X00);
	IIC_WaitAck();
	IIC_SendByte(Addr);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(AT24C02_ADDRESS|0X01);		//重新指定方向为读方向
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);		//发送非应答表示结束
	IIC_Stop();

	return Data;
}