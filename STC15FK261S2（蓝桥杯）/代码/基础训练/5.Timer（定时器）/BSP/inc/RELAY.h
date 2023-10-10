#ifndef __RELAY_H
#define __RELAY_H

#include <STC15F2K60S2.H>
#define HC138_A P2^5
#define HC138_B P2^6
#define HC138_C P2^7
void RELAY_Init(void);
void RELAY_ON(void);
#endif