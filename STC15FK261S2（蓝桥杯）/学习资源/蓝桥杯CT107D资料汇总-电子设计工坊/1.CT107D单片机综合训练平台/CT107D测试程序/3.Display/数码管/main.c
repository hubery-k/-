#include "reg52.h"

unsigned char code dsp_code_ca[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned int tflag;
unsigned char dspflag;
unsigned char second;
unsigned char dflag;

void display(unsigned char d);

void main()
{
  TMOD |= 0x02;
  TH0 = 0x06;
  TL0 = 0x06;
  EA = 1;
  ET0 = 1;
  TR0 = 1;
  while(1)
  {
    display(second);  
  }
}

//T0中断服务函数
void isr_t0(void) interrupt 1
{
  tflag++;
  dflag++;
  if(dflag == 16)
  {
  	dflag = 0;
	dspflag++;
	if(dspflag == 2)
	  dspflag = 0;
  }
  if(tflag == 4000)
  {
    tflag = 0;
    second++;
    if(second == 60)
      second = 0;
  }  
}

//显示程序
void display(unsigned char d)
{
  if((dspflag == 0) && (d/10 != 0))
  {
	P0 = 0xFF;
    P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d/10];
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x40;
    P2 |= 0xC0;
    P2 &= 0x3F;
  } 
  if(dspflag == 1)
  {
    P0 = 0xFF;
	P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d%10]; 
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x80;
    P2 |= 0xC0;
    P2 &= 0x3F;
  }  
}
