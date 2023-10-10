#ifndef __DS18B20_H
#define __DS18B20_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "onewire.h"
#define SKIP_ROM 0XCC
#define CONVERT_T 0X44
#define READ_T 0XBE

void DS18B20_ConvertT();
f DS18B20_ReadT();
#endif