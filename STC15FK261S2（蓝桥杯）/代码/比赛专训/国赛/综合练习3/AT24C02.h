#ifndef __AT24C02_H
#define __AT24C02_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

uc AT24C02_ReadByte(uc Addr);
void AT24C02_WriteByte(uc Addr,Dat);
#endif