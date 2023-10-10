/*
 * 问题描述：记录温度应该有十个，但是只能记录前两个
 * 解决方案：第126行的数组定义问题，u8 temperature[2];应该定义成u8 temperature[10]; 因为要记录10个温度。
 * Copyright (c) 2018 电子设计工坊 dianshe.taobao.com
 * All rights reserved
 */
#include "STC15F2K60S2.h"
#include<DS1302.h>			
#include<onewire.h>
#include<intrins.h>

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
#define u8 unsigned char
u8 code smg_du[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
u8 code smg_wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; 

#define KEY P3
#define NO_KEY	 0xff//无按键按下
#define key_state_0 0//判断按键按下
#define key_state_1 1//抖动
#define key_state_2 2//释放  弹起
unsigned char Key_Scan()
{
  u8 key_value=0,key_temp;
  static u8 key_state;
  u8 key1,key2;

  P30=0;P31=0;P32=0;P33=0;P34=1;P35=1;P42=1;P44=1;
  if(P44==0) key1=0x70;
  if(P42==0) key1=0xb0;
  if(P35==0) key1=0xd0;
  if(P34==0) key1=0xe0;
  if((P34==1)&&(P35==1)&&(P42==1)&&(P44==1)) key1=0xf0;

  P30=1;P31=1;P32=1;P33=1;P34=0;P35=0;P42=0;P44=0;
  if(P30==0) key2=0x0e;
  if(P31==0) key2=0x0d;
  if(P32==0) key2=0x0b;
  if(P33==0) key2=0x07;
  if((P30==1)&&(P31==1)&&(P32==1)&&(P33==1)) key2=0x0f;
  key_temp=key1|key2;

  switch(key_state)
  {
    case key_state_0:
	if(key_temp!=NO_KEY)
	  key_state=key_state_1;
	  break;
	 
	 case key_state_1:
	 if(key_state==NO_KEY)
	 key_state=key_state_0;
	 else
	 {
	   switch(key_temp)
	   {
	    case 0x77:key_value=4;break;
		case 0x7b:key_value=5;break;
		case 0x7d:key_value=6;break;
		case 0x7e:key_value=7;break;

		case 0xb7:key_value=8;break;
		case 0xbb:key_value=9;break;
		case 0xbd:key_value=10;break;
		case 0xbe:key_value=11;break;

		case 0xd7:key_value=12;break;
		case 0xdb:key_value=13;break;
		case 0xdd:key_value=14;break;
		case 0xde:key_value=15;break;

		case 0xe7:key_value=16;break;
		case 0xeb:key_value=17;break;
		case 0xed:key_value=18;break;
		case 0xee:key_value=19;break;
	   }
	    key_state=key_state_2;
	 }
	  break;
     
	  case key_state_2:
	  if(key_temp==NO_KEY)
	  key_state=key_state_0;
	  break;
  }
  return key_value;
}
void Timer_Init(void)
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


u8 menu1[8],menu3[8];
u8 sample_time[]={1,5,30,60}; 
u8 sample_index;			  //切换的四个温度索引
u8 shi,fen,miao;
u8 menu_index;
u8 shijian[8];
u8 real_sample_time;
u8 display_count;

u8 temperature[10];
bit led_blink_flag;
bit key_flag;
bit temp_flag;
void main()
{
     u8 sample_countcount;
    //count = 0;                      //initial counter
	u8 key_val=NO_KEY;
	P2=0xa0;P0=0;P2=0;// close buzzer and relay
	P2=0x80;P0=0xff;P2=0;//close led
	set_sfm(23,59,55);
	Timer_Init();

	while(1)
	{
	  miao=Read_Ds1302(0x81);
	  fen=Read_Ds1302(0x83);
	  shi=Read_Ds1302(0x85);

	  shijian[0]=smg_du[shi/16];
	  shijian[1]=smg_du[shi%16];
	  shijian[3]=smg_du[fen/16];
	  shijian[4]=smg_du[fen%16];
	  shijian[6]=smg_du[miao/16];
	  shijian[7]=smg_du[miao%16];

	  menu1[0]=0x00;menu1[1]=0x00;menu1[2]=0x00;menu1[3]=0x00;menu1[4]=0x00;menu1[5]=0x40;
	  menu1[6]=smg_du[sample_time[sample_index]/10];menu1[7]=smg_du[sample_time[sample_index]%10];

		  if(key_flag)
		  {
		    key_flag=0;
			key_val=Key_Scan();
			switch(key_val)
			{
			  case 4:
			         if(menu_index==0)
					 {
					   sample_index++;
					   if(sample_index==4)
					    sample_index=0;
					 }
			         break;
			  case 5:
			  		 if(menu_index==0)
					 {
					    menu_index=1;  //进入时间显示界面
						real_sample_time=sample_time[sample_index];	 //  进入中断,满足开启采集温度的时间-->回到while ，执行温度采集--> 确定采样间隔
					 }
			         break;
			  case 6:
			         led_blink_flag=0;
					 P2=0x80;P0=0xff;P2=0x00;
					 display_count++;
					 if(display_count==10)
					 display_count=0;
			         break;
			  case 7:
			  		 menu_index=0;
					 sample_countcount=0;
			         break;
			  case 8:break;
			}
		  }
	   if(temp_flag)
	   {
	      temp_flag=0;
		  
		  if(sample_countcount<10)
		  {
		     ET0=0;
		     temperature[sample_countcount]= (u8)rd_temperature_f();
			 ET0=1;
			 sample_countcount++;
		  }
		  else
		  {	 
              
			  menu_index=2;
			  led_blink_flag=1;
		  }
	   }
	   if(menu_index==2)
	   {
	      menu3[0]=0x40;
		  menu3[1]=smg_du[display_count/10];
		  menu3[2]=smg_du[display_count%10];		//
		  menu3[3]=0x00;menu3[4]=0x00;menu3[5]=0x40;
		  menu3[6]=smg_du[temperature[display_count]/10];
		  menu3[7]=smg_du[temperature[display_count]%10];
	   }
	}							
}
bit blink_flag;

void tm0_isr() interrupt 1 using 1
{
     static int i=0 ,key_count=0,smg_count=0,blink_count=0,sample_count=0;
	key_count++;smg_count++;blink_count++;
	if(key_count==10)	//10ms
	{
	  key_count=0;
	  key_flag=1;
	}
	if(smg_count==3)		  //3ms
	{
	  smg_count=0;
	  P2=0xc0;P0=0;P2=0;//消影

	  if(menu_index==0)
	  {
	     P2=0xe0;P0=~menu1[i];P2=0;
	  }
	  if(menu_index==1)	//时间显示界面
	  {
	      P2=0xe0;P0=~shijian[i];P2=0;
	  }
	  if(menu_index==2)
	  {
	     P2=0xe0;P0=~menu3[i];P2=0;
	  }
	  P2=0xc0;P0=smg_wei[i];P2=0;
	  i++;
	  if(i==8)  i=0;
	}

	if(blink_count==1000)//1s
	{
	   blink_count=0;
	   blink_flag=~blink_flag;
	   if(blink_flag)
	   {
	      shijian[2]=0x40;
		  shijian[5]=0x40;
	   }
	   else
	   {
	      shijian[2]=0x00;
		  shijian[5]=0x00;
	   }

	   if(led_blink_flag)
	   {
	      if(blink_flag)
		  {
		    P2=0x80;P0=~0x01;P2=0;
		  }
		  else
		  {
		    P2=0x80;P0=0xff;P2=0;
		  }
	   }
	}

	if(menu_index==1)
	{
	  sample_count++;
	  if(sample_count==real_sample_time*1000)
	  {
	     sample_count=0;  
		 temp_flag=1;
	  }
	}
}



