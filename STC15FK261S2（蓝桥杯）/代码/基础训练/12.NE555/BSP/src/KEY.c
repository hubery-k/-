#include "KEY.h"
#include "Delay.h"

unsigned char KeyNum;
unsigned char MatrixKey()
{

	P3|=0X3F;
	P4|=0X14;
	P30=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=1;}
	if(P42==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=2;}
	if(P35==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=3;}
	if(P34==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=4;}
	
	P3|=0X3F;
	P4|=0X14;
	P31=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=5;}
	if(P42==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=6;}
	if(P35==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=7;}
	if(P34==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=8;}
	
	P3|=0X3F;
	P4|=0X14;
	P32=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=9;}
	if(P42==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=10;}
	if(P35==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=11;}
	if(P34==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=12;}
	
	P3|=0X3F;
	P4|=0X14;
	P33=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=13;}
	if(P42==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=14;}
	if(P35==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=15;}
	if(P34==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=16;}
	return KeyNum;
}


