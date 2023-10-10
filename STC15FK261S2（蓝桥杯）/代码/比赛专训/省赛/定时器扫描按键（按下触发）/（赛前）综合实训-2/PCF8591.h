#ifndef __PCF8591_H
#define __PCF8591_H
#include <STC15F2K60S2.H>
#include "iic.h"
#include "Define.h"

#define PCF8591_ADDRESS 0X90
#define AD_AIN1 0X01
#define AD_AIN3 0X03
uc Read_AD(bit Channel);
#endif