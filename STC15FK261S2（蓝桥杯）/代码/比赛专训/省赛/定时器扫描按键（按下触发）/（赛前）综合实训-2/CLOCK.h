#ifndef __CLOCK_H
#define __CLOCK_H

#include <STC15F2K60S2.H>
#include "ds1302.h"
#include "Define.h"
void Write_Time(uc *Time);
void Read_Time(uc *Time);
#endif