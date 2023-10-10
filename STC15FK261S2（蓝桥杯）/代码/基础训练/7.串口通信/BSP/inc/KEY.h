#ifndef __KEY_H
#define __KEY_H
#include <STC15F2K60S2.H>
void KEY_Loop();
unsigned char KEY();
unsigned char Key_scan();  //扫描按键按下的情况，采用定时器轮询
#endif