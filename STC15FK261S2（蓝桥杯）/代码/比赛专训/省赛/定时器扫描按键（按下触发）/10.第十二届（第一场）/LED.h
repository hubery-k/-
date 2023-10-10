#ifndef __LED_H
#define __LED_H
#include <STC15F2K60S2.H>
void LED_Control(unsigned char Open);
void LED_Init(void);
sbit L1=P0^0;
sbit L2=P0^1;
sbit L3=P0^2;
sbit L4=P0^3;
sbit L5=P0^4;
#endif