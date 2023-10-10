#include "KEY.h"

uc key_val;
uc Key_state;

void KBD()
{
	uc key_temp=0;
	uc key1,key2;
	
	P32=1;P33=1;P42=0;P44=0;
	if(P33==0)key1=0x07;
	if(P32==0)key1=0x0b;
	if(P33==1 && P32==1)key1=0x0f;
	
	P32=0;P33=0;P42=1;P44=1;
	if(P44==0)key2=0x70;
	if(P42==0)key2=0xb0;
	if(P44==1 && P42==1)key2=0xf0;
	key_temp=key1|key2;
	
	switch(Key_state)
	{
		case 0:	//按下检测
				if(key_temp!=0xff)Key_state=1;
				break;
				
		case 1:	//判断是哪个按键按下
				if(key_temp==0xff)Key_state=0;
				else 
				{
					switch(key_temp)
					{
						case 0X77:key_val=4;break;
						case 0X7B:key_val=5;break;
						case 0XB7:key_val=8;break;
						case 0XBB:key_val=9;break;
					}
					Key_state=2;
				}
				break;
		case 2:	//松手检测
				if(key_temp==0xff)
				{
					Key_state=0;
				}
				break;
	}
}

uc KEY()
{
	uc KeyNum;

	KBD();
	KeyNum=key_val;
	key_val=0;
	return KeyNum;
}
