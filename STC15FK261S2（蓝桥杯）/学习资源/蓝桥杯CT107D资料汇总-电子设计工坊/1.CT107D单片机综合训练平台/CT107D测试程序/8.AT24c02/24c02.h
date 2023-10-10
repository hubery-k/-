#ifndef _24C02_H
#define _24C02_H

#include "reg52.h"
#include "intrins.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();}


sbit SCL = P2^0;
sbit SDA = P2^1;

void iic_start(void);
void iic_stop(void);
void iic_ack(bit ackbit);
void iic_sendbyte(unsigned char byt);
void wrbyte_24c02(unsigned char add,unsigned char dat);
void delay(unsigned char t);

bit iic_waitack(void);
unsigned char i2c_recbyte(void);
unsigned char rdbyte_24c02(unsigned char add);


#endif