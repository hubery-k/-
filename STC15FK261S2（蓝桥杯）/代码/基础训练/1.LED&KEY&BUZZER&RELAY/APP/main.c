#include "Nixie.h"
#include "LED.h"
#include "BUZZER.h"
#include "RELAY.h"


/*很重要的是138的输出是互斥低有效输出，也就是Y0~Y7只能输出一路低电平*/


//关闭外设
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