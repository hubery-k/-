#include "reg52.h"

sbit buzzer = P0^6;
sbit relay = P0^4;

void main(void)
{
	
	//�رշ�����
	buzzer = 0;	            //����Ч
	P2 = 0xA0;
	P2 = 0x00;

	//�رռ̵���
	relay = 0;              //����Ч
	P2 = 0xA0;
	P2 = 0x00;

	//�ر��������ʾ
	P0 = 0x00;              //λѡ�õ�
	P2 = 0xC0;
	P2 = 0x00;

	//�رշ����������ʾ
	P0 = 0xFF;              //����Ч
	P2 = 0x80;
	P2 = 0x00;
		
	while(1);
}