#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "Delay.h"
#include "DS18B20.h"
#include "CLOCK.h"
#include "KEY.h"
/*BCD转十进制：BCD/16*10+BCD%16	*/
/*十进制转BCD：DEC/10*16+DEC%10	（得出的结果是该BCD码以十进制形式表示。例如：20/10*16+20%10=32 二进制就是0010 0000 BCD码就是0X20）*/
unsigned char code Nodot_SMG[16]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};

unsigned char Time[3]={0X50,0X59,0X23};		//BCD格式
char setTime[3];
char setClock[3];
unsigned char T;
unsigned char KeyNum;
unsigned char setClock_State;
unsigned char setTime_State;
unsigned char Show_State;	
unsigned char T1_Count1;
unsigned char T1_Count2;
unsigned int T1_Count3;


bit interval;
bit LED_State;
bit T_flag;
bit C_flag;
bit Clock_flag;
bit TC_flag;
void Sys_Init(void)
{
	LED_Control(0X00);
	RB_Control(0X00);
}

void USE_DS18B20()
{
	if(T_flag==1)
	{
		T_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
	}
}

void USE_DS1302()
{
	if(C_flag==1)
	{
		C_flag=0;
		Read_Time(Time);
	}
	
	if(Time[0]==(setClock[0]/10)*16+(setClock[0]%10) && Time[1]==(setClock[1]/10)*16+(setClock[1]%10) && Time[2]==(setClock[2]/10)*16+(setClock[2]%10))
	{
		Clock_flag=1;//定时时间到
	}
}

void USE_Key(void)
{
	KeyNum=Key();
	
	if(KeyNum==7)		//S7-时钟按键
	{
		if(Show_State==0)
		{
			setTime_State++;
			if(setTime_State==4)
			{
				setTime_State=0;
				if(setTime[0]!=0||setTime[1]!=0||setTime[2]!=0)//当不设置时间
				{
					Time[0]=(setTime[0]/10)*16+(setTime[0]%10);		//十进制转BCD码（Dec/10*16+Dec%10）
					Time[1]=(setTime[1]/10)*16+(setTime[1]%10);
					Time[2]=(setTime[2]/10)*16+(setTime[2]%10);
					Write_Time(Time);
				}
			}
		}
		T1_Count1=0;		//每一次按下都计数清0，为了看到的第一眼显示
		Show_State=0;
		setClock_State=0;		//在时钟设置状态下闹钟设置页面为时钟显示
		
		if(Clock_flag==1){Clock_flag=0;LED_State=0;T1_Count3=0;}//闹钟提示状态下任意键关闭闹钟状态
	}
	if(KeyNum==6)		//S6-闹钟设置按键
	{
		if(Show_State==1)
		{
			setClock_State++;
			if(setClock_State==4){setClock_State=0;}
		}
		Show_State=1;
		setTime_State=0;	//在闹钟设置状态下时钟设置页面为时钟显示
		
		if(Clock_flag==1){Clock_flag=0;LED_State=0;T1_Count3=0;}//闹钟提示状态下任意键关闭闹钟状态
	}
	if(KeyNum==5)		//S5-加功能按键
	{
		if(setClock_State==1){setClock[2]+=1;}
		else if(setClock_State==2){setClock[1]+=1;}
		else if(setClock_State==3){setClock[0]+=1;}
		
		if(setTime_State==1){setTime[2]+=1;}
		else if(setTime_State==2){setTime[1]+=1;}
		else if(setTime_State==3){setTime[0]+=1;}
		
		/*阈值范围*/
		if(setClock[2]>23){setClock[2]=0;}
		if(setClock[1]>59){setClock[1]=0;}
		if(setClock[0]>59){setClock[0]=0;}
		
		if(setTime[2]>23){setTime[2]=0;}
		if(setTime[1]>59){setTime[1]=0;}
		if(setTime[0]>59){setTime[0]=0;}
		
		if(Clock_flag==1){Clock_flag=0;LED_State=0;T1_Count3=0;}//闹钟提示状态下任意键关闭闹钟状态
	}
	if(KeyNum==4)		//S4-减功能和切换显示功能
	{
		if(setClock_State==1){setClock[2]-=1;}
		else if(setClock_State==2){setClock[1]-=1;}
		else if(setClock_State==3){setClock[0]-=1;}
		
		if(setTime_State==1){setTime[2]-=1;}
		else if(setTime_State==2){setTime[1]-=1;}
		else if(setTime_State==3){setTime[0]-=1;}
		
		
		/*阈值范围*/
		if(setClock[2]<0){setClock[2]=23;}
		if(setClock[1]<0){setClock[1]=59;}
		if(setClock[0]<0){setClock[0]=59;}
		
		if(setTime[2]<0){setTime[2]=23;}
		if(setTime[1]<0){setTime[1]=59;}
		if(setTime[0]<0){setTime[0]=59;}
		
		if(Clock_flag==1){Clock_flag=0;LED_State=0;T1_Count3=0;}//闹钟提示状态下任意键关闭闹钟状态
	}
	
	if(setTime_State==0&&setClock_State==0)
	{
		if(Cont&0X08)//按下没松开
		{
			TC_flag=1;
		}
		else{TC_flag=0;}	//松开返回
	}
}

void USE_SMG()
{
	if(TC_flag==0)
	{
		if(Show_State==0)
		{
			if(setTime_State==0&&setClock_State==0)				//时钟显示
			{
				Nixie_SetBuf(0,Nodot_SMG[Time[2]/16]);
				Nixie_SetBuf(1,Nodot_SMG[Time[2]%16]);
				Nixie_SetBuf(2,0XBF);
				Nixie_SetBuf(3,Nodot_SMG[Time[1]/16]);
				Nixie_SetBuf(4,Nodot_SMG[Time[1]%16]);
				Nixie_SetBuf(5,0XBF);
				Nixie_SetBuf(6,Nodot_SMG[Time[0]/16]);
				Nixie_SetBuf(7,Nodot_SMG[Time[0]%16]);
			}
			else if(setTime_State==1)		//时钟设置-时设置
			{
				if(interval==0)
				{
					Nixie_SetBuf(0,Nodot_SMG[setTime[2]/10]);
					Nixie_SetBuf(1,Nodot_SMG[setTime[2]%10]);
				}
				else
				{
					Nixie_SetBuf(0,0XFF);
					Nixie_SetBuf(1,0XFF);
				}
				
				Nixie_SetBuf(2,0XFF);
				Nixie_SetBuf(3,0XFF);
				Nixie_SetBuf(4,0XFF);
				Nixie_SetBuf(5,0XFF);
				Nixie_SetBuf(6,0XFF);
				Nixie_SetBuf(7,0XFF);
			}
			else if(setTime_State==2)				//时钟设置-分设置
			{
				if(interval==0)
				{
					Nixie_SetBuf(3,Nodot_SMG[setTime[1]/10]);
					Nixie_SetBuf(4,Nodot_SMG[setTime[1]%10]);
				}
				else
				{
					Nixie_SetBuf(3,0XFF);
					Nixie_SetBuf(4,0XFF);
				}
				
				Nixie_SetBuf(0,0XFF);
				Nixie_SetBuf(1,0XFF);
				Nixie_SetBuf(2,0XFF);
				Nixie_SetBuf(5,0XFF);
				Nixie_SetBuf(6,0XFF);
				Nixie_SetBuf(7,0XFF);
			}
			else if(setTime_State==3)				//时钟设置-秒设置
			{
				if(interval==0)
				{
					Nixie_SetBuf(6,Nodot_SMG[setTime[0]/10]);
					Nixie_SetBuf(7,Nodot_SMG[setTime[0]%10]);
				}
				else
				{
					Nixie_SetBuf(6,0XFF);
					Nixie_SetBuf(7,0XFF);
				}
				
				Nixie_SetBuf(0,0XFF);
				Nixie_SetBuf(1,0XFF);
				Nixie_SetBuf(2,0XFF);
				Nixie_SetBuf(3,0XFF);
				Nixie_SetBuf(4,0XFF);
				Nixie_SetBuf(5,0XFF);
			}
		}
		else if(Show_State==1)		//闹钟显示
		{
			if(setTime_State==0&&setClock_State==0)
			{
				Nixie_SetBuf(0,Nodot_SMG[Time[2]/16]);
				Nixie_SetBuf(1,Nodot_SMG[Time[2]%16]);
				Nixie_SetBuf(2,0XBF);
				Nixie_SetBuf(3,Nodot_SMG[Time[1]/16]);
				Nixie_SetBuf(4,Nodot_SMG[Time[1]%16]);
				Nixie_SetBuf(5,0XBF);
				Nixie_SetBuf(6,Nodot_SMG[Time[0]/16]);
				Nixie_SetBuf(7,Nodot_SMG[Time[0]%16]);
			}
			else if(setClock_State==1)		//闹钟设置-时设置
			{
				Nixie_SetBuf(0,Nodot_SMG[setClock[2]/10]);
				Nixie_SetBuf(1,Nodot_SMG[setClock[2]%10]);
				
				Nixie_SetBuf(2,0XFF);
				Nixie_SetBuf(3,0XFF);
				Nixie_SetBuf(4,0XFF);
				Nixie_SetBuf(5,0XFF);
				Nixie_SetBuf(6,0XFF);
				Nixie_SetBuf(7,0XFF);
			}
			else if(setClock_State==2)		//闹钟设置-分设置
			{

				Nixie_SetBuf(3,Nodot_SMG[setClock[1]/10]);
				Nixie_SetBuf(4,Nodot_SMG[setClock[1]%10]);
				
				Nixie_SetBuf(0,0XFF);
				Nixie_SetBuf(1,0XFF);
				Nixie_SetBuf(2,0XFF);
				Nixie_SetBuf(5,0XFF);
				Nixie_SetBuf(6,0XFF);
				Nixie_SetBuf(7,0XFF);
			}
			else if(setClock_State==3)		//闹钟设置-秒设置
			{

				Nixie_SetBuf(6,Nodot_SMG[setClock[0]/10]);
				Nixie_SetBuf(7,Nodot_SMG[setClock[0]%10]);
				
				Nixie_SetBuf(0,0XFF);
				Nixie_SetBuf(1,0XFF);
				Nixie_SetBuf(2,0XFF);
				Nixie_SetBuf(3,0XFF);
				Nixie_SetBuf(4,0XFF);
				Nixie_SetBuf(5,0XFF);
			}
		}
	}

	else if(TC_flag==1)
	{
		Nixie_SetBuf(5,Nodot_SMG[T/10]);
		Nixie_SetBuf(6,Nodot_SMG[T%10]);
		Nixie_SetBuf(7,Nodot_SMG[12]);
		
		Nixie_SetBuf(0,0XFF);
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
	}

}

void USE_LED()
{
	if(LED_State==1){LED_Control(0X01);}
	else{LED_Control(0X00);}
}

/*1ms的中断定时函数*/
void Timer0_Routine(void) interrupt 1
{
	static unsigned char T0_Count1;
	static unsigned char T0_Count2;
	static unsigned char T_Count;
	static unsigned char C_Count;
	T0_Count1++;
	T0_Count2++;
	if(T0_Count1==2)
	{
		Nixie_Loop();
		T0_Count1=0;
	}
	if(T0_Count2==10)
	{
		Key_Loop();
		T0_Count2=0;
	}
	
	T_Count++;
	if(T_Count==100)
	{
		T_Count=0;
		T_flag=1;
	}
	C_Count++;
	if(C_Count==50)
	{
		C_Count=0;
		C_flag=1;
	}
	
}
/*10ms的中断定时函数*/
void Timer1_Routine(void) interrupt 3
{
	T1_Count1++;
	
	if(T1_Count1==100)
	{
		T1_Count1=0;
		interval=~interval;
	}
	
	if(Clock_flag==1)
	{
		T1_Count2++;
		T1_Count3++;
		if(T1_Count2==20)		//达到0.2s
		{
			T1_Count2=0;
			LED_State=~LED_State;
		}
	}
	if(T1_Count3==500)	//达到5s
	{
		T1_Count3=0;
		Clock_flag=0;
		LED_State=0;
	}
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	Write_Time(Time);
	DS18B20_ConvertT();
	Delay1000ms();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_DS18B20();
		USE_DS1302();
		USE_Key();
		USE_SMG();
	}
}