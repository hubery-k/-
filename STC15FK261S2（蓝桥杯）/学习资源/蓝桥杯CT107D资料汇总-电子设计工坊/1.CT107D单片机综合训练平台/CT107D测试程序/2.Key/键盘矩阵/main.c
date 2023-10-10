#include "reg52.h"

unsigned char dspflag;
unsigned char keyflag;	//按键标志位
unsigned char tflag;   //标志位 是一个变量
unsigned char code dsp_code_ca[] = {0xc0,0xf9,0xa4,0xb0,
0x99,0x92,0x82,0xf8,0x80,0x90};

unsigned char keyscan();
void display(unsigned char d);

void main()
{
  unsigned char temp;

  TMOD = 0x02;
  TH0 = 0x06;
  TL0 = 0x06;
  EA = 1;
  ET0 = 1;
  TR0 = 1;

  while(1)
  {
    if(keyflag)	
	{
	  temp = keyscan();
	  keyflag = 0;
	}
	display(temp);  	
  }
}

//定时器0中断服务函数
void isr_t0(void) interrupt 1
{
  tflag++;
  if(tflag == 16) // 4ms
  {
	dspflag++;
	if(dspflag == 2)
	dspflag = 0;
  }
  if(tflag == 20) //10ms
  {
  	tflag = 0;
	keyflag = 1;
  }  
}

// 4×4键盘矩阵扫描函数
unsigned char keyscan(void)
{
  unsigned char keyvalue;
  
  P3 = 0x7F;   //S4 S5 S6 S7
  switch(P3)
  {
    case 0x7E: keyvalue = 0;break;
    case 0x7D: keyvalue = 4;break;
    case 0x7B: keyvalue = 8;break;
    case 0x77: keyvalue = 12;break;
    default: break;    
  }
  P3 = 0xBF;   //S8 S9 S10 S11
  switch(P3)
  {
    case 0xBE: keyvalue = 1;break;
    case 0xBD: keyvalue = 5;break;
    case 0xBB: keyvalue = 9;break;
    case 0xB7: keyvalue = 13;break;
    default: break;
  }  
  P3 = 0xDF; //S12 S13 S14 S15
  switch(P3)
  {
    case 0xDE: keyvalue = 2;break;
    case 0xDD: keyvalue = 6;break;
    case 0xDB: keyvalue = 10;break;
    case 0xD7: keyvalue = 14;break;
    default: break;
  }
  P3 = 0xEF; //S16 S17 S18 S19
  switch(P3)
  {
    case 0xEE: keyvalue = 3;break;
    case 0xED: keyvalue = 7;break;
    case 0xEB: keyvalue = 11;break;
    case 0xE7: keyvalue = 15;break;
    default: break;  
  }
  return keyvalue;
}

//数码管显示函数,显示两位
void display(unsigned char d)
{
  if((dspflag == 0) && (d/10 != 0))
  {
	P0 = 0xFF;
    P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d/10];
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x40;
    P2 |= 0xC0;
    P2 &= 0x3F;
  } 
  if(dspflag == 1)
  {
    P0 = 0xFF;
	P2 |= 0xE0;
    P2 &= 0x1F;

	P0 = dsp_code_ca[d%10]; 
    P2 |= 0xE0;
    P2 &= 0x1F;
    P0 = 0x80;
    P2 |= 0xC0;
    P2 &= 0x3F;
  }  
}