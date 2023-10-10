#include "KEY.h"

/*�߼����������·���Num����ֵ���ɿ�����Num=0����KEY_Loop�ж�������״̬������һ�����水��״̬��һ�������ɿ�״̬*/
static unsigned char KeyNum;

unsigned char KEY_GetState()
{
	unsigned char Num=0;
	if(S7==0){Num=7;TR1=1;}		//�������¿�ʼ��ʱ
	if(S6==0){Num=6;}
	if(S5==0){Num=5;}
	if(S4==0){Num=4;}
	
	return Num;
}

void KEY_Loop()
{
	static unsigned char N_State,L_State;
	L_State=N_State;
	N_State=KEY_GetState();
	if(L_State==4 && N_State==0){KeyNum=4;}
	if(L_State==5 && N_State==0){KeyNum=5;}
	if(L_State==6 && N_State==0){KeyNum=6;}
	if(L_State==7 && N_State==0){KeyNum=7;TR1=0;}		//�����ɿ�ֹͣ��ʱ
}

unsigned char KEY()
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}