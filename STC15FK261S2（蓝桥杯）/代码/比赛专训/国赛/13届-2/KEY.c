#include "KEY.h"

uc Key_val;
uc Key_state;
uc Cont;
bit S7_Press;
void BTN()
{
	P3|=0X0F;
	
	switch(Key_state)
	{
		case 0:
			if(P30==0){Key_val=7;Key_state=1;Cont=7;S7_Press=1;}
			if(P31==0){Key_val=6;Key_state=1;}
			if(P32==0){Key_val=5;Key_state=1;}
			if(P33==0){Key_val=4;Key_state=1;}
			break;
		case 1:
			if(P30==1&&P31==1&&P32==1&&P33==1){Key_state=0;S7_Press=0;}
			break;
	}
}

uc KEY()
{
	uc KeyNum;
	BTN();
	
	KeyNum=Key_val;
	Key_val=0;
	return KeyNum;
}
