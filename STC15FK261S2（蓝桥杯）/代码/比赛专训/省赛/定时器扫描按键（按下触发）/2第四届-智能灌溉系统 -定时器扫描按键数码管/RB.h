#ifndef __RB_H
#define __RB_H
#include <STC15F2K60S2.H>

void RB_Control(unsigned char Open);
void RB_Init(void);

sbit Relay=P0^4;
sbit Buzzer=P0^6;
#endif