#ifndef __KEY_H
#define __KEY_H
#include <STC15F2K60S2.H>

sbit S7=P3^0;
sbit S6=P3^1;
sbit S5=P3^2;
sbit S4=P3^3;

void KEY_Loop();
unsigned char KEY();

#endif