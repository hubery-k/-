#include "BUZZER.h"


void BUZZER_Init(void)
{
	P2&=0X1F;//����
	P0&=~0X40;
	P2=(P2&0X1F)|0XA0;
}
void BUZZER_Open(void)
{
	BUZZER_Init();
	P0|=0X40;//(P0������ߵ�ƽ)�������ֶٹ�ת��Ϊ�͵�ƽ����
	P2=(P2&0X1F)|0XA0;
	P2&=0X1F;//����
}

void BUZZER_Close(void)
{
	BUZZER_Init();
	P0&=~0X40;//(P0������͵�ƽ)�������ֶٹ�ת��Ϊ�ߵ�ƽ�ر�
	P2=(P2&0X1F)|0XA0;
	P2&=0X1F;//����
}