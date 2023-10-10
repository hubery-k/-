#include "KEY.h"

static unsigned char KeyNum;
unsigned char Trg=0,Cont=0;
void BTN()
{
	unsigned char dat=P3^0xFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}



void KEY_Loop()
{
	BTN();
	if(Trg&0X01){KeyNum=7;}		//S7
	if(Trg&0X02){KeyNum=6;}		//S6
	if(Trg&0X04){KeyNum=5;}		//S5
	if(Trg&0X08){KeyNum=4;}		//S4
}


unsigned char KEY(void)
{
	unsigned char Temp=0;
	Temp=KeyNum;
	KeyNum=0;		//ÿһ�ε��ý������������״̬
	return Temp;
}
/*��ס��Key�����в�������return�����ؼ�ֵ�����磬����һ�ΰ���KeyNum=1����ô�´ε��øú���ʱ��ʹû�а���������Ȼ�᷵�ؼ�ֵKeyNum=1
	������ʹ��ȫ�ֱ������ؼ�ֵ*/