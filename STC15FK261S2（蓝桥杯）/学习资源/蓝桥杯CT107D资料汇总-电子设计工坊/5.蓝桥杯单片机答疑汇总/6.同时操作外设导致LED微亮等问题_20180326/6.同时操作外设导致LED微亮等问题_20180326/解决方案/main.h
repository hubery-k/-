#ifndef _MAIN_H_
#define _MAIN_H_
#include "define.h"
#include "key.h"
#include "nixie.h"
#include "delay.h"
//时间变量
u8 hour,min,sec;
//定时器计时变量
u16 s1_count;
//led标志位，只有两个状态，为减少空间占用，定义为bit,0->off,1->on
bit led8_flag,led7_flag,led6_flag,led5_flag;
//
u8 led_count;
#endif