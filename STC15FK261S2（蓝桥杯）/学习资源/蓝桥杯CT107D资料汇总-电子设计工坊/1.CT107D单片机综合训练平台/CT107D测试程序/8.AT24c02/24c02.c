#include "24c02.h"

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

void wrbyte_24c02(unsigned char add,unsigned char dat)
{
  // Device Address 1100 000 R/W
  iic_start();
  iic_sendbyte(0xa0);
  iic_waitack();
  iic_sendbyte(add);
  iic_waitack();
  iic_sendbyte(dat);
  iic_waitack();
  iic_stop();
  delay(10);
}

unsigned char rdbyte_24c02(unsigned char add)
{
  // Device Address 1100 000 R/W
  unsigned char da;
  iic_start();
  iic_sendbyte(0xa0);
  iic_waitack();
  iic_sendbyte(add);
  iic_waitack();
  iic_start();
  iic_sendbyte(0xa1);
  iic_waitack();
  da = iic_recbyte();
  iic_ack(0); 
  iic_stop();
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
