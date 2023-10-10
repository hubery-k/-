#include <STC15F2K60S2.H>
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
	//����Ҫ�����ж�
}

/*����8����������*/
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

	/*�����ϰ���ص����룩=����RXΪ�ߵ�ƽʱ��ʱ�䣩ʱ��*�ٶ�/2  ʱ��ͨ����ʱ����ȡ���ٶ���340m/s*/
	/*��RX=0 - �ɹ������ź� RX=1 �����ź�ʧ�ܣ���TX=0 - ���ͳɹ� TX=1 - ����ʧ�ܣ�*/
ui Sonic_Work()
{
	ui Distance;
	ui t;
	
	Sonic_SendWave();
	TR1=1;
	while(RX==1&&TF1==0);//���յ��źŻ����������ѭ������һ������������ѭ����
	TR1=0;
	if(TF1==0)
	{
		t=(TH1<<8)|TL1;
		Distance=t*0.017;
	}
	else if(TF1==1)
	{
		TF1=0;
		Distance=999;
	}
	TH1=0;
	TL1=0;
	return Distance;
}

