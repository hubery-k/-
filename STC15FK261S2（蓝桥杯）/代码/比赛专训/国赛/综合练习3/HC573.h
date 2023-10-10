#ifndef __HC573_H
#define __HC573_H
#include <STC15F2K60S2.H>
#include "Define.h"

void LED_Init();
void RB_Init();
void Nixie_Loop();
void Nixie_SetBuf(uc pos,val);
#endif