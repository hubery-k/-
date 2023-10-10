#include "pcf8591.h"

void iic_start(void)
{
  SDA = 1;
  _nop_();
  SCL = 1;
  somenop;
  SDA = 0;
  somenop;
  SCL = 0;
}

void iic_stop(void)
{
  SDA = 0;
  _nop_();
  SCL = 1;
  somenop;
  SDA = 1;
}

bit iic_waitack(void)
{
  SDA = 1;
  somenop;
  SCL = 1;
  somenop;
  if(SDA)    
  {   
	SCL = 0;
	iic_stop();
	return 0;
  }
  else  
  { 
	SCL = 0;
	return 1;
  }
}

void iic_sendbyte(unsigned char byt)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {   
  	if(byt&0x80) 
      SDA = 1;
	else 
	  SDA = 0;
	somenop;
	SCL = 1;
	byt <<= 1;
	somenop;
    SCL = 0;
  }
}