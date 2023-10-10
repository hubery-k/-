#include "AD.h"
#include "iiC.h"
unsigned char Data;
#define PCF8591_ADDRESS  0X90
#define COMMAND_AIN1 0X01					//��������
#define COMMAND_AIN3 0X03					//�ɵ�����

unsigned char AD_Read(void)
{
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//���ʹӻ��豸��ַ
	IIC_WaitAck();//�ӻ����պ�ȴ��ӻ�Ӧ��
	IIC_SendByte(COMMAND_AIN3);//д������֣����ֽ�д����ƼĴ�����
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
