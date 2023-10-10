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

void iic_ack(bit ackbit)
{
  if(ackbit) 
	SDA = 0;
  else 
	SDA = 1; 
  somenop;
  SCL = 1;
  somenop;
  SCL = 0;
  SDA = 1; 
  somenop;
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

unsigned char iic_recbyte(void)
{
  unsigned char da;
  unsigned char i;
  for(i=0;i<8;i++)
  {   
	SCL = 1;
	somenop;
	da <<= 1;
	if(SDA) 
	  da |= 0x01;
	SCL = 0;
	somenop;
  }
  return da;
}

void delay(unsigned char t)
{
  unsigned char i;
  while(t--)
  {
    for(i=0;i<112;i++);
  }
}

unsigned char adc_pcf8591(void)
{
	unsigned char temp;

	iic_start();
	iic_sendbyte(0x91);
	iic_waitack();
	temp = iic_recbyte();
	iic_ack(0);
	iic_stop();
	
	return temp;
}
