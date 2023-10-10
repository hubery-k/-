#include "AT24C02.h"

/*ע�⣡������AT24C02һ����ַ���ֻ�ܴ�ȡ256����Ҫ��ȡ����256�������Է�������ַ�洢*/
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
	IIC_SendByte(AT24C02_ADDRESS|0X01);		//����ָ������Ϊ������
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);		//���ͷ�Ӧ���ʾ����
	IIC_Stop();

	return Data;
}