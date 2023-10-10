#ifndef _DEFINE_H_
#define _DEFINE_H_
#include <STC15F2K60S2.H>
typedef unsigned char u8;
typedef unsigned int u16;
sbit BUZZER=P0^6;
sbit RELAY=P0^4;
#define NU 0X1F
#define Y P2=NU
#define Y4 P2=(P2&NU)|0X80
#define Y5 P2=(P2&NU)|0XA0
#define Y6 P2=(P2&NU)|0XC0
#define Y7 P2=(P2&NU)|0XE0
#define LED_OFF P0=0XFF;Y4;Y//¹Ø±Õled
#define RnB_OFF BUZZER=0;RELAY=0;Y5;Y//¹Ø±Õ·äÃùÆ÷
#define NIX_OFF  P0=0Xff;Y7;Y//¹Ø±Õ
#define LED8_ON  Y4;P0=0X7F;Y
#define LED7_ON  Y4;P0=0XBF;Y
#define LED6_ON  Y4;P0=0XDF;Y
#define LED5_ON  Y4;P0=0XEF;Y

#endif