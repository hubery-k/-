#include <reg52.h>
#include <intrins.h>

sbit TX= P1^0;
sbit RX= P1^1;

unsigned char dspflag;
unsigned char cnt;
unsigned char tflag;
unsigned char code dsp_code_ca[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void display(unsigned int d);
void init_t(void);

void Pulse()
{
	unsigned char i;
	for (i =0 ;i<10; ++i) 
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
//		_nop_();		 //��ʱ,��������Ƶ��

		TX = ~TX;		 //P1_0 �����������
	}
}

unsigned int sonic()
{
	unsigned int n;
	RX=1;				//P1_1	���������նˣ��ڽ��յ��ź�ʱ����һ��������
	TR1=0;				//�رն�ʱ��1
	TH1=TL1=0;
	Pulse();
	TR1=1;			//�򿪶�ʱ��1	
	TF1=0;				 //��ʱ��1�жϽ���
	for(n=0;n<160;n++);			// �ų����ڸ���
	while( RX ==1 && TF1 == 0 ) ;
	TR1=0;
	if ( TF1 ) 
	{
		n=9999;
	}else
	{
		n=(TH1<<8)+TL1;
		n=(unsigned int) ( n*0.017);//0.018446);	   //   0.017*12/11.0592
	}
	return n;
}



void main()
{
  unsigned int k;
  unsigned int cnt;
  init_t();	
  while(1)
  {
  	cnt = sonic();
	  for(k=0;k<30000;k++)
		display(cnt);
  }	
}


void isr_t0(void) interrupt 1
{
  tflag++;
  if(tflag == 16)
  {
	tflag = 0;
	dspflag ++;
	if(dspflag == 3)
	  dspflag = 0;
  }
}

void init_t(void)
{	
  TMOD = 0x12;	 // �����ö�ʱ��1
  TH0 = (25536-50)/256;      //0x06;
  TL0 = (25536-50)%256;      //  0x06;
  ET0 = 1;
  EA = 1;
  TR0 = 1;		  //����TF0�ж�
}


void display(unsigned int d)
{
  if((dspflag == 0)&&(d>99))
  {
    P0 = 0xFF;
	P2 |= 0xE0;		 //P2��1110 0000���롱������λ��0
	P2 &= 0x1F;		 //P2��0001 1111���򡱣�����λ��1

	P0 = dsp_code_ca[d/100];
	P2 |= 0xE0;
	P2 &= 0x1F;
	P0 = 0x20;
	P2 |= 0xC0;
	P2 &= 0x3F;
  }
  if((dspflag == 1)&&(d>9))
  {
	P0 = 0xFF;
    P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d%100/10];
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x40;
    P2 |= 0xC0;
    P2 &= 0x3F;
  } 
  if(dspflag == 2)
  {
    P0 = 0xFF;
	P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d%100%10]; 
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x80;
    P2 |= 0xC0;
    P2 &= 0x3F;
  }  
}