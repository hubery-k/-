#include "DS18B20.h"
#include "onewire.h"


void DS18B20_Convert(void)
{
	init_ds18b20();
	Write_DS18B20(0XCC);//����ֻ��һ���ӻ��豸�����Կ�������ROMָ��豸��ַ����ROM�У������豸��ַ��
	Write_DS18B20(0X44);//��ʼ�¶�ת��������ָ����ݴ������в�����
}
float DS18B20_ReadT(void)
{
	unsigned char TMSB,TLSB;
	int Temp;
	float T;
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);//��ȡ�ݴ����е��¶�����
	TLSB=Read_DS18B20();//�ȶ�ȡ��λ����
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;//�Զ�����ת��������Temp��16λ�ģ�TMSB��TLSB��Temp����ʱ���Զ�ת��λint���ʹ�С�ˣ�
	T=Temp*0.0625;//���Ծ�����ֵ���ܵ����¶���ֵ��
	return T;
}