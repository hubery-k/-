#include "KEY.h"

static unsigned char KeyNum;
unsigned char Trg=0,Cont=0;
void BTN()
{
	unsigned char dat=P3^0xFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}



void KEY_Loop()
{
	BTN();
	if(Trg&0X01){KeyNum=7;}		//S7
	if(Trg&0X02){KeyNum=6;}		//S6
	if(Trg&0X04){KeyNum=5;}		//S5
	if(Trg&0X08){KeyNum=4;}		//S4
}


unsigned char KEY(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//每一次调用结束后清除按键状态
	return Temp;
}
/*记住在Key函数中不能利用return来返回键值。例如，当第一次按下KeyNum=1，那么下次调用该函数时即使没有按键按下依然会返回键值KeyNum=1
	这里我使用全局变量返回键值*/