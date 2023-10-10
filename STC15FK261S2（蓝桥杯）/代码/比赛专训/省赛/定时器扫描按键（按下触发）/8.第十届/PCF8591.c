#include "PCF8591.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01		//����
#define COMMAND_AIN3 0X03		//��λ��RB2
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
	IIC_SendAck(1);		//�������ͷ�Ӧ���ʾ����
	IIC_Stop();
	return Data;
}

void DA_Write(uc Data)
{
	IIC_Start();
	IIC_SendByte(PCF8591_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(COMMAND_AIN3|COMMAND_DA);		//ѡ��DAת���Ҵ���AIN0��AD��0ͨ��������ʹ�õ���DAC�������Կ��ĸ�ADͨ��������ν��
	IIC_WaitAck();
	IIC_SendByte(Data);		//д����ֵ
	IIC_WaitAck();
	IIC_SendAck(1);		//�������ͷ�Ӧ���ʾ����
	IIC_Stop();
}

