#include "EEPROM.h"
#include "iic.h"

/*AT24C02�豸��ַ��һ���ֽ���ɡ�ǰ��λ�ǹ̶�ֵΪ1010������λ�ǿɱ�̲��֣����������ͼ��ȫ���ӵ�������ȫ��Ϊ0�����һλ�Ƕ�дλ��0д1��*/
#define AT24C02_ADDRESS 0XA0


void AT24C02_WriteByte(unsigned char Word_Addr,unsigned char Data)
{
	IIC_Start(); 
	IIC_SendByte(AT24C02_ADDRESS);//���ʹӻ��豸��ַ+д����������
	IIC_WaitAck();
	IIC_SendByte(Word_Addr);//��������Ҫ����ĵ�ַ
	IIC_WaitAck();
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_Stop();	
}

unsigned char AT24C02_Read(unsigned char Word_Addr)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(AT24C02_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(Word_Addr);//����λ������ȡ���ݵĵ�ַ����д��ĵ�ַ������ַָ���У��¸�ʱ������ݶ�ȡ�ͻ�Ӹõ�ַ��ʼ��
	IIC_WaitAck();
	
	IIC_Start(); //����һ��ʱ������ָ����д����
	IIC_SendByte(AT24C02_ADDRESS|0X01);//���ʹӻ��豸��ַ+������������
	IIC_WaitAck();
	Data=IIC_RecByte(); //��ȡ����
	IIC_SendAck(1);//���ͷ�Ӧ���ʾ����
	IIC_Stop();
	return Data;
}