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
//		_nop_();		 //延时,调整发射频率

		TX = ~TX;		 //P1_0 超声波发射端
	}
}

unsigned int sonic()
{
	unsigned int n;
	RX=1;				//P1_1	超声波接收端，在接收到信号时产生一个低脉冲
	TR1=0;				//关闭定时器1
	TH1=TL1=0;
	Pulse();
	TR1=1;			//打开定时器1	
	TF1=0;				 //定时器1中断结束
	for(n=0;n<160;n++);			// 排出相邻干扰
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
  TMOD = 0x12;	 // 超声用定时器1
  TH0 = (25536-50)/256;      //0x06;
  TL0 = (25536-50)%256;      //  0x06;
  ET0 = 1;
  EA = 1;
  TR0 = 1;		  //启用TF0中断
}


void display(unsigned int d)
{
  if((dspflag == 0)&&(d>99))
  {
    P0 = 0xFF;
	P2 |= 0xE0;		 //P2和1110 0000“与”，后五位清0
	P2 &= 0x1F;		 //P2和0001 1111“或”，后五位置1

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