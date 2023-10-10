#include "reg52.h"

void delay(void);

void main()
{
  unsigned char i;
  while(1)
  {
    for(i=0;i<8;i++)
    {
      P0 = ~(1<<i);
      P2 |= 0xE0;
      P2 &= 0x9F; 
      delay();
    }
  }
}

//Èí¼þÑÓÊ±³ÌÐò
void delay(void)
{
  unsigned char i,j,k;
  for(i=20;i>0;i--)
  for(j=20;j>0;j--)
  for(k=248;k>0;k--);
}