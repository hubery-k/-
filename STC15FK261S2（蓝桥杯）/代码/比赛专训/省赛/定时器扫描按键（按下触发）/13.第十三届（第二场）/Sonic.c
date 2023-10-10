#include "Sonic.h"
#include "Delay.h"

void SendSignal()
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

ui Sonic()
{
	ui t;
	ui distance;
	SendSignal();
	TF1=0;
	TR1=1;
	while(RX==1&&TF1==0);
	TR1=0;
	
	if(TF1==0)
	{
		t=TH1<<8|TL1;
		distance=(ui)t*0.017;
		TH1=0;
		TL1=0;
	}
	else if(TF1==1)	//��ʱ�������ʱ��Ϊ65535us���ó��ľ��볬��������ģ������������
	{
		TF1 = 0;  //��������־λ
		distance = 999;
		TH1 = 0;
		TL1 = 0;
	}
	return distance;
}