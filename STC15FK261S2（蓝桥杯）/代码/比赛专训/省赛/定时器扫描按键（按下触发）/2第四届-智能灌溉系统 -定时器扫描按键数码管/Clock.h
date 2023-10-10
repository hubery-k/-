#ifndef __CLOCK_H
#define __CLOCK_H
#include <STC15F2K60S2.H>
#include "ds1302.h"
void Set_Time(unsigned char *Time);
void Read_Time(unsigned char *Time);
#endif