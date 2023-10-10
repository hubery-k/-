#include "KEY.h"

static uc KeyNum;

uc KEY_GetState()
{
	uc Num=0;
	
	P3|=0X3F;
	P4|=0X14;
	P32=0;
	if(P35==0){Num=13;}		//S13
	if(P34==0){Num=17;}		//S17
	
	P3|=0X3F;
	P4|=0X14;
	P33=0;
	if(P35==0){Num=12;}		//S12
	if(P34==0){Num=16;}		//S16
	
	return Num;
}

void KEY_Loop()
{
	static uc L_State,N_State;
	L_State=N_State;
	N_State=KEY_GetState();
	
	if(L_State==13 && N_State==0){KeyNum=13;}
	if(L_State==17 && N_State==0){KeyNum=17;}
	if(L_State==12 && N_State==0){KeyNum=12;}
	if(L_State==16 && N_State==0){KeyNum=16;}
}

uc KEY_Matrix()
{
	uc Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}