#include "RELAY.h"

void RELAY_Init(void)
{
	P2&=0X1F;//����
	
	P0&=~0X10;//��P0������͵�ƽ��ͨ�����ֶٹ�ת��Ϊ�ߵ�ƽ�ߵ�ƽ�Ͽ�
	P2=(P2&0X1F)|0XA0;
}

void RELAY_Open(void)
{
	RELAY_Init();
	
	P0|=0X10;//P0������ߵ�ƽͨ�����ֶٹ�ת��Ϊ�͵�ƽ���͵�ƽ���ϣ�
	P2=(P2&0X1F)|0XA0;
}

void RELAY_Close(void)
{
	P2&=0X1F;//����
	
	P0&=~0X10;//P0������͵�ƽͨ�����ֶٹ�ת��Ϊ�ߵ�ƽ���ߵ�ƽ�Ͽ���
	P2=(P2&0X1F)|0XA0;
	
}