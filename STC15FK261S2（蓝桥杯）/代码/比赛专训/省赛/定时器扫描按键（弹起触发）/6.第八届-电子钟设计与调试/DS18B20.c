#include "DS18B20.h"
#include "onewire.h"
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);
void DS18B20_ConvertT()
{
	init_ds18b20();
	Write_DS18B20(0XCC);		//跳过Rom指令（只有一个从机设备不需要再寻找指定从机）
	Write_DS18B20(0X44);		//开始转换温度
}

float DS18B20_ReadT()
{
	unsigned char TMSB,TLSB;
	unsigned int Temp;
	float T;
	init_ds18b20();
	Write_DS18B20(0XCC);		//跳过Rom指令（只有一个从机设备不需要再寻找指定从机）
	Write_DS18B20(0XBE);		//开始读取温度
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	
	return T;
}