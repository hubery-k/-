#ifndef __PCF8591_H
#define __PCF8591_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

#define AD_AIN1 0X01
#define AD_AIN3 0X03

void PCF8591_WriteDA(uc Dat);
#endif