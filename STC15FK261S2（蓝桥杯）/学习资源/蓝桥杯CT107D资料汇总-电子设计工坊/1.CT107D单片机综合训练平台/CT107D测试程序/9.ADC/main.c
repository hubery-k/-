#include "reg52.h"
#include "pcf8591.h"

void init_t0(void);
void init_pcf8591(void);
void display(unsigned char d);

bit adcflag = 0;
unsigned char dspflag;
unsigned char tflag1;
unsigned char tflag2;
unsigned char code dsp_code_ca[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void main(void)
{
  unsigned char adcvalue;
	
  init_pcf8591();
  init_t0();
  while(1)
  {
	display(adcvalue);
	if(adcflag == 1)
	{
	  adcvalue = adc_pcf8591();
	  adcflag = 0;
	}			
  }
}

void isr_t0(void) interrupt 1
{
  tflag1++;
  tflag2++;
  if(tflag1 == 16)  //4ms
  {
	tflag1 = 0;
	dspflag++;
	if(dspflag == 3)
	  dspflag = 0;
  }
  if(tflag2 == 80) //20ms
  {
	tflag2 = 0;
	adcflag = 1;
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

void init_pcf8591(void)
{
  iic_start();
  iic_sendbyte(0x90);
  iic_waitack();
  iic_sendbyte(CHANNEL_3);
  iic_waitack();
  iic_stop();
  delay(10);
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