#include "CLOCK.h"

void DS1302_WriteTime(uc *Time)
{
	Write_Ds1302_Byte(0X8E,~0X80);//�ر�д����
	/*д��ʱ��*/
	Write_Ds1302_Byte(0X80,Time[0]);
	Write_Ds1302_Byte(0X82,Time[1]);
	Write_Ds1302_Byte(0X84,Time[2]);
	
	Write_Ds1302_Byte(0X8E,0X80);//��д����
}

void DS1302_ReadTime(uc *Time)
{
	Time[0]=Read_Ds1302_Byte (0X81);
	Time[1]=Read_Ds1302_Byte (0X83);
	Time[2]=Read_Ds1302_Byte (0X85);
}