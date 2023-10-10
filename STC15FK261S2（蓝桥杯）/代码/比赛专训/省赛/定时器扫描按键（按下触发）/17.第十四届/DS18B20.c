#include "DS18B20.h"

void DS18B20_ConvertT()
{
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(CONVERT_T);
}

f DS18B20_ReadT()
{
	uc TLSB,TMSB;
	ui Temp;
	f T;
	
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(READ_T);
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	return T;
}