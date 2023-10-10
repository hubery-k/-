#include "DS18B20.h"
#include "onewire.h"

void DS18B20_ConvertT(void)
{
	init_ds18b20();
	Write_DS18B20(0XCC);		//跳过ROM指令（只有一个设备就不用再寻找从机序列号了）
	Write_DS18B20(0X44);		//开始转换温度
}
float DS18B20_ReadT(void)
{
	unsigned char TMSB,TLSB;
	int Temp;
	float T;
	
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);
	TLSB=Read_DS18B20();		//先读取低位字节
	TMSB=Read_DS18B20();		//再读取高位字节
	Temp=(TMSB<<8)|TLSB;		//自动强制类型转换
	T=Temp*0.0625;		//乘以精度数据转换为温度数据
	return T;
}