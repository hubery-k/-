#include "RB.h"
/*���������̵���-P0������ߵ�ƽ�������ֶٹܷ�תΪ�͵�ƽ���ϡ����졣*/

void RB_Init(void)
{
	P2&=0X1F;		//����
	P0=0X00;		//�رշ��������̵���
	P2=(P2&0X1F)|0XA0;
}

void RB_Control(uc Open)
{
	RB_Init();
	P0|=Open;
	P2&=0X1F;
}