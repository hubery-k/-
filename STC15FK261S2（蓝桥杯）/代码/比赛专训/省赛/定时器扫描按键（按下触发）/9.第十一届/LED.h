#ifndef __LED_H
#define __LED_H
#include <STC15F2K60S2.H>
#include "Define.h"
void LED_Init();
void LED_Control(uc Open);

sbit L1=P0^0;
sbit L2=P0^1;
sbit L3=P0^2;
#endif