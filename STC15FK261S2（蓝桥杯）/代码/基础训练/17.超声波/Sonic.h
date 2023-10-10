#ifndef __SONIC_H
#define __SONIC_H
#include <STC15F2K60S2.H>

sbit TX=P1^0;
sbit RX=P1^1;

unsigned int Sonic();
#endif