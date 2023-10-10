#ifndef __DS1302_H
#define __DS1302_H
#include <reg52.h>
#include <intrins.h>
#define uint unsigned int 
#define uchar unsigned char
void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
void set_sfm(uchar shi,uchar fen,uchar miao);
unsigned char Read_Ds1302 ( unsigned char address );

#endif
