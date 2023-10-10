#include "ds1302.h"

/*十进制转BCD码：(BCD)=(DEC)/10*16+DEC%10*/
/*BCD码转十进制：(DEC)=(BCD)/16*10+BCD%16*/
unsigned char code Write_Address[]={0X80,0X82,0X84,0X86,0X88,0X8A,0X8C,0X8E};
unsigned char code Read_Address[]={0X81,0X83,0X85,0X87,0X89,0X8B,0X8D,0X8F};

void Set_Time(unsigned char *Time)
{
	unsigned char i;
	Write_Ds1302_Byte(0X8E,~0X80 );//清除写保护位
	for(i=0;i<7;i++)
	{
		Write_Ds1302_Byte(Write_Address[i],Time[i]);//写入时间
	}
	Write_Ds1302_Byte(0X8E,0X80 );//打开写保护
}

void Read_Time(unsigned char *Time)
{
	unsigned char i;
	for(i=0;i<7;i++)
	{
		Time[i]=Read_Ds1302_Byte(Read_Address[i]);
	}
}