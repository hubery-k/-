#include "BUZZER.h"
#include "Nixie.h"
#include "KEY.h"
#include "Timer.h"
#include "LED.h"

unsigned char code SMG_nodot[19]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f,0XFF};
unsigned char code SMG_dot[16]=		
{0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};


unsigned char T0Count1;
unsigned char T0Count2;
unsigned char KeyNum;
unsigned char i,j,k,l;
bit Key_flag;



void main(void)
{
	BUZZER_Init();
	Timer0Init();
	while(1)
	{
		if(Key_flag==1)
		{
			Key_flag=0;
			BTN();
			if(Trg & 0x01)//s7(判断P30是否为0-S7是否按下)
			{
				Nixie_SetBuf(0,SMG_nodot[i++]);
			}
			if(Trg & 0x02)//s6
			{
				Nixie_SetBuf(1,SMG_nodot[j++]);
			}
			if(Trg & 0x04)//s5
			{
				Nixie_SetBuf(2,SMG_nodot[k++]);
			}
			if(Trg & 0x08)//s4
			{
				Nixie_SetBuf(3,SMG_nodot[l++]);
			}
		}
	}
}

void Timer0_Routine(void) interrupt 1
{
	T0Count1++;
	if(T0Count1==2)//2ms扫描显示数码管1次
	{
		T0Count1=0;
		Nixie_Loop();
	}
	T0Count2++;
	if(T0Count2==10)//10ms扫描显示按键1次
	{
		T0Count2=0;
		Key_flag=1;
	}
}
