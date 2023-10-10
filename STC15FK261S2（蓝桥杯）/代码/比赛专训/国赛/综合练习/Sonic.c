#include "Sonic.h"

sbit TX=P1^0;
sbit RX=P1^1;

void Soinc_Init()
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x00;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0�ȴ���ʼ��ʱ
	//����Ҫ�����ж�
}

void Sonic_SendWave()
{
	uc i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay13us();
		TX=0;
		Delay13us();
	}
}

ui Sonic_Work()
{
	ui Distance;
	ui t;
	
	Sonic_SendWave();
	TR0=1;
	while(RX==1&&TF0==0);//���յ��źŻ����������ѭ������һ������������ѭ����
	TR0=0;
	if(TF0==0)
	{
		t=(TH0<<8)|TL0;
		Distance=t*0.017;
	}
	else if(TF0==1)
	{
		TF0=0;
		Distance=999;
	}
	TH0=0;
	TL0=0;
	return Distance;
}