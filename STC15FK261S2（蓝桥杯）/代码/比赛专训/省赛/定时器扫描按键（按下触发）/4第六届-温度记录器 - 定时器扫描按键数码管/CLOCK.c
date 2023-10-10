#include "CLOCK.h"  		
#include "ds1302.h"  

unsigned char code Write_address[]={0X80,0X82,0X84,0X86,0X88,0X8A,0X8C,0X8E};
unsigned char code Read_address[]={0X81,0X83,0X85,0X87,0X89,0X8B,0X8D,0X8F};

void Write_time(unsigned char *Time)
{
	unsigned char i;
	Write_Ds1302_Byte(0X8E,0X00);//关闭写保护（bit7置0关闭写保护写入数据有效）
	for(i=0;i<3;i++)		//只需写入秒、时、分三个数据
	{
		Write_Ds1302_Byte(Write_address[i],Time[i]);
	}
	Write_Ds1302_Byte(0X8E,0X80);//打开写保护（bit7置1打开写保护写入数据无效）
}

void Read_time(unsigned char *Time)
{
	unsigned char i;
	for(i=0;i<3;i++)		//只需读取秒、时、分三个数据
	{
		Time[i]=Read_Ds1302_Byte(Read_address[i]);
	}
}