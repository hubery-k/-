#include "KEY.h"
#include "Delay.h"

static unsigned char KeyNum;

/**
  * @brief  获取按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,	0表示无按键按下
  */
	
unsigned char Trg=0,Cont=0;
void BTN()
{
	unsigned char dat=P3^0xFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}


