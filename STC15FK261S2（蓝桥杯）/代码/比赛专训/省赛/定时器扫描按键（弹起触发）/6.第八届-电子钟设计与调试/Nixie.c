#include "RB.h"

//���ݻ�������ͨ������ѭ�����������ݻ�����Ȼ���ڶ�ʱ���в���ɨ����ʾ
unsigned char Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

/*Com�˸ߵ�ƽ��ͨ����ѡ�͵�ƽ��ͨ*/
void Nixie_Init(unsigned char n)
{
	P2&=0X1F;
	if(n==1)		//Com��ѡ
	{
		P0=0X00;		//����ѡ��
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)		//��ѡ
	{
		P0=0XFF;		//��������
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,value)
{
	Nixie_Init(2);
	
	Nixie_Init(1);
	P0|=0x01<<pos;
	Nixie_Init(2);
	P0&=value;
	P2&=0X1F;
}

/*�����ѭ�����������ڶ�ʱ��ɨ��*/
void Nixie_Loop(void)
{
	static unsigned char i=0;		//�ֲ���������֮ǰ��Ҫ����ֵ
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	if(i>8){i=0;}
}

/*�������ݻ�����*/
void Nixie_SetBuf(unsigned char Location,Number)		
{
	Nixie_Buf[Location]=Number;
}