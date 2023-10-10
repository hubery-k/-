#include "Sonic.h"
#include "Delay.h"

/*T1������������ʱ*/
void Sonic_Init(void)		//0΢��@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0x00;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1�ȴ���ʼ��ʱ
}

void Send_Wave()
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
	ui t;
	ui Distance;
	
	Send_Wave();
	TR1=1;
	while(RX==1&&TF1==0);	//��һ���������������˳�ѭ��
	TR1=0;
	if(TF1==0)
	{
		t=(TH1<<8|TL1);
		Distance=(ui)t*0.017;
		/*�������ʱ��*�ٶȣ��ٶ�Ϊ340m/s -> 340*10^3(cm)/1*10^6(us)=0.034cm/us������Ϊ��ʱ��ʱ����һ�����ص�ʱ�������ٳ���2*/
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

