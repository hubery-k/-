#include "DA.h"
#include "iic.h"

#define ADDRESS 0X90
  

void DAC_Write(unsigned char Data)
{
	IIC_Start();
	IIC_SendByte(ADDRESS);		//�豸��ַ��д����
	IIC_WaitAck();
	IIC_SendByte(0X40);		//ѡ��DAת���Ҵ���AIN0��AD��0ͨ��������ʹ�õ���DAC�������Կ��ĸ�ADͨ��������ν��
	IIC_WaitAck();
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_SendAck(1);	//�������ͷ�Ӧ���ʾ����
	IIC_Stop();
}
