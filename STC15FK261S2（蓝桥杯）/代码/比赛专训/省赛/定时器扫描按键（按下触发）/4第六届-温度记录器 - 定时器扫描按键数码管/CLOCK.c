#include "CLOCK.h"  		
#include "ds1302.h"  

unsigned char code Write_address[]={0X80,0X82,0X84,0X86,0X88,0X8A,0X8C,0X8E};
unsigned char code Read_address[]={0X81,0X83,0X85,0X87,0X89,0X8B,0X8D,0X8F};

void Write_time(unsigned char *Time)
{
	unsigned char i;
	Write_Ds1302_Byte(0X8E,0X00);//�ر�д������bit7��0�ر�д����д��������Ч��
	for(i=0;i<3;i++)		//ֻ��д���롢ʱ������������
	{
		Write_Ds1302_Byte(Write_address[i],Time[i]);
	}
	Write_Ds1302_Byte(0X8E,0X80);//��д������bit7��1��д����д��������Ч��
}

void Read_time(unsigned char *Time)
{
	unsigned char i;
	for(i=0;i<3;i++)		//ֻ���ȡ�롢ʱ������������
	{
		Time[i]=Read_Ds1302_Byte(Read_address[i]);
	}
}