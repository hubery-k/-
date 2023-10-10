#include "BUZZER.h"
#include "Delay.h"


void BUZZER_Init(void)
{
	P2=(P2&0x1f)|0xA0;
	P0&=~0X40;
}

void BUZZER_ring(void)
{
	BUZZER_Init();
	P0|=0X40;//´ò¿ª
	Delay500ms();
	P0&=~0X40;//¹Ø±Õ
}