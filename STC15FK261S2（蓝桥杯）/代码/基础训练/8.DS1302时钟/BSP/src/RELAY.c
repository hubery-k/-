#include "RELAY.h"
#include "Delay.h"

void RELAY_Init()
{
	P2=(P2&0x1f)|0xA0;
	P0&=~0X10;
}

void RELAY_ON(void)
{
	RELAY_Init();
	P0|=0x10;//����ߵ�ƽͨ�����ֶٹ�����͵�ƽ�̵�������
	Delay500ms();
	P0&=~0X10;//�ر�
}
