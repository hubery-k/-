#include "KEY.h"
#include "Delay.h"

static unsigned char KeyNum;

/**
  * @brief  ��ȡ��������
  * @param  ��
  * @retval ���°����ļ��룬��Χ��0,1~4,	0��ʾ�ް�������
  */
	
unsigned char Trg=0,Cont=0;
void BTN()
{
	unsigned char dat=P3^0xFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}


