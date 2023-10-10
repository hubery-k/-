#include "BUZZER.h"
#include "Ds1302_Clcok.h"
#include "Nixie.h"

//BCD��ĺô���ʱ���϶Ա�16���ƿ�������ֱ�ۣ�20������12��25��22ʱ59��30s��
unsigned char Time[]={0X30,0X59,0X23,0X25,0X12,0X07,0X20};
unsigned char i;
void main(void)
{
	BUZZER_Init();
	
	Set_Time(Time);
	
	while(1)
	{
		Read_Time(Time);
		
		Nixie_show(0,Time[2]/16);
		Nixie_show(1,Time[2]%16);
		Nixie_show(2,16);
		Nixie_show(3,Time[1]/16);
		Nixie_show(4,Time[1]%16);
		Nixie_show(5,16);
		Nixie_show(6,Time[0]/16);
		Nixie_show(7,Time[0]%16);
	}
}

