#include "onewire.h"
#include "DS18B20.h"

#define SKIP_ROM 0XCC
#define CONVERT_T 0X44
#define READ_T 0XBE 

void Convert_T()
{
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(CONVERT_T);
}

float Read_T()
{
	unsigned char TMSB,TLSB;
	unsigned int Temp;
	float T;
	
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(READ_T);
	TLSB=Read_DS18B20();
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	return T;
}