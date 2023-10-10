#include "BUZZER.h"
#include "Nixie.h"
#include "AD.h"
#include "UART.h"
#include "stdio.h"
#include "Timer.h"

unsigned char code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
unsigned char code dot_SMG[]={~0xBF,~0x86,~0xDB,~0xCF,~0xE6,~0xED,~0xFD,~0x87,~0xFF,~0xEF,~0xF7,~0xFC,~0xB9,~0xDE,~0xF9,~0xF1};
float adc_vol;
bit AD_flag;

void USE_PCF8591()
{
	/*AD数据采集*/
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_vol=AD_Read()*(5.0/255);//采集到的AD值量化转换为电压
	}
	
	
	/*DA输出*/
	DA_Write(255);
}
void USE_SMG()
{	
	Nixie_SetBuf(5,dot_SMG[(unsigned int)(adc_vol*100)/100]);
	Nixie_SetBuf(6,Nodot_SMG[(unsigned int)(adc_vol*100)/10%10]);
	Nixie_SetBuf(7,Nodot_SMG[(unsigned int)(adc_vol*100)%10]);
}


void main(void)
{
	BUZZER_Init();
//	UART_Init();
	Timer0Init();	
	while(1)
	{
		USE_PCF8591();
		USE_SMG();
		//printf("%f V\r\n",adc_vol);
	}
}
void Timer0_Routine(void) interrupt 1
{
	static unsigned char Count1;
	static unsigned char AD_Count;
	Count1++;
	if(Count1==2)
	{
		Count1=0;
		Nixie_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)
	{
		AD_Count=0;
		AD_flag=1;
	}
	
}

