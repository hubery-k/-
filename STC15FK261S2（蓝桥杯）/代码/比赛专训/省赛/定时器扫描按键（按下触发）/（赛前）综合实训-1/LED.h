#ifndef __LED_H
#define __LED_H
#include <STC15F2K60S2.H>
#include "Define.h"
void LED_Init(void);
void LED_Control(uc Open);

sbit L1=P0^0;
sbit L2=P0^1;
sbit L3=P0^2;
sbit L4=P0^3;
sbit L5=P0^4;
sbit L6=P0^5;
sbit L7=P0^6;
#endif