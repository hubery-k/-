#include "RB.h"
#include "Define.h"

/*���������̵���-P0������ߵ�ƽ�������ֶٹ�ת��Ϊ�ߵ�ƽ���ϡ����죬�͵�ƽ�Ͽ������졣*/
void RB_Init(void)
{
	P2&=0X1F;
	P0=0X00;
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(uc Open)
{
	RB_Init();
	P0|=Open;		//P0����ߵ�ƽ���ϡ�����
	P2&=0X1F;
}