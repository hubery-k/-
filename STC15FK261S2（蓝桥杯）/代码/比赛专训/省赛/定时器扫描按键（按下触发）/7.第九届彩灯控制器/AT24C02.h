#ifndef __AT24C02_H
#define __AT24C02_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

/*�ӻ��豸��ַ+д-0XA0���ӻ��豸��ַ+��-0XA1*/
#define AT24C02_ADDRESS 0XA0

uc AT24C02_ReadByte(uc Addr);
void AT24C02_WriteByte(uc Addr,Data);

#endif