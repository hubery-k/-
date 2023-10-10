#include "ADC.h"
#include "iic.h"

#define ADDRESS 0X90
#define AIN1 0X01		//����ͨ��
#define AIN3 0X03		//��λ��RB1ͨ��

uc Data;

uc ADC_Read(uc AIN)
{
	IIC_Start();
	IIC_SendByte(ADDRESS);		//�����豸��ַ(д�ֽ�)
	IIC_WaitAck();
	if(AIN=='1'){IIC_SendByte(AIN1);}
	else if(AIN=='3'){IIC_SendByte(AIN3);}
			//д��Ҫ��ȡ��ADͨ��
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(ADDRESS|0X01);		//�����豸��ַ(���ֽ�)
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);		//�������ͷ�Ӧ���ʾ����
	IIC_Stop();
	return Data;
}