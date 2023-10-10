#include "DS18B20.h"
#include "onewire.h"

void DS18B20_ConvertT(void)
{
	init_ds18b20();
	Write_DS18B20(0XCC);//����ROMָ�����ֻ��һ���豸������Ѱ���豸��ַ�ˣ�
	Write_DS18B20(0X44);//����ָ�ʼת���¶�
}

float DS18B20_ReadT(void)
{
	unsigned char TMSB,TLSB;
	int Temp;
	float T;
	
	init_ds18b20();
	Write_DS18B20(0XCC);//����ROMָ�����ֻ��һ���豸������Ѱ���豸��ַ�ˣ�
	Write_DS18B20(0XBE);//���Ϳ�ʼ��ȡ�¶�ָ��
	TLSB=Read_DS18B20();//�ȶ�ȡ�Ͱ�λ����
	TMSB=Read_DS18B20();//�ٶ�ȡ�߰�λ����
	Temp=(TMSB<<8)|TLSB;//������������һ��
	T=Temp*0.0625;//���Ծ�����ֵ0.0625��������ת�����¶�
	
	return T;
}