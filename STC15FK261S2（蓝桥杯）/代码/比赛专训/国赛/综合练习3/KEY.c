#include "KEY.h"

uc Key_val;
uc Key_state;
//void BTN()
//{
//	P3|=0X0F;
//	
//	switch(Key_state)
//	{
//		case 0:
//			if(P30==0){Key_val=7;Key_state=1;}
//			if(P31==0){Key_val=6;Key_state=1;}
//			if(P32==0){Key_val=5;Key_state=1;}
//			if(P33==0){Key_val=4;Key_state=1;}
//			break;
//		case 1:
//			if(P30==1&&P31==1&&P32==1&&P33==1){Key_state=0;}
//			break;
//	}
//}

void KBD()
{
	uc Key1,Key2;
	uc Key_temp;
	
	P32=1;P33=1;P42=0;P44=0;
	if(P32==0){Key1=0X0B;}
	if(P33==0){Key1=0X07;}
	if(P32==1&&P33==1){Key1=0X0F;}
	
	P32=0;P33=0;P42=1;P44=1;
	if(P42==0){Key2=0XB0;}
	if(P44==0){Key2=0X70;}
	if(P42==1&&P44==1){Key2=0XF0;}

	Key_temp=Key1|Key2;
	
	switch(Key_state)
	{
		case 0:if(Key_temp!=0XFF){Key_state=1;}
			break;
		case 1:
			if(Key_temp==0XFF){Key_state=0;}
			else
			{
				switch(Key_temp)
				{
					case 0x77:Key_val=4;break;
					case 0x7B:Key_val=5;break;
					case 0xB7:Key_val=8;break;
					case 0xBB:Key_val=9;break;
				}
				Key_state=2;
			}
			break;
		case 2:if(Key_temp==0XFF){Key_state=0;}
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