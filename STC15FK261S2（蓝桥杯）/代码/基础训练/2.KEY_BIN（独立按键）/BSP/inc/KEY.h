#ifndef __KEY_H
#define __KEY_H
#include <STC15F2K60S2.H>
extern unsigned char KeyNum;
sbit KEY4=P3^3;
sbit KEY5=P3^2;
sbit KEY6=P3^1;
sbit KEY7=P3^0;
void KEY_Press(void);

#endif