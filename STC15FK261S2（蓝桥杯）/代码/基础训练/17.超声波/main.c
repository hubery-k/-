#include "Sonic.h"
#include "Timer.h"
#include "Nixie.h"
#include "Delay.h"
unsigned char code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
unsigned int Distance;
bit S_flag;
void Timer1_Routine() interrupt 3
{
	static unsigned char T1_Count;
	static unsigned char S_Count;
	T1_Count++;
	if(T1_Count==2)
	{
		T1_Count=0;
		Nixie_Loop();
	}
	
	S_Count++;
	if(S_Count==200)		//200ms采集一次
	{
		S_Count=0;
		S_flag=1;
	}
}




void main()
{
	Timer0Init();
	Timer1Init();
	while(1)
	{
		if(S_flag==1)		
		{
			S_flag=0;
			Distance=Sonic();
		}
		Nixie_SetBuf(5,Nodot_SMG[Distance/100]);
		Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
	}
}
