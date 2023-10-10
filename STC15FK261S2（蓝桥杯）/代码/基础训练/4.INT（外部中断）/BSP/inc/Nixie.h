#ifndef __NIXIE_H
#define __NIXIE_H
#include <STC15F2K60S2.H>
void Nixie_Init(unsigned char n);
void Nixie_show(unsigned char index);
#endif