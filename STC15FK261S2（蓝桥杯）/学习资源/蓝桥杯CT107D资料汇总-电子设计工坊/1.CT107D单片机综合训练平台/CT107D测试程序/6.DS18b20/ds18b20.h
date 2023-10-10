#ifndef _DS18B20_H
#define _DS18B20_H

#include "reg52.h"
#include "intrins.h"

sbit DQ = P1^4;

bit init_ds18b20(void);
void delay(unsigned int t);
void wr_ds18b20(unsigned char byt);
unsigned char rd_ds18b20(void);
unsigned char rd_temperature(void);

#endif
