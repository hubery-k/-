#include "KEY.h"

unsigned char Trg,Cont;

void KEY()
{
	unsigned char dat;
	dat=P3^0XFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}

/*���д�����ͣ�
	��1��û�а������£�dat=0;Cont=0;Trg=0;
	��2����S7�������£�KEYPORT=0XFE;dat=0X01;Trg=0X01&(0X01^0X01)=0X01;Cont=0X01;
	��3����S7�����������£�Trg�ڵ�һ��ɨ�����ڵ���0XFE���ڵڶ���ɨ�����ں�Trg=0�����ԭ״̬(Ϊ�˰��²���δ���);
				Cont=0X01Cont״̬û��������ʾ�����������¡�
	��4����S7�����ɿ���Trg=0X00(0X00^0X01)=0;Cont=0;
*/