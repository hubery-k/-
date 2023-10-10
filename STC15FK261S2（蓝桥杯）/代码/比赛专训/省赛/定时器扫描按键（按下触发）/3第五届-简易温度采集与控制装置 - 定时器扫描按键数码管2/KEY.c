#include "KEY.h"

#define set(x) P4=(x>>3|x>>4);P3=x
#define get() ((P4&0X10)<<3)|((P4&0X04)<<4)|(P3&0X3F)

unsigned char Trg,Cont;
static unsigned char KeyNum;

void KBD()
{
	unsigned char Dat;
	set(0X0F);
	Dat=get();
	set(0xF0);
	Dat=(Dat|get())^0XFF;
	Trg=Dat&(Dat^Cont);		//��֤����ֻ����һ��
	Cont=Dat;		//��ס���µ�״̬
}

void KEY_Loop(void)
{
	KBD();
	switch(Trg ^ 0xFF)
	{
		case 0x77:KeyNum=9;break;
		case 0x7b:KeyNum=6;break;
		case 0x7d:KeyNum=3;break;
		case 0x7e:KeyNum=0;break;
		
		case 0xb7:KeyNum=10;break;
		case 0xbb:KeyNum=7;break;
		case 0xbd:KeyNum=4;break;
		case 0xbe:KeyNum=1;break;
		
		case 0xd7:KeyNum=11;break;
		case 0xdb:KeyNum=8;break;
		case 0xdd:KeyNum=5;break;
		case 0xde:KeyNum=2;break;
	}
}

unsigned char KEY()
{
	unsigned char Temp;
	Temp=KeyNum;
	KeyNum=20;		//20û�ж�Ӧʹ�õļ�ֵ
	return Temp;
}

/*���д�����ͣ�
	��1��û�а������£�dat=0;Cont=0;Trg=0;
	��2����S7�������£�KEYPORT=0XFE;dat=0X01;Trg=0X01&(0X01^0X00)=0X01;Cont=0X01;
	��3����S7�����������£�Trg�ڵ�һ��ɨ�����ڵ���0X01���ڵڶ���ɨ�����ں�Trg=0�����ԭ״̬(Ϊ�˰��²���δ���);
				Cont=0X01Cont״̬û��������ʾ�����������¡�
	��4����S7�����ɿ���Trg=0X00(0X00^0X01)=0;Cont=0;
*/
