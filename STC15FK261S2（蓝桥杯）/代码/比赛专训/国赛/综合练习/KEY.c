#include "KEY.h"

uc Cont;
bit K9_Press;
bit K8_Press;
uc Key_val;
uc Key_state;

void KBD()
{
	uc Key1,Key2;
	uc Key_temp=0;
	
	P32=1;P33=1;P42=0;P44=0;
	if(P32==0)Key1=0x0b;
	if(P33==0)Key1=0x07;
	if(P32==1 && P33==1)Key1=0x0f;
	
	P32=0;P33=0;P42=1;P44=1;
	if(P42==0)Key2=0xb0;
	if(P44==0)Key2=0x70;
	if(P42==1&&P44==1)Key2=0xf0;
	
	Key_temp=Key1|Key2;
	
	switch(Key_state)
	{
		case 0:
					if(Key_temp != 0xff)Key_state=1;
					break;
		case 1:
					if(Key_temp == 0xff)Key_state=0;
					else
					{
						switch(Key_temp)
						{
							case 0X77:Key_val=4;break;
							case 0X7b:Key_val=5;break;
							case 0Xb7:Key_val=8;Cont=8;K8_Press=1;break;
							case 0Xbb:Key_val=9;Cont=9;K9_Press=1;break;
						}
						Key_state=2;
					}
					break;
		case 2:
					if(Key_temp == 0xff)
					{
						Key_state=0;
						K8_Press=0;
						K9_Press=0;
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