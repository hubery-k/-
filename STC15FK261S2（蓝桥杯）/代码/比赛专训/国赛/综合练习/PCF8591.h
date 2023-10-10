#ifndef __PCF8591_H
#define __PCF8591_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

uc PCF8591_ReadAD(bit Channel);
void PCF8591_WriteDA(uc Dat);

#endif