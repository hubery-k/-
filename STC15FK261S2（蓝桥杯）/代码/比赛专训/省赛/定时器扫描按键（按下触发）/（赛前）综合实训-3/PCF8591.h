#ifndef __PCF8591_H
#define __PCF8591_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

#define PCF8591_ADDRESS 0X90
#define AD_AIN1 0X01
#define AD_AIN3 0X03
#define DA 0X40
uc PCF8591_ReadAD(bit Channel);
void PCF8591_WriteDA(uc Data);
#endif