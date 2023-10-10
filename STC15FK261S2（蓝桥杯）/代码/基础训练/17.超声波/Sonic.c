#include "Sonic.h"
#include "Delay.h"


void SendSignal()
{
	/*����8��40HZ�ķ�����40HZ-25us*/
	unsigned char i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay13us();	//�ߵ�ƽ��ʱ13us
		TX=0;
		Delay13us();//�͵�ƽ��ʱ13us
	}
}

unsigned int Sonic()
{
	/*�����ϰ���ص����룩=����RXΪ�ߵ�ƽʱ��ʱ�䣩ʱ��*�ٶ�/2  ʱ��ͨ����ʱ����ȡ���ٶ���340m/s*/
	/*��RX=0 - �ɹ������ź� RX=1 �����ź�ʧ�ܣ���TX=0 - ���ͳɹ� TX=1 - ����ʧ�ܣ�*/
	unsigned int t;
	unsigned int distance;
	SendSignal();	//����8����������
	TR0=1;	//��ʱ��0��ʼ��ʱ
	while(RX==1 && TF0==0);//���յ��źŻ����������ѭ������һ������������ѭ����
	TR0=0;		//�رն�ʱ��
	
	if(TF0==0)//��ʱ��û�����˵��RXΪ�͵�ƽ
	{
		t=TH0<<8|TL0;
		distance=(unsigned int)t*0.017;	
		TH0=0;
		TL0=0;
	}
	else if(TF0==1)	//��ʱ�������ʱ��Ϊ65535us���ó��ľ��볬��������ģ������������
	{
		TF0 = 0;  //��������־λ
		distance = 999;
		TH0 = 0;
		TL0 = 0;
	}
	return distance;
}