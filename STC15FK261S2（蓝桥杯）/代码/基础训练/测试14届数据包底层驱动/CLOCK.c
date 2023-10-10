#include "CLOCK.h"
#include "ds1302.h"  

void Set_Time(unsigned char* Time)
{
	Write_Ds1302_Byte(0X8E,0X00);		//关闭写保护
	
	Write_Ds1302_Byte(0X80,Time[0]);
	Write_Ds1302_Byte(0X82,Time[1]);
	Write_Ds1302_Byte(0X84,Time[2]);
	
	Write_Ds1302_Byte(0X8E,0X80);		//打开写保护
}

void Read_Time(unsigned char* Time)
{
	Time[0]=Read_Ds1302_Byte(0X81);
	Time[1]=Read_Ds1302_Byte(0X83);
	Time[2]=Read_Ds1302_Byte(0X85);
}