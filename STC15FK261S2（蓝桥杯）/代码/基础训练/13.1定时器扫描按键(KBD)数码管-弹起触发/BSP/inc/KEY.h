#ifndef __KEY_H
#define __KEY_H
#include <STC15F2K60S2.H>
unsigned char Key(void);
unsigned char Key_GetState();
void Key_Loop(void);
#endif