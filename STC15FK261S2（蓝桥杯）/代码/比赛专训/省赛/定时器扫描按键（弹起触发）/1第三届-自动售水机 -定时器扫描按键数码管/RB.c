#include "RB.h"

void RB_Init(void)
{
	P2&=0X1F;//����
	P0=0X00;//��P0������͵�ƽ��ͨ�����ֶٹ�ת��Ϊ�ߵ�ƽ�ߵ�ƽ�Ͽ�
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(unsigned char Open)
{
	RB_Init();
	P0|=Open;//��P0������ߵ�ƽ��ͨ�����ֶٹ�ת��Ϊ�͵�ƽ�͵�ƽ����
	P2&=0X1F;//����
}

