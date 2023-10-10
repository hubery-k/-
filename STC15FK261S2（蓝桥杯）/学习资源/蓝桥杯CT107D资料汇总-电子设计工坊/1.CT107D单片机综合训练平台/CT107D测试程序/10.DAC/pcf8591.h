#ifndef _PCF8591_H
#define _PCF8591_H

#include "reg52.h"
#include "intrins.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();}

sbit SDA = P2^1;
sbit SCL = P2^0;

#define CHANNEL_0 0x00
#define CHANNEL_1 0x01
#define CHANNEL_2 0x02
#define CHANNEL_3 0x03

void iic_start(void);
void iic_stop(void);
void iic_ack(bit ackbit);
void iic_sendbyte(unsigned char byt);

bit iic_waitack(void);
unsigned char i2c_recbyte(void);

#endif