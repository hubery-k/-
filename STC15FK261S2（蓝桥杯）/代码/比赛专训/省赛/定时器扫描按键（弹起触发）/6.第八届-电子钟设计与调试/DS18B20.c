#include "DS18B20.h"
#include "onewire.h"
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);
void DS18B20_ConvertT()
{
	init_ds18b20();
	Write_DS18B20(0XCC);		//����Romָ�ֻ��һ���ӻ��豸����Ҫ��Ѱ��ָ���ӻ���
	Write_DS18B20(0X44);		//��ʼת���¶�
}

float DS18B20_ReadT()
{
	unsigned char TMSB,TLSB;
	unsigned int Temp;
	float T;
	init_ds18b20();
	Write_DS18B20(0XCC);		//����Romָ�ֻ��һ���ӻ��豸����Ҫ��Ѱ��ָ���ӻ���
	Write_DS18B20(0XBE);		//��ʼ��ȡ�¶�
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	
	return T;
}