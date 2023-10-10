#include "Nixie.h"
#include "KEY.h"
#include "BUZZER.h"
#include "Delay.h"

unsigned char i;
unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
     0xbf,0x7f};
		
		 
void main(void)
{
	BUZZER_Init();
	Nixie_Init(1);
	while(1)
	{
		MatrixKey();

		if(KeyNum!=0){DisplaySMG(0,SMG_duanma[KeyNum]);}
	}
}