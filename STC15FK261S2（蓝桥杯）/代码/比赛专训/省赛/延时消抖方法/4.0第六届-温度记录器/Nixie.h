#ifndef __NIXIE_H
#define __NIXIE_H
#include <STC15F2K60S2.H>

void Nixie_Display(unsigned char pos,value);
void Nixie_NoDisplay(void);
void Nixie_ALL(void);
#endif