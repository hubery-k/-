#include "KEY.h"
#include "Delay.h"
unsigned char KeyNum;


void KEY_Press(void)
{
	if(KEY7==0){Delay20ms();while(KEY7==0)Delay20ms();KeyNum=4;}
	if(KEY6==0){Delay20ms();while(KEY6==0)Delay20ms();KeyNum=3;}
	if(KEY5==0){Delay20ms();while(KEY5==0)Delay20ms();KeyNum=2;}
	if(KEY4==0){Delay20ms();while(KEY4==0)Delay20ms();KeyNum=1;}
}
