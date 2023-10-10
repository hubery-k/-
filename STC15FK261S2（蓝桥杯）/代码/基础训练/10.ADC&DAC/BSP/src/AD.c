#include "AD.h"
#include "iiC.h"

#define PCF8591_ADDRESS  0X90
#define COMMAND_DA 0X40		//DA-AIN0ͨ�������ű���Ƭ����û��AIN0��ADͨ����
#define COMMAND_AIN1 0X01					//AD-��������ͨ��
#define COMMAND_AIN3 0X03					//AD-�ɵ�����ͨ��

unsigned char AD_Read(void)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//���ʹӻ��豸��ַ
	IIC_WaitAck();//�ӻ����պ�ȴ��ӻ�Ӧ��
	IIC_SendByte(COMMAND_AIN3|COMMAND_DA);//д������ִ�ADת�������ֽ�д����ƼĴ�����
	IIC_WaitAck();//�ӻ����պ�ȴ��ӻ�Ӧ��
	IIC_Stop();
	
	
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS|0X01);//���ʹӻ��豸��ַ
	IIC_WaitAck();//�ӻ����պ�ȴ��ӻ�Ӧ��
	Data=IIC_RecByte();
	IIC_SendAck(1);//���ͷ�Ӧ��
	IIC_Stop();
	return Data;	
}
/*�������������������ݺ�Ӧ�򱻿��������ͷ�Ӧ��(1)��ʹ���������ͷ������ߣ��Ա�������������ֹͣ�źŶ���ֹ���ݴ���*/

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