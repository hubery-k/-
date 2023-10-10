#include "reg52.h"

sbit buzzer = P0^6;
sbit relay = P0^4;

void main(void)
{
	
	//关闭蜂鸣器
	buzzer = 0;	            //高有效
	P2 = 0xA0;
	P2 = 0x00;

	//关闭继电器
	relay = 0;              //高有效
	P2 = 0xA0;
	P2 = 0x00;

	//关闭数码管显示
	P0 = 0x00;              //位选置低
	P2 = 0xC0;
	P2 = 0x00;

	//关闭发光二极管显示
	P0 = 0xFF;              //低有效
	P2 = 0x80;
	P2 = 0x00;
		
	while(1);
}