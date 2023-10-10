#ifndef __AT24C02_H
#define __AT24C02_H
#include <STC15F2K60S2.H>
#include "Define.h"
void AT24C02_Write(uc Addr,Dat);
uc AT24C02_Read(uc Addr);

#endif