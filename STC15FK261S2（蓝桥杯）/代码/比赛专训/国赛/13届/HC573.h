#ifndef __HC573_H
#define __HC573_H
#include <STC15F2K60S2.H>
#include "Define.h"

sbit L1=P0^0;
sbit L2=P0^1;
sbit L3=P0^2;
sbit L4=P0^3;
sbit L5=P0^4;
sbit L6=P0^5;
sbit L7=P0^6;
sbit L8=P0^7;
sbit RELAY=P0^4;

void LED_Init(void);
void RB_Init(void);
void Nixie_Loop(void);
void Nixie_SetBuf(uc pos,val);


#endif