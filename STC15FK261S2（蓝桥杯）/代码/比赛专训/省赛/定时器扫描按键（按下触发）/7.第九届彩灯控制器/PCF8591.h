#ifndef __PCF8591_H
#define __PCF8591_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

/*从及设备地址+写指令==0X90   从机设备地址+读地址==0X91 */
#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01					//光敏电阻
#define COMMAND_AIN3 0X03					//可调电阻

uc PCF8591_ReadAD();
#endif