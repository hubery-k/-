#ifndef __NIXIE_H
#define __NIXIE_H
#include <STC15F2K60S2.H>
void Nixie_Init(unsigned char n);
void DisplaySMG_Bit(unsigned char pos,unsigned char value);
void Nixie_show(unsigned char pos,unsigned char index);
#endif