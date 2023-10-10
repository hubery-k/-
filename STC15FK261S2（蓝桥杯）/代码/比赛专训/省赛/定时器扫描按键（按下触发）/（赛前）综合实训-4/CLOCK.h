#ifndef __CLOCK_H
#define __CLOCK_H

#include <STC15F2K60S2.H>
#include "Define.h"
#include "ds1302.h"  
void DS1302_WriteTime(uc *Time);
void DS1302_ReadTime(uc *Time);

#endif