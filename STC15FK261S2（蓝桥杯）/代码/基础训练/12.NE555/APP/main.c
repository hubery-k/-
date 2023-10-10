#include "BUZZER.h"
#include "Nixie.h"
#include "KEY.h"
#include "NE555.h"
#include "Delay.h"
unsigned char code SMG_donot[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
unsigned char code SMG_not[16]=		
{0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};


void main(void)
{
	BUZZER_Init();
	NE555_Init();
	while(1)
	{
		Nixie_show(0,SMG_donot[15]);
		
		if(temp>9999){Nixie_show(3,SMG_donot[temp/10000]);}
		if(temp>999){Nixie_show(4,SMG_donot[temp/1000%10]);}
		if(temp>99){Nixie_show(5,SMG_donot[temp/100%10]);}
		if(temp>9){Nixie_show(6,SMG_donot[temp/10%10]);}
		Nixie_show(7,SMG_donot[temp%10]);
	}
}
