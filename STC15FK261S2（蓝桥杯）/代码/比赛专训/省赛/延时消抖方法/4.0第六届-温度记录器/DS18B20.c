#include "DS18B20.h"
#include "onewire.h"

void DS18B20_ConvertT(void)
{
	init_ds18b20();
	Write_DS18B20(0XCC);//跳过ROM指令（由于只有一个设备不用再寻找设备地址了）
	Write_DS18B20(0X44);//发出指令开始转换温度
}

float DS18B20_ReadT(void)
{
	unsigned char TMSB,TLSB;
	int Temp;
	float T;
	
	init_ds18b20();
	Write_DS18B20(0XCC);//跳过ROM指令（由于只有一个设备不用再寻找设备地址了）
	Write_DS18B20(0XBE);//发送开始读取温度指令
	TLSB=Read_DS18B20();//先读取低八位数据
	TMSB=Read_DS18B20();//再读取高八位数据
	Temp=(TMSB<<8)|TLSB;//将数据整合在一起
	T=Temp*0.0625;//乘以精度数值0.0625，将数据转换成温度
	
	return T;
}