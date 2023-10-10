#include "onewire.h"

void DS18B20_ConvertT(void)
{
	init_ds18b20();
	Write_DS18B20(0XCC);//����ֻ��һ���ӻ��豸�����Կ�������ROMָ�ROMָ���64λROM���������ý����豸Ѱַ�ˡ�
	Write_DS18B20(0X44);//��ʼת���¶ȣ�����ָ����ݴ������в�����
}

float DS18B20_ReadT(void)
{
	unsigned char TMSB,TLSB;
	int Temp;
	float T;
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);//��ȡ�ݴ����е��¶�����
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;//�Զ�����ת��������Temp��16λ�ģ�TMSB��TLSB��Temp����ʱ���Զ�ת��Ϊint���ʹ�С�ˣ�
	T=Temp*0.0625;
	return T;
}