#include "reg52.h"

void main(void)
{
  while(1)
  {
    switch(P3)
    {
      case 0xFE:    //S1
       {
		P0 = 0xFC;  
        P2 |= 0xE0;
        P2 &= 0x9F;
        break;
      }
	  case 0xFD:    //S2
      {
	    P0 = 0xF3;  
        P2 |= 0xE0;
        P2 &= 0x9F;
        break;
	  }
      case 0xFB:    //S3
      {
	    P0 = 0xCF;  
        P2 |= 0xE0;
        P2 &= 0x9F;
        break;
	  }
      case 0xF7:	//S4
      {
	    P0 = 0x3F;  
        P2 |= 0xE0;
        P2 &= 0x9F;
        break;
	  }
    }
  }
}
