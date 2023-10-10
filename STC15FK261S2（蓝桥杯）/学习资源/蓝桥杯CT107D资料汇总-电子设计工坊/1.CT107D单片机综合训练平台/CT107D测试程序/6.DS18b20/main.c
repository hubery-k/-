#include "reg52.h"
#include "ds18b20.h"

unsigned int tflag;
bit secflag = 0;
unsigned char text[] = "Temperature = ";
unsigned char enter[] = {0x0A,0x0D};    //回车符

void uart_tx(unsigned char *p,unsigned char length);
char dat(char a);

void main(void)
{
  unsigned char temp;
  
  SCON = 0x50;
  TMOD = 0x22;
  
  TH1 = 0xF3;
  TL1 = 0xF3;
  TH0 = 0x06;
  TL0 = 0x06;
  
  EA = 1;
  ET0 = 1;

  TR0 = 1;
  TR1 = 1;
  
  while(1)
  {
    
	if(secflag == 1)
	{
	  secflag = 0;
	  uart_tx(text,14);
	  
	  temp = rd_temperature();

	  if(temp>99)
	  {
	  	SBUF = temp/100 + 0x30;
	  	while(TI == 0);
	  	TI = 0;
	  }

	  if(temp>9)
	  {
	  	SBUF = temp%100/10 + 0x30;
	  	while(TI == 0);
	  	TI = 0;
	  }

	  SBUF = temp%100%10 + 0x30;
	  while(TI == 0);
	  TI = 0;

	  uart_tx(enter,2);
	}        
  }
}

//定时器TO中断服务函数
void isr_t0(void) interrupt 1
{
  tflag++;
  if(tflag == 4000)
  {
    tflag = 0;
	secflag = 1;
  }
}

//串口传输函数
void uart_tx(unsigned char *p,unsigned char length)
{
  unsigned char i;
  for(i=0;i<length;i++)
  {
	SBUF = *(p+i);
	while(TI == 0);
	TI = 0;
  }
}

