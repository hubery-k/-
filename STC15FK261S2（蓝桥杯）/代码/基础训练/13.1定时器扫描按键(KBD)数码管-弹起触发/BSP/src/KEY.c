#include "KEY.h"
#include "Delay.h"

static unsigned char KeyNum;

/**
  * @brief  ��ȡ��������
  * @param  ��
  * @retval ���°����ļ��룬��Χ��0,1~4,	0��ʾ�ް�������
  */
unsigned char Key(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}

/**
  * @brief  ��ȡ��ǰ������״̬�������������ּ��
  * @param  ��
  * @retval ���°����ļ��룬��Χ��0,1~4,	0��ʾ�ް�������
  */
unsigned char Key_GetState()
{
	unsigned char Num=0;
	
	if(P30==0){Num=1;}
	if(P31==0){Num=2;}
	if(P32==0){Num=3;}
	if(P33==0){Num=4;}
	
	return Num;
}

/**
  * @brief  �����������������ж��е���
  * @param  ��
  * @retval ��
  */
void Key_Loop(void)
{
	static unsigned char N_State,L_State;
	L_State=N_State;				//����״̬����
	N_State=Key_GetState();		//��ȡ��ǰ����״̬
	//����ϸ�ʱ��㰴�����£����ʱ���δ���£���������˲�䣬�Դ˱������������ּ��
	if(L_State==1 && N_State==0)
	{
		KeyNum=1;
	}
	if(L_State==2 && N_State==0)
	{
		KeyNum=2;
	}
	if(L_State==3 && N_State==0)
	{
		KeyNum=3;
	}
	if(L_State==4 && N_State==0)
	{
		KeyNum=4;
	}
}


