#ifndef __AT24C02_H
#define __AT24C02_H
#include <STC15F2K60S2.H>
#include "Define.h"
void EEPROM_Write(uc addr,f da);
uc EEPROM_Read(uc addr);
#endif