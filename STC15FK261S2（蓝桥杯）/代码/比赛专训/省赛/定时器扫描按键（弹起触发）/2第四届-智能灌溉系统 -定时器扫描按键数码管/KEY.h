#ifndef __KEY_H
#define __KEY_H
#include <STC15F2K60S2.H>
sbit KEY7 = P3^0;
sbit KEY6 = P3^1;
sbit KEY5 = P3^2;
sbit KEY4 = P3^3;
void Key_Loop(void);
unsigned char Key(void);
#endif