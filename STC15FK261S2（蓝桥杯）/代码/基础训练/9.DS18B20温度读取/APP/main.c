#include "BUZZER.h"
#include "DS18B20.h"
#include "Nixie.h"
#include "Delay.h"
#include "UART.h"
#include "stdio.h"

unsigned char code SMG_donot[]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
unsigned char code SMG_not[]=		
{0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

float T;
char T_integer;
unsigned int T_decimals;
void main(void)
{
	BUZZER_Init();
	UART_Init();
	DS18B20_ConvertT();
	Delay1000ms();//等待温度转换完成
	
	
	while(1)
	{
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		printf("%f C\r\n",T);
		
		if(T>=0)//正温度
		{
			T_integer=T;
			T_decimals=(int)(T*100)%100;//保留小数点后两位
			if(T>=0&&T<10)
			{
				Nixie_show(0,SMG_not[T_integer]);
				Nixie_show(1,SMG_donot[T_decimals/10]);
				Nixie_show(2,SMG_donot[T_decimals%10]);
			}
			else
			{
				Nixie_show(0,SMG_donot[T_integer/10]);
				Nixie_show(1,SMG_not[T_integer%10]);
				Nixie_show(2,SMG_donot[T_decimals/10]);
				Nixie_show(3,SMG_donot[T_decimals%10]);
			}
		}
		else//负温度
		{
			T_integer=-T;
			T_decimals=(int)(-T*100)%100;//保留小数点后两位
			Nixie_show(0,SMG_donot[16]);
			if(T>-10&&T<0)
			{
				Nixie_show(1,SMG_not[T_integer]);
				Nixie_show(2,SMG_donot[T_decimals/10]);
				Nixie_show(3,SMG_donot[T_decimals%10]);
			}
			else
			{
				Nixie_show(1,SMG_donot[T_integer/10]);
				Nixie_show(2,SMG_not[T_integer%10]);
				Nixie_show(3,SMG_donot[T_decimals/10]);
				Nixie_show(4,SMG_donot[T_decimals%10]);
			}
		}
	}
}

