#include "KEY.h"

unsigned char Trg,Cont;

void KEY()
{
	unsigned char dat;
	dat=P3^0XFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}

/*三行代码解释：
	（1）没有按键按下：dat=0;Cont=0;Trg=0;
	（2）当S7按键按下：KEYPORT=0XFE;dat=0X01;Trg=0X01&(0X01^0X01)=0X01;Cont=0X01;
	（3）当S7按键持续按下：Trg在第一个扫描周期等于0XFE，在第二个扫描周期后Trg=0变回了原状态(为了按下不多次触发);
				Cont=0X01Cont状态没有消除表示按键持续按下。
	（4）当S7按键松开：Trg=0X00(0X00^0X01)=0;Cont=0;
*/