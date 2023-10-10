#include "KEY.h"

static uc KeyNum;
bit Stop;
uc Trg,Cont;
void BTN()
{
	uc Dat;
	Dat=P3^0XFF;
	Trg=Dat&(Dat^Cont);
	Cont=Dat;
}

void KEY_Loop()
{
	BTN();
	if(Trg&0X01){KeyNum=7;AUXR|=0x10;Stop=0;}	//S7���´򿪶�ʱ��2
	if(Trg&0X02){KeyNum=6;}
	if(Trg&0X04){KeyNum=5;}
	if(Trg&0X08){KeyNum=4;}
	
	if(Cont==0){AUXR&=~0x10;Stop=1;}		//�ɿ������رն�ʱ��2ֹͣ��ʱ
}

uc KEY()
{
	uc Temp=0;
	Temp=KeyNum;
	KeyNum=0;
	return Temp;
}