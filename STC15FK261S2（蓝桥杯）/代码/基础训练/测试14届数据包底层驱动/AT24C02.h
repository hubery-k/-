#ifndef __AT24C02_H
#define __AT24C02_H
#include <STC15F2K60S2.H>

void AT24C02_Write(unsigned char Addr,Dat);
unsigned char AT24C02_Read(unsigned char Addr);

#endif