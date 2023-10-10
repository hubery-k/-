#include "KEY.h"

#define set(x) P4=(x>>3|x>>4);P3=x
#define get() ((P4&0X10)<<3)|((P4&0X04)<<4)|(P3&0X3F)

unsigned char Trg,Cont;
static unsigned char KeyNum;

void KBD()
{
	unsigned char Dat;
	set(0X0F);
	Dat=get();
	set(0xF0);
	Dat=(Dat|get())^0XFF;
	Trg=Dat&(Dat^Cont);		//保证按下只触发一次
	Cont=Dat;		//记住按下的状态
}

void KEY_Loop(void)
{
	KBD();
	switch(Trg ^ 0xFF)
	{
		case 0x77:KeyNum=9;break;
		case 0x7b:KeyNum=6;break;
		case 0x7d:KeyNum=3;break;
		case 0x7e:KeyNum=0;break;
		
		case 0xb7:KeyNum=10;break;
		case 0xbb:KeyNum=7;break;
		case 0xbd:KeyNum=4;break;
		case 0xbe:KeyNum=1;break;
		
		case 0xd7:KeyNum=11;break;
		case 0xdb:KeyNum=8;break;
		case 0xdd:KeyNum=5;break;
		case 0xde:KeyNum=2;break;
	}
}

unsigned char KEY()
{
	unsigned char Temp;
	Temp=KeyNum;
	KeyNum=20;		//20没有对应使用的键值
	return Temp;
}

/*三行代码解释：
	（1）没有按键按下：dat=0;Cont=0;Trg=0;
	（2）当S7按键按下：KEYPORT=0XFE;dat=0X01;Trg=0X01&(0X01^0X00)=0X01;Cont=0X01;
	（3）当S7按键持续按下：Trg在第一个扫描周期等于0X01，在第二个扫描周期后Trg=0变回了原状态(为了按下不多次触发);
				Cont=0X01Cont状态没有消除表示按键持续按下。
	（4）当S7按键松开：Trg=0X00(0X00^0X01)=0;Cont=0;
*/
