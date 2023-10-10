#ifndef __LED_H
#define __LED_H

#include <STC15F2K60S2.H>
#define HC138_A P2^5
#define HC138_B P2^6
#define HC138_C P2^7

sbit LED1=P0^0;
sbit LED2=P0^1;
sbit LED3=P0^2;
sbit LED4=P0^3;
sbit LED5=P0^4;
sbit LED6=P0^5;
sbit LED7=P0^6;
sbit LED8=P0^7;
void LED_Init();

#endif