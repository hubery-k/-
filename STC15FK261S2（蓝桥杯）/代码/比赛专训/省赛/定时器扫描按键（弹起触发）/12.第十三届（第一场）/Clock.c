#include "Clock.h"
#include "ds1302.h"  		

uc code R_Addr[3]={0X81,0X83,0X85};
uc code W_Addr[3]={0X80,0X82,0X84};


void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

void Write_Time(uc* Time)
{
	uc i=0;
	Write_Ds1302_Byte(0x8E,0x00);//打开写保护
	
	for(i=0;i<3;i++)
	{
		Write_Ds1302_Byte(W_Addr[i],Time[i]);
	}
	
	Write_Ds1302_Byte(0x8E,0x80);//打开写保护//关闭写保护
}

void Read_Time(uc* Time)
{
	uc i=0;
	for(i=0;i<3;i++)
	{
		Time[i]=Read_Ds1302_Byte(R_Addr[i]);
	}
}
