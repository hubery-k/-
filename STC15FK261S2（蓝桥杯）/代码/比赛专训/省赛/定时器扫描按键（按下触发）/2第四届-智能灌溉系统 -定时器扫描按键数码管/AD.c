#include "AD.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01		//��������
#define COMMAND_AIN3 0X03		//�ɵ�����

unsigned char AD_Read(void)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//д������ַ
	IIC_WaitAck();
	IIC_SendByte(COMMAND_AIN3); //д�������
	IIC_WaitAck();
	IIC_Stop();  
	
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS|0X01);//��������ַ
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);//���ͷ�Ӧ��
	IIC_Stop();
	return Data;
}
