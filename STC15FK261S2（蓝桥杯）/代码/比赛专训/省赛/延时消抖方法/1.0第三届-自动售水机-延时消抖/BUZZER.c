#include "BUZZER.h"

void BUZZER_Init(void)
{
	P2&=0X1F;//����
	
	P0&=~0X40;//P0������͵�ƽͨ�����ֶٹ�ת��Ϊ�ߵ�ƽ���ߵ�ƽ�رգ�
	P2=(P2&0X1F)|0XA0;
}

void BUZZER_Open(void)
{
	BUZZER_Init();
	
	P0|=0X40;//P0������ߵ�ƽͨ�����ֶٹ�ת��Ϊ�͵�ƽ���͵�ƽ���죩
	//P2=(P2&0X1F)|0XA0;
}

void BUZZER_Close(void)
{
	P2&=0X1F;//����
	
	P0&=~0X40;//P0������͵�ƽͨ�����ֶٹ�ת��Ϊ�ߵ�ƽ���ߵ�ƽ�رգ�
	P2=(P2&0X1F)|0XA0;
}