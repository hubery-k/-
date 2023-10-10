/*
 * 问题描述：用提高篇里的那个矩阵键盘模板数码管不能正常显示，但用我注释里的那种键盘写法就可以正常显示。
 * Copyright (c) 2017 电子设计工坊 dianshe.taobao.com
 * All rights reserved
 */
#include <STC15F2K60S2.H>
#include<onewire.h>
#define u8 unsigned char
u8 code smg_du[] ={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00};
u8 code smg_wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
#define KEY P3
#define NO_KEY 0xff
#define KEY_STATE0 0
#define KEY_STATE1 1
#define KEY_STATE2 2
u8 mode=1;//mode 1 2 3
u8 work_display[8];
u8 S5_count;
u8 temp_display[8];
u8 temp_mode=0;
u8 temperature;
bit output_flag=1;
unsigned int work_time;
unsigned char Key_scan()
{
	static unsigned char key_state=KEY_STATE0;
	u8 key_value,key_temp;
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
		case KEY_STATE0:
			if(key_temp!=NO_KEY)
			{
				key_state=KEY_STATE1;
			}
			break;
		case KEY_STATE1:
			if(key_temp==NO_KEY)
			{
				key_state=KEY_STATE0;
			}
			else
			{
				switch(key_temp)
				{
					case 0x77: key_value=4;break;
					case 0x7b: key_value=5;break;
					case 0x7d: key_value=6;break;		
					case 0x7e: key_value=7;break;
					
					case 0xb7: key_value=8;break;
					case 0xbb: key_value=9;break;
					case 0xbd: key_value=10;break;
					case 0xbe: key_value=11;break;
					
					case 0xd7: key_value=12;break;
					case 0xdb: key_value=13;break;
					case 0xdd: key_value=14;break;
					case 0xde: key_value=15;break;
					
					case 0xe7: key_value=16;break;
					case 0xeb: key_value=17;break;
					case 0xed: key_value=18;break;
					case 0xee: key_value=19;break;
				}
				key_state=KEY_STATE2;
			}
			break;
			
			case KEY_STATE2:
				if(key_temp==NO_KEY)
				{
					key_state=KEY_STATE0;
				}
				break;
			}
	return key_value;
}
//unsigned char Key_scan()
//{
//	static u8 state=KEY_STATE0;
//	u8 temp,keyval=0;
//	P44=0;P42=1;P35=1;P34=1;
//	temp=P3;
//	temp&=0x0f;
//	switch(temp)
//	{
//		case 0x0e:keyval=7;break;
//		case 0x0d:keyval=6;break;
//		case 0x0b:keyval=5;break;
//		case 0x07:keyval=4;break;
//	}
//	P44=1;P42=0;P35=1;P34=1;
//	temp=P3;
//	temp&=0x0f;
//	switch(temp)
//	{
//		case 0x0e:keyval=11;break;
//		case 0x0d:keyval=10;break;
//		case 0x0b:keyval=9;break;
//		case 0x07:keyval=8;break;
//	}
//	P44=1;P42=1;P35=0;P34=1;
//	temp=P3;
//	temp&=0x0f;
//	switch(temp)
//	{
//		case 0x0e:keyval=15;break;
//		case 0x0d:keyval=14;break;
//		case 0x0b:keyval=13;break;
//		case 0x07:keyval=12;break;
//	}
//	P44=1;P42=1;P35=1;P34=0;
//	temp=P3;
//	temp&=0x0f;
//	switch(temp)
//	{
//		case 0x0e:keyval=19;break;
//		case 0x0d:keyval=18;break;
//		case 0x0b:keyval=17;break;
//		case 0x07:keyval=16;break;
//	}
//	switch(state)
//	{
//		case KEY_STATE0:
//			if(keyval!=0) state=KEY_STATE1;
//			return 0;
//		  break;
//		case KEY_STATE1:
//		  if(keyval!=0)
//			{
//				state=KEY_STATE2;
//			return keyval;
//			}
//			else
//			 state=KEY_STATE0;
//			break;
//		case KEY_STATE2:
//			if(keyval==0) state=KEY_STATE0;
//			return 0;
//		break;
//	}

//	return 0;
//}
void Timer0Init(void)		//100??@11.0592MHz
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TL0 = 0xAE;		//??????
	TH0 = 0xFB;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;
	ET0=1;
	EA=1;
}


sbit buzzer=P0^6;
sbit relay=P0^4;
bit key_flag;
void main()
{
	u8 key_val=NO_KEY;
	P2=0xa0;buzzer=0;P2=0X00;
	P2=0xa0;relay=0;P2=0x00;
	P2=0x80;P0=0xff;P2=0x00;
	Timer0Init();
	
	
	
		while(1)
		{   
			if(temp_mode)
			{				
			 temperature=(u8)rd_temperature_f();
			}
	
			temp_display[0]=0x40;
			temp_display[1]=smg_du[4];
			temp_display[2]=0x40;
			temp_display[3]=0x00;
			temp_display[4]=0x00;
			temp_display[5]=smg_du[temperature/10];
			temp_display[6]=smg_du[temperature%10];
			temp_display[7]=0x39;
		if(work_time==0)
				output_flag=0;
			else
				output_flag=1;
			work_display[0]=0x40;
			work_display[1]=smg_du[mode];
			work_display[2]=0x40;
			work_display[3]=0x00;
			work_display[4]=smg_du[0];
			work_display[5]=smg_du[work_time/100];
			work_display[6]=smg_du[work_time/10%10];
			work_display[7]=smg_du[work_time%10];
			if(key_flag)
			{
				key_flag=0;
				key_val=Key_scan();
				switch(key_val)
				{
					case 4: 
						mode++;
					  if(mode==4)
						{
							mode=1;
						}
					  break;
				  case 5:
						 S5_count++;
					   if(S5_count==3)
						 {
							 work_time=0 ;
							 S5_count=0;
						 }
					   if(S5_count==1)
							 work_time=60;
						 if(S5_count==2)
							 work_time=120;
						 
						 break;
					case 6:
						work_time=0;
					  output_flag=0;
					break;
					case 7:
						temp_mode++;
					if(temp_mode==2)
						temp_mode=0;
					break;
				}
			}
		}
}
	


void timer() interrupt 1 using 1
{
	static int key_count=0,smg_count=0,i=0,pwm_count=0,time_count=0;
	key_count++;smg_count++;pwm_count++;time_count++;
	if(time_count==10000)
	{
		time_count=0;
		if(work_time>0)
		{
			work_time--;
		}
	}
	if(key_count==100)
	{
		key_count=0;key_flag=1;
	}
	if(smg_count==30)
	{
		smg_count=0;
		P2=0Xc0;P0=0;P2=0;
		if(temp_mode==0)
		{
		P2=0Xe0;P0=~work_display[i];P2=0x00;
		}
		if(temp_mode==1)
			
		{
			P2=0Xe0;P0=~temp_display[i];P2=0x00;
		}
		
		P2=0Xc0;P0=smg_wei[i];P2=0x00;
		i++;
		if(i==8) i=0;
	}
	if(output_flag)
	{
			if(mode==1)
		 {
			
			if(pwm_count==8)
			{
				P34=1;
			}
			if(pwm_count==10)
			{
				P34=0;
				pwm_count=0;
			}
		 }
		 if(mode==2)
		 {
			
			if(pwm_count==7)
			{
				P34=1;
			}
			if(pwm_count==10)
			{
				P34=0;
				pwm_count=0;
			}
		 }
		 if(mode==3)
		 {
			
			if(pwm_count==3)
			{
				P34=1;
			}
			if(pwm_count==10)
			{
				P34=0;
				pwm_count=0;
			}
	   }
		 else
		 {P34=0;
		 pwm_count=0;
		 }
   }
}

		