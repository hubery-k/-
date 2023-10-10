#include "DS18B20.h"
#include "onewire.h"

void DS18B20_ConvertT()
{
	init_ds18b20();
	Write_DS18B20(0XCC);		//����ROM��ֻ��һ���ӻ��豸���Բ�����Ѱ�Ҵӻ��豸��
	Write_DS18B20(0X44);		//��ʼת���¶�
}

float DS18B20_ReadT()
{
	unsigned char TMSB,TLSB;
	unsigned int Temp;
	float T;
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);	//��ȡ�ݴ����е��¶����ݣ��ȴӵ�λ��ʼ��ȡ��
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	return T;
}