#ifndef __EEPROM_H
#define __EEPROM_H
#include <STC15F2K60S2.H>
void AT24C02_WriteByte(unsigned char Word_Addr,unsigned char Data);
unsigned char AT24C02_Read(unsigned char Word_Addr);
#endif