#include "DS18B20.h"
#include "onewire.h"

#define SKIP_ROM	0XCC
#define CONVERT_T 0X44
#define READ_T 0XBE

void DS18B20_ConvertT(void)
{
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(CONVERT_T);
}

float DS18B20_ReadT(void)
{
	f T;
	uc TLSB,TMSB;
	ui Temp;
	init_ds18b20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(READ_T);
	TLSB=Read_DS18B20();	//œ»∂¡»°µÕŒª
	TMSB=Read_DS18B20();
	Temp=(TMSB<<8)|TLSB;
	T=Temp*0.0625;
	return T;
}