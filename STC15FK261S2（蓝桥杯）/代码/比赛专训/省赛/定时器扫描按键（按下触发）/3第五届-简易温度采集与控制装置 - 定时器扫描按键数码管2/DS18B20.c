#include "DS18B20.h"
#include "onewire.h"


void DS18B20_Convert(void)
{
	init_ds18b20();
	Write_DS18B20(0XCC);//由于只有一个从机设备，所以可以跳过ROM指令（设备地址存在ROM中，查找设备地址）
	Write_DS18B20(0X44);//开始温度转换（功能指令对暂存器进行操作）
}
float DS18B20_ReadT(void)
{
	unsigned char TMSB,TLSB;
	int Temp;
	float T;
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);//读取暂存器中的温度数据
	TLSB=Read_DS18B20();//先读取低位数据
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;//自动类型转换（由于Temp是16位的，TMSB和TLSB与Temp运算时，自动转换位int类型大小了）
	T=Temp*0.0625;//乘以精度数值才能等于温度数值。
	return T;
}