#ifndef __NIXIE_H
#define __NIXIE_H
#include <STC15F2K60S2.H>
#include "Define.h"
void Nixie_Loop(void);
void Nixie_SetBuf(uc Location,Data);
#endif