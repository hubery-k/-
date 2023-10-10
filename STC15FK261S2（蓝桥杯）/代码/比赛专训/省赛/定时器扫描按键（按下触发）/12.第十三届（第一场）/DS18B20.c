#include "DS18B20.h"
#include "onewire.h"

#define SKIP_ROM 0XCC
#define CONVERT_T 0X44
#define READ_T 0XBE

void DS18B20_ConvertT()
{
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);		//����ֻ��һ���ӻ�����ֱ������ROM����ָ��
	Write_DS18B20(CONVERT_T);		//��ʼת���¶�
}

f DS18B20_ReadT()
{
	uc TMSB,TLSB;
	ui Temp;
	f T;
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(READ_T);		//��ȡ�ݴ����е��¶����ݣ��ȶ���λ��
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	return T;
}