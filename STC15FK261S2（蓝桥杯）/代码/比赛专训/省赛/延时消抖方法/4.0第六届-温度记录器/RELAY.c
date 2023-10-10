#include "RELAY.h"


/*P0����ߵ�ƽͨ�����ֶٹ�ת��Ϊ�͵�ƽʹ�̵�������*/
void RELAY_Init(void)
{
	P2&=0X1F;//����
	P0&=~0X10;
	P2=(P2&0X1F)|0XA0;
}
void RELAY_Open(void)
{
	RELAY_Init();
	P0|=0X10;
	P2=(P2&0X1F)|0XA0;
}

void RELAY_Close(void)
{
	P2&=0X1F;//����
	P0&=~0X10;
	P2=(P2&0X1F)|0XA0;
}