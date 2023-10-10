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

ui Sonic_Work()
{
	ui t;
	ui distance;
	SendSignal();
	TR0=1;
	while(RX==1&&TF0==0);
	TR0=0;
	
	if(TF0==0)
	{
		t=(TH0<<8|TL0)+156;
		distance=(ui)t*0.017;
	}
	else if(TF0==1)	//��ʱ�������ʱ��Ϊ65535us���ó��ľ��볬��������ģ������������
	{
		TF0 = 0;  //��������־λ
		distance = 999;
	}
	TH0 = 0;
	TL0 = 0;
	return distance;
}