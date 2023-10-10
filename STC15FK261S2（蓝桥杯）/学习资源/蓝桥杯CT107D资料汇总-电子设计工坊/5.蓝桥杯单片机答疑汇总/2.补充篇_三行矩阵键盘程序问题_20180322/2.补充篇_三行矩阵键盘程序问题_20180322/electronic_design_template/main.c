/*
 * 简介：三行按键-矩阵键盘
 * Copyright (c) 2017 电子设计工坊 dianshe.taobao.com
 * All rights reserved
 */
#include "STC15F2K60S2.h"

sbit buzzer = P0^4;
sbit relay = P0^6;

#define  u8		unsigned char
#define  u16	unsigned int

u8 code smg_du[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00}; //0-9 
u8 code smg_wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

#define SetKeyBoard(x) P4 = (x>>3) | (x>>4);P3 = x 
#define GetKeyBoard() ((P4&0x10)<<3) | ((P4&0x04)<<4) | (P3&0x3F)
unsigned char Trg; 
unsigned char Cont; 

/**
  * @brief  三行按键-矩阵键盘，提供简化版本，大家自行消化理解
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
void KeyRead( void ) 
{ 
    unsigned char ReadData; 
    SetKeyBoard(0x0f);
    ReadData=GetKeyBoard(); 
	SetKeyBoard(0xf0);
    ReadData=(ReadData | GetKeyBoard())^0xff; 
    Trg  = ReadData & (ReadData ^ Cont); 
    Cont = ReadData; 
} 

/**
  * @brief  按键处理函数
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
void KeyProc(void) 
{ 
    if (Trg)  // 如果有按下 
    {
		P2=0x80;P0=Trg^0xff;;P2=0x00;	
    }
	
	if (Trg==0x81)//S7
	{
		P2=0xa0;relay=0;buzzer=0;P2=0x00;
	}
	if (Trg==0x82)//S6
	{
		P2=0xa0;relay=1;buzzer=0;P2=0x00;
	}
	if (Trg==0x84)//S5
	{
		P2=0xa0;relay=0;buzzer=1;P2=0x00;
	}
	if (Trg==0x88)//S4
	{
		P2=0xa0;relay=1;buzzer=1;P2=0x00;
	}
	
    if (Cont)  // 如果按键被按着不放 
    { 
           
    }
	
    if (Trg ==0 & Cont==0)  //按键放开  
    { 
       
    } 
} 

void Timer0_Init(void)
{
	AUXR |= 0x80;	
	TMOD &= 0xF0;
	TL0 = 0xCD;		
	TH0 = 0xD4;		
	TF0 = 0;		
	TR0 = 1;		
	ET0 = 1;
	EA=1; 
}

bit key_flag;
void main() 
{ 
	P2=0x80;P0=0x00;P2=0x00;
	P2=0xa0;P0=0x00;P2=0x00;
    Trg=0; 
    Cont=0;
	Timer0_Init();					/*timer0 1ms interrupt*/
    while(1) 
    { 
		if(key_flag)
		{
			key_flag=0;
			KeyRead(); 
			KeyProc(); 
		}
    } 
}

void Timer0_Interrupt() interrupt 1  using 1                   
{
	static int key_count=0,smg_count=0,i=0;
	key_count++;smg_count++;
	if(key_count==10)				/*10ms interrupt*/
	{
		key_count=0;
		key_flag=1;
	}

	if(smg_count==3)				/*3ms interrupt*/
	{
		smg_count=0;
		P2=0xc0;P0=0;P2=0;			
		P2=0xe0;P0=~smg_du[i];P2=0;
		P2=0xc0;P0=smg_wei[i];P2=0;
		i++;
		if(i==8) i=0;
	}
}