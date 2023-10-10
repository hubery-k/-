#ifndef __RB_H
#define __RB_H
#include <STC15F2K60S2.H>
#include "Define.h"
void RB_Control(uc Open);
void RB_Init();
sbit Relay=P0^4;
sbit Buzzer=P0^6;
#endif

