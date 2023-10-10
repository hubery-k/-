#include "Nixie.h"
#include "LED.h"
#include "BUZZER.h"
#include "RELAY.h"


/*����Ҫ����138������ǻ������Ч�����Ҳ����Y0~Y7ֻ�����һ·�͵�ƽ*/


//�ر�����
void Cls_Peripherial(void)
{
	P0=0xff;
	P2=P2&0x1f|0x80;
	P2&=0x1f;
	
	P0=0;
	P2=P2&0X1F|0XA0;
	P2&=0X1F;
}
void main(void)
{
	BUZZER_Init();
	RELAY_Init();
	while(1)
	{
		LED_Run();
		BUZZER_ring();
		RELAY_ON();
	}
}