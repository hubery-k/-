#include "KEY.h"

uc Key_val;
uc Key_state;
uc Cont;	//用来保存按键状态
bit K12_Press;
bit K13_Press;

void KBD()
{
	uc Key_temp=0;
	uc Key1,Key2;
	
	P32=1;P33=1;P34=0;P35=0;
	if(P33==0)Key1=0x07;
	if(P32==0)Key1=0x0b;
	if(P33==1 && P32==1)Key1=0x0f;
	
	P32=0;P33=0;P34=1;P35=1;
	if(P35==0)Key2=0xd0;
	if(P34==0)Key2=0xe0;
	if(P35==1 && P34==1)Key2=0xf0;
	Key_temp=Key1|Key2;
	
	switch(Key_state)
	{
		case 0:	//按下检测
				if(Key_temp!=0xff)Key_state=1;
				break;
				
		case 1:	//判断是哪个按键按下
				if(Key_temp==0xff)Key_state=0;
				else 
				{
					switch(Key_temp)
					{
						case 0XD7:Key_val=12;Cont=12;K12_Press=1;break;
						case 0XDB:Key_val=13;Cont=13;K13_Press=1;break;
						case 0XE7:Key_val=16;break;
						case 0XEB:Key_val=17;break;
					}
					Key_state=2;
				}
				break;
		case 2:	//松手检测
				if(Key_temp==0xff)
				{
					Key_state=0;
					K12_Press=0;
					K13_Press=0;
				}
				break;
	}
}

uc KEY()
{
	uc KeyNum;

	KBD();
	KeyNum=Key_val;
	Key_val=0;
	return KeyNum;
}