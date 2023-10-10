#ifndef __AT24C02_H
#define __AT24C02_H
#include <STC15F2K60S2.H>

void AT24C02_WriteByte(unsigned char Word_Addr,Date);
unsigned char AT24C02_ReadByte(unsigned char Word_Addr);
#endif