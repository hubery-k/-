#include "main.h"

bit Key_flag;
uc Time[3]={0X50,0X59,0X20};
uc KeyNum;
uc Interface;
uc Dat_Interface;
uc Rec_Interface;
uc P_Interface;


void Hardware_Init()
{
	LED_Init();
	RB_Init();
	Timer0Init();
	DS1302_WriteTime(Time);
	DS18B20_ConvertT();
	Delay750ms();
}

void Task_Processing()
{
	
}


void USE_KEY()
{
	if(Key_flag==1)
	{
		Key_flag=0;
		KeyNum=KEY();
		
		if(KeyNum==4)
		{
			Interface++;
			Interface%=3;
		}
		if(KeyNum==5)
		{
			if(Interface==0){Dat_Interface++;Dat_Interface%=5;}
			if(Interface==1){Rec_Interface++;Rec_Interface%=3;}
			if(Interface==2){P_Interface++;P_Interface%=3;}
		}
	}
}


void USE_SMG()
{
	if(Interface==0)
	{
		if(Dat_Interface==0)
		{
			Nixie_SetBuf(0,Nodot_SMG[Time[2]/16]);
			Nixie_SetBuf(1,Nodot_SMG[Time[2]%16]);
			Nixie_SetBuf(2,0XBF);	//间隔符
			Nixie_SetBuf(3,Nodot_SMG[Time[1]/16]);
			Nixie_SetBuf(4,Nodot_SMG[Time[1]%16]);
			Nixie_SetBuf(5,0XBF);	//间隔符
			Nixie_SetBuf(6,Nodot_SMG[Time[0]/16]);
			Nixie_SetBuf(7,Nodot_SMG[Time[0]%16]);
		}
		else if(Dat_Interface==1)
		{
			Nixie_SetBuf(0,Nodot_SMG[12]);	//标识符-C
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
		else if(Dat_Interface==2)
		{
			Nixie_SetBuf(0,Nodot_SMG[14]);	//标识符-E
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
		else if(Dat_Interface==3)
		{
			Nixie_SetBuf(0,Nodot_SMG[15]);	//标识符-F
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
		else if(Dat_Interface==4)
		{
			Nixie_SetBuf(0,0XC7);	//标识符-L
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
	}
	else if(Interface==1)
	{
		if(Rec_Interface==0)
		{
			Nixie_SetBuf(0,0X89);	//标识符-H
			Nixie_SetBuf(1,0XFE);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
		else if(Rec_Interface==1)
		{
			Nixie_SetBuf(0,0X89);	//标识符-H
			Nixie_SetBuf(1,0XBF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
		else if(Rec_Interface==2)
		{
			Nixie_SetBuf(0,0X89);	//标识符-H
			Nixie_SetBuf(1,0XF7);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
	}
	else if(Interface==2)
	{
		if(P_Interface==0)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[1]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
		else if(P_Interface==1)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[2]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
		else if(P_Interface==2)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[3]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
	}
}


void main()
{
	Hardware_Init();
	while(1)
	{
		USE_SMG();
		USE_KEY();
	}
}

void Timer0_Routine(void) interrupt 1
{
	static uc Key_count;
	static uc SMG_count;

	Key_count++;
	if(Key_count==10)
	{
		Key_count=0;
		Key_flag=1;
	}
	SMG_count++;
	if(SMG_count==2)
	{
		SMG_count=0;
		Nixie_Loop();
	}
}