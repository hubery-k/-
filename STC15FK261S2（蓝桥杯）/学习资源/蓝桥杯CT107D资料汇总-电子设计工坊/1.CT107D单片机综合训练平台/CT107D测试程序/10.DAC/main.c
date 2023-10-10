#include "reg52.h"
#include "pcf8591.h"
#include "math.h"

unsigned char idata table[128];

void main(void)
{

  unsigned char i = 0;

  for(i=0;i<128;i++)
  {
	table[i] = 127*sin(2.0*3.1415/256.0*i*2)+128;  
  }
 
  iic_start();
  iic_sendbyte(0x90);
  iic_waitack();
  iic_sendbyte(0x40); //选择DAC功能
  iic_waitack();

  while(1)
  {
    for(i=0;i<128;i++)
	{
	  iic_sendbyte(table[i]); //通过PCF8591输出正弦波
	  iic_waitack();
	}
  }
}