#include "KEY.h"
#include "Delay.h"

/*使用独立按键*/
unsigned char KeyNum;
void KEY_Press(void)
{
	
	if(KEY7==0){Delay20ms();while(KEY7==0);Delay20ms();KeyNum=4;}
	if(KEY6==0){Delay20ms();while(KEY6==0);Delay20ms();KeyNum=3;}
	if(KEY5==0){Delay20ms();while(KEY5==0);Delay20ms();KeyNum=2;}
	if(KEY4==0){Delay20ms();while(KEY4==0);Delay20ms();KeyNum=1;}
}
/*记住在Key函数中不能利用return来返回键值。例如，当第一次按下KeyNum=1，那么下次调用该函数时即使没有按键按下依然会返回键值KeyNum=1
	这里我使用全局变量返回键值*/