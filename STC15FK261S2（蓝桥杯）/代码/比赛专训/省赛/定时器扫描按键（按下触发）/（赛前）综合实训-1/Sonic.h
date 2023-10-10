#ifndef __SONIC_H
#define __SONIC_H
#include <STC15F2K60S2.H>
#include "Define.h"
sbit TX=P1^0;
sbit RX=P1^1;
void Sonic_Init(void);
ui Sonic_Work();

#endif