#ifndef _KEY_H_
#define _KEY_H_
#include "define.h"
sbit key1=P3^0;
sbit key2=P3^1;
sbit key3=P3^2;
sbit key4=P3^3;
extern u8 key_val;
extern bit key_flag;
void KeyScan();
void Key_Judge();
#endif