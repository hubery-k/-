#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "KEY.h"
#include "Timer.h"
#include "PCF8591.h"
#include "Sonic.h"

uc code Nodot_SMG[16]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[16]={~0xBF,~0x86,~0xDB,~0xCF,~0xE6,~0xED,~0xFD,~0x87,~0xFF,~0xEF,~0xF7,~0xFC,~0xB9,~0xDE,~0xF9,~0xF1};

uc KeyNum;
uc Interface;		//显示界面-变量	
f adc_v;
f V_P;
f V_Up=4.5;
f V_Low=0.5;
f rate;
ui Distance;

bit AD_flag;
bit Sel_P;	//参数选择-标志
bit Sonic_Start;
bit DA_flag;
bit Sonic_flag;
bit L8_flag;
void Sys_Init()
{
	LED_Init();
	RB_Init();
}


void USE_PCF8591()
{
	/*AD数据采集与转换*/
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=AD_Read()*(5.0/255);
			/*判断是否开启超声波测距*/
		if(Interface!=2)
		{
			if((adc_v>V_Low) && (adc_v<V_Up))
			{
				Sonic_Start=1;		//启动连续超声波测距功能
				DA_flag=1;
			}
			else{Sonic_Start=0;DA_flag=0;}
		}
		
			if(DA_flag==1)	//超声波测距开启
			{
				if(Distance<=20){DA_Write(1.0/5*255);}	//输出1V
				if(Distance>20 && Distance<80)	//线性输出1~5V
				{
					/*1.5CM-0.1V*/
					rate=(Distance-20)*(1.0/15);
					DA_Write((uc)(rate+1)/5.0*255);
				}
				if(Distance>=80){DA_Write(255);}	//输出5V
			}
			else if(DA_flag==0)	//超声波测距未开启
			{
				DA_Write(0);
			}
	}
	

	
	/*DA输出*/

}
void USE_Sonic()
{
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		if(Sonic_Start==1){Distance=Sonic();}
	}
}

void USE_LED()
{
	LED_Init();
	if(Interface==0){L1=0;L2=1;L3=1;}
	else if(Interface==1){L1=1;L2=0;L3=1;}
	else if(Interface==2){L1=1;L2=1;L3=0;}
	
	if(L8_flag==1){L8=0;}
	else{L8=1;}
	
	P2&=0X1F;
}
	
void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-界面切换按键
	{
		Interface++;
		if(Interface==3){Interface=0;}
	}
	if(KeyNum==5)		//S5-参数选择按键
	{
		Sel_P=~Sel_P;
	}
	if(Interface==2)		//在参数选择界面下
	{
		if(KeyNum==6)		//S6-参数加按键
		{
			if(Sel_P==0){V_Up+=0.5;}
			else{V_Low+=0.5;}
			
			if(V_Up>5.0){V_Up=0.5;}
			if(V_Low>5.0){V_Low=0.5;}
		}
		if(KeyNum==7)		//S7-参数减按键
		{
			if(Sel_P==0){V_Up-=0.5;}
			else{V_Low-=0.5;}
			
			if(V_Up<0.5){V_Up=5.0;}
			if(V_Low<0.5){V_Low=5.0;}
		}
	}
	
}
void USE_SMG()
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XC1);		//标识符-U
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[(ui)(adc_v*100)/100]);
		Nixie_SetBuf(6,Nodot_SMG[(ui)(adc_v*100)/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[(ui)(adc_v*100)%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XC3);		//标识符-L
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		if(Sonic_Start==1)
		{
			Nixie_SetBuf(5,Nodot_SMG[Distance/100]);
			Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
		else if(Sonic_Start==0)
		{
			Nixie_SetBuf(5,Nodot_SMG[10]);
			Nixie_SetBuf(6,Nodot_SMG[10]);
			Nixie_SetBuf(7,Nodot_SMG[10]);
		}

	}
	else if(Interface==2)
	{
		Nixie_SetBuf(0,0X8C);		//标识符-P
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,dot_SMG[(uc)(V_Up*10)/10]);
		Nixie_SetBuf(4,Nodot_SMG[(uc)(V_Up*10)%10]);
		Nixie_SetBuf(5,0XFF);
		Nixie_SetBuf(6,dot_SMG[(uc)(V_Low*10)/10]);
		Nixie_SetBuf(7,Nodot_SMG[(uc)(V_Low*10)%10]);
	}
}



void main()
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_Sonic();
		USE_PCF8591();
		USE_KEY();
		USE_SMG();
	}
}



void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count1;
	static uc T0_Count2;
	static uc Sonic_Count;
	static uc AD_Count;
	static uc L8_Count;
	
	
	T0_Count1++;T0_Count2++;
	if(T0_Count1==2)
	{
		T0_Count1=0;
		Nixie_Loop();
	}
	if(T0_Count2==10)
	{
		T0_Count2=0;
		KEY_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)
	{
		AD_Count=0;
		AD_flag=1;
	}
	
	Sonic_Count++;
	if(Sonic_Count==250)
	{
		Sonic_Count=0;
		Sonic_flag=1;
	}
	
	if(Sonic_Start==1)
	{
		L8_Count++;
		if(L8_Count==100)	//达到0.1s
		{
			L8_Count=0;
			L8_flag=~L8_flag;
		}
	}
	else{L8_flag=0;}

}