#ifndef __KEY_H
#define __KEY_H
#include <STC15F2K60S2.H>
#include "Define.h"
extern uc Trg,Cont;
void KEY_Loop();
uc Key();		//传递键值并清除按键状态

#endif