#ifndef __AT24C02_H
#define __AT24C02_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

/*从机设备地址+写-0XA0，从机设备地址+读-0XA1*/
#define AT24C02_ADDRESS 0XA0

uc AT24C02_ReadByte(uc Addr);
void AT24C02_WriteByte(uc Addr,Data);

#endif