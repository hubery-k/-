#include "reg52.h"
#include "24c02.h"

unsigned char dspflag;
unsigned char cnt;
unsigned char tflag;
unsigned char code dsp_code_ca[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void display(unsigned char d);
void init_t0(void);

void main()
{
  //wrbyte_24c02(0x00,0x00);
  cnt = rdbyte_24c02(0x00);
  wrbyte_24c02(0x00,cnt+1);
  
  init_t0();	
  while(1)
  {
    display(cnt);
  }	
}

void isr_t0(void) interrupt 1
{
  tflag++;
  if(tflag == 16)
  {
	tflag = 0;
	dspflag ++;
	if(dspflag == 3)
	  dspflag = 0;
  }
}

void init_t0(void)
{	
  TMOD = 0x02;
  TH0 = 0x06;
  TL0 = 0x06;
  ET0 = 1;
  EA = 1;
  TR0 = 1;
}
void display(unsigned char d)
{
  if((dspflag == 0)&&(d>99))
  {
    P0 = 0xFF;
	P2 |= 0xE0;
	P2 &= 0x1F;

	P0 = dsp_code_ca[d/100];
	P2 |= 0xE0;
	P2 &= 0x1F;
	P0 = 0x20;
	P2 |= 0xC0;
	P2 &= 0x3F;
  }
  if((dspflag == 1)&&(d>9))
  {
	P0 = 0xFF;
    P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d%100/10];
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x40;
    P2 |= 0xC0;
    P2 &= 0x3F;
  } 
  if(dspflag == 2)
  {
    P0 = 0xFF;
	P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d%100%10]; 
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x80;
    P2 |= 0xC0;
    P2 &= 0x3F;
  }  
}