#include "Sonic.h"

sbit TX=P1^0;
sbit RX=P1^1;

void Sonic_Init(void)		//0΢��@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0x00;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1�ȴ���ʼ��ʱ
}


void Sonic_SendWave()
{
	uc i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay12us();
		TX=0;
		Delay12us();
	}
}

ui Sonic_Work()
{
	ui Distance;
	ui t;
	
	Sonic_SendWave();
	TR1=1;
	while(TF1==0&&RX==1);
	TR1=0;
	
	if(TF1==0)
	{
		t=(TH1<<8)|TL1;
		Distance=t*0.017;
	}
	else
	{
		TF1=0;
		Distance=999;
	}
	TH1=0;
	TL1=0;
	
	return Distance;
}