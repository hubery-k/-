#include "DS18B20.h"

#define SKIP_ROM 0XCC
#define T_CONVERT 0X44
#define T_READ 0XBE

void T_Convert()
{
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);//跳过寻找需要通信的设备，因为就这一个设备
	Write_DS18B20(T_CONVERT);
}
f T_Read()
{
	uc TLSB,TMSB;
	ui Temp;
	f T;
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(T_READ);
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	return T;
}