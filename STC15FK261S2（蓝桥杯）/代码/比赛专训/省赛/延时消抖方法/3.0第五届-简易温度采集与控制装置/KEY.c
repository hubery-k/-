#include "KEY.h"
#include "Delay.h"

unsigned char KeyNum;



unsigned char MatrixKey()
{
	P3|=0X3F;//为了方便判断按键按下先都给赋成1。
	P4|=0X14;
	P30=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=0;}
	if(P42==0){Delay20ms();while(P42==0);Delay20ms();KeyNum=1;}
	if(P35==0){Delay20ms();while(P35==0);Delay20ms();KeyNum=2;}

	
	P3|=0X3F;
	P4|=0X14;
	P31=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=3;}
	if(P42==0){Delay20ms();while(P42==0);Delay20ms();KeyNum=4;}
	if(P35==0){Delay20ms();while(P35==0);Delay20ms();KeyNum=5;}

	
	P3|=0X3F;
	P4|=0X14;
	P32=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=6;}
	if(P42==0){Delay20ms();while(P42==0);Delay20ms();KeyNum=7;}
	if(P35==0){Delay20ms();while(P35==0);Delay20ms();KeyNum=8;}

	
	P3|=0X3F;
	P4|=0X14;
	P33=0;
	if(P44==0){Delay20ms();while(P44==0);Delay20ms();KeyNum=9;}
	if(P42==0){Delay20ms();while(P42==0);Delay20ms();KeyNum=10;}//设置按键
	if(P35==0){Delay20ms();while(P35==0);Delay20ms();KeyNum=11;}//清除按键

	return KeyNum;
}