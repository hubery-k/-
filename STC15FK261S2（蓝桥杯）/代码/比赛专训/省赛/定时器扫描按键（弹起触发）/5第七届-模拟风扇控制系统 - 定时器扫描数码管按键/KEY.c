#include "KEY.h"

/*���Խ��������붨ʱ����*/
static unsigned char KeyNum;
unsigned char Key_GetState()
{
	unsigned char Num=0;		//Num�Ǿֲ�����ÿһ�ε��ö��ḳ��ֵΪ0
	
	if(KEY7==0){Num=7;}
	if(KEY6==0){Num=6;}		//��������򷵻ؼ�ֵ���ɿ���return����0
	if(KEY5==0){Num=5;}
	if(KEY4==0){Num=4;}
	
	return Num;
}

void KEY_Loop(void)
{
	static unsigned char N_State,L_State;
	L_State=N_State;				//����״̬����
	N_State=Key_GetState();		//��ȡ��ǰ����״̬
	//����ϸ�ʱ��㰴�����£����ʱ���δ���£���������˲�䣬�Դ˱������������ּ��
	if(L_State==7 && N_State==0)		//��һ��״̬���£���һ��״̬�ɿ�
	{
		KeyNum=7;
	}
	if(L_State==6 && N_State==0)
	{
		KeyNum=6;
	}
	if(L_State==5 && N_State==0)
	{
		KeyNum=5;
	}
	if(L_State==4 && N_State==0)
	{
		KeyNum=4;
	}
}

unsigned char KEY(void)		
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//ÿһ�ε��ý������������״̬
	return Temp;
}