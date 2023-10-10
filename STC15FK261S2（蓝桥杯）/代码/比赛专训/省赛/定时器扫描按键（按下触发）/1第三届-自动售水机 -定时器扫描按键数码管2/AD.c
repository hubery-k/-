#include "AD.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01		//��������
#define COMMAND_AIN3 0X03		//�ɵ�����


unsigned char AD_Read(void)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS);//���ʹӻ�д�豸��ַ
	IIC_WaitAck();//�ӻ����պ�ȴ��ӻ�Ӧ��
	IIC_SendByte(COMMAND_AIN1);//д������֣����ֽ�д����ƼĴ�����
	IIC_WaitAck();//�ӻ����պ�ȴ��ӻ�Ӧ��
	IIC_Stop();
	
	/*��ΪҪ�л���д��������Ҫ��������һ��ʱ��*/
	IIC_Start(); 
	IIC_SendByte(PCF8591_ADDRESS|0X01);//���ʹӻ��豸��ַ
	IIC_WaitAck();//�ӻ����պ�ȴ��ӻ�Ӧ��
	Data=IIC_RecByte();
	IIC_SendAck(1);//���ͷ�Ӧ��
	IIC_Stop();
	return Data;	
}