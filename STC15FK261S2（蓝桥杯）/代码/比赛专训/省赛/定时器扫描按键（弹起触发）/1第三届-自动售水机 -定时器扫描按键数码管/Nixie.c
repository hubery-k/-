#include "Nixie.h"

/*˼·��ͨ��������ʾ������ʵ�ֶ�ʱ��ɨ�������*/

//�������ʾ������
unsigned char Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};


void Nixie_Init(unsigned char n)
{
	P2&=0X1F;//����
	
	if(n==1)
	{
		P0=0x00;//com����0
		P2=(P2&0x1f)|0xC0;//com��
	}
	else if(n==2)
	{
		P0=0xFF;//�������0
		P2=(P2&0x1f)|0xE0;//�����
	}
}

void Nixie_Display(unsigned char pos,unsigned char value)
{
	Nixie_Init(2);		//����
	
	Nixie_Init(1);
	P0 |= 0x01 << pos;
	Nixie_Init(2);
	P0 &= value;//�͵�ƽ��Ч
}


/**
  * @brief  ������������������ж��е���
  * @param  ��
  * @retval ��
  */
void Nixie_Loop(void)
{
	static unsigned char i=0;
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	if(i>=8){i=0;}
}

/**
  * @brief  ������ʾ������
  * @param  Location Ҫ���õ�λ�ã���Χ��1~8
  * @param  Number Ҫ���õ����֣���Χ�������������Χ
  * @retval ��
  */
void Nixie_SetBuf(unsigned char Location,Number)
{
	Nixie_Buf[Location]=Number;
}
