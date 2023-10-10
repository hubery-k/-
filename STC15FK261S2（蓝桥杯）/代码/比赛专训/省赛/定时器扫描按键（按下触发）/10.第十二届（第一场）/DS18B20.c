#include "DS18B20.h"
#include "onewire.h"

void DS18B20_ConvertT()
{
	init_ds18b20();
	Write_DS18B20(0XCC);		//跳过ROM（只有一个从机设备所以不用再寻找从机设备）
	Write_DS18B20(0X44);		//开始转换温度
}

float DS18B20_ReadT()
{
	unsigned char TMSB,TLSB;
	unsigned int Temp;
	float T;
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);	//读取暂存器中的温度数据（先从低位开始读取）
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	return T;
}