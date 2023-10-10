#include "main.h"

/*******定义变量*******/
uc Time[]={0X50,0X59,0X16};
uc KeyNum;
uc Data_Interface;	//数据子界面
uc P_Interface;	//参数子界面
f T;
f adc;
c Hour_P=17;	//小时参数(0~23)
c T_P=25;	//温度参数(0~99)
uc LP=4;	//指示灯参数(4~8)
c HP_temp=17;
c TP_temp=25;
uc LP_temp=4;

bit Interface;	//父界面切换（数据界面、参数界面）
bit Mode;	//（0-亮、1-暗）
bit CLOCK_Flag;		//时间采集标志
bit T_Flag;		//温度采集标志
bit ADC_Flag;	//AD采集标志
bit L1_Flag;	//L1亮灭标志
bit L2_Flag;	//L2亮灭标志
bit L3_Flag;	//L3亮灭标志
bit LP_Flag;	//(L4~L8)亮灭标志
bit KEY_flag;	//按键键值读取标志
bit led_run;

/*******硬件初始化函数*******/
void Hardware_Init()
{
	LED_Init();
	RB_Init();
	Timer0Init();
	DS1302_WriteTime(Time);
	DS18B20_ConvertT();
	Delay750ms();
}

/*******任务处理函数*******/

void Task_Processing()
{
	/*50ms读取一次时间数据*/
	uc Time_temp;
	if(CLOCK_Flag==1)
	{
		CLOCK_Flag=0;
		DS1302_ReadTime(Time);
		Time_temp=Time[2]/16*10+Time[2]%16;
		/*L1*/
		if((Time_temp>Hour_P && Hour_P<8) || (Time_temp>Hour_P && Hour_P>8))
		{
			//情况1：大于小时参数且小于8点。情况2：是大于小时参数且大于8点
			L1_Flag=1;
		}
		else{L1_Flag=0;}
	}
	/*50ms读取一次AD采集的数据*/
	if(ADC_Flag==1)
	{
		ADC_Flag=0;
		adc=PCF8591_ReadAD(0)*(5.0/255);
		if(adc<2.5)
		{
			Mode=1;	//暗状态
			LP_Flag=1;
		}
		else
		{
			Mode=0;	//亮状态
			LP_Flag=0;
		}
	}
	/*100ms读取一次温度数据*/
	if(T_Flag==1)
	{
		T_Flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		/*L2*/
		if(T < T_P){L2_Flag=1;}
		else{L2_Flag=0;}
	}
}

/*******使用LED函数*******/
void USE_LED()
{
	LED_Init();
	
	if(L1_Flag==1){L1=0;}
	else{L1=1;}
	if(L2_Flag==1){L2=0;}
	else{L2=1;}
	if(L3_Flag==1){L3=0;}
	else{L3=1;}
	if(LP_Flag==1)
	{
		if(LP==4){L4=0;}
		else{L4=1;}
		if(LP==5){L5=0;}
		else{L5=1;}
		if(LP==6){L6=0;}
		else{L6=1;}
		if(LP==7){L7=0;}
		else{L7=1;}
		if(LP==8){L8=0;}
		else{L8=1;}
	}
	else{L4=1;L5=1;L6=1;L7=1;L8=1;}
	
	P2&=0X1F;
}

/*******使用KEY函数*******/

void USE_KEY()
{
	if(KEY_flag==1)
	{
		KEY_flag=0;
		KeyNum=KEY();
		/*S4-界面切换按键*/
		if(KeyNum==4)
		{
			Interface=~Interface;
			Data_Interface=0;
			P_Interface=0;
			/*在退出参数界面后更新参数数值*/
			if(Interface==0)
			{
				Hour_P = HP_temp;
				T_P = TP_temp;
				LP = LP_temp;
			}
		}
		/*S5-子界面切换按键*/
		if(KeyNum==5)
		{
			/*数据界面下*/
			if(Interface==0)
			{
				Data_Interface++;
				Data_Interface%=3;
			}
			/*参数界面下*/
			else if(Interface==1)
			{
				P_Interface++;
				P_Interface%=3;
			}
		}
		/*S8-参数减按键*/
		if(KeyNum==8)
		{
			/*参数界面下*/
			if(Interface==1)
			{
				if(P_Interface==0){HP_temp-=1;}
				else if(P_Interface==1){TP_temp-=1;}
				else if(P_Interface==2){LP_temp-=1;}
			}
			if(Hour_P<0){Hour_P=23;}
			if(T_P<0){T_P=99;}
			if(LP<4){LP=8;}
		}
		/*S9-参数加按键*/
		if(KeyNum==9)
		{
			/*参数界面下*/
			if(Interface==1)
			{
				if(P_Interface==0){HP_temp+=1;}
				else if(P_Interface==1){TP_temp+=1;}
				else if(P_Interface==2){LP_temp+=1;}
			}
			if(Hour_P>23){Hour_P=0;}
			if(T_P>99){T_P=0;}
			if(LP>8){LP=4;}
		}
	}
}


/*******使用SMG函数*******/

void USE_SMG()
{
	/*数据界面*/
	if(Interface==0)
	{
		/*时间数据显示*/
		if(Data_Interface==0)
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
		/*温度数据显示*/
		else if(Data_Interface==1)
		{
			Nixie_SetBuf(0,Nodot_SMG[12]);
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,Nodot_SMG[((ui)(T*10))/100]);
			Nixie_SetBuf(6,dot_SMG[((ui)(T*10))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(T*10))%10]);
		}
		/*亮暗状态显示*/
		else if(Data_Interface==2)
		{
			Nixie_SetBuf(0,Nodot_SMG[14]);
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,dot_SMG[((ui)(adc*100))/100]);
			Nixie_SetBuf(3,Nodot_SMG[((ui)(adc*100))/10%10]);
			Nixie_SetBuf(4,Nodot_SMG[((ui)(adc*100))%10]);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,Nodot_SMG[Mode]);
		}
	}
	/*参数界面*/
	else if(Interface==1)
	{
		/*时间参数*/
		if(P_Interface==0)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[1]);	//编号-1
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[HP_temp/10]);	//小时参数(0~23)
			Nixie_SetBuf(7,Nodot_SMG[HP_temp%10]);
		}
		/*温度参数*/
		else if(P_Interface==1)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[2]);	//编号-2
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[TP_temp/10]);	//温度参数(0~99)
			Nixie_SetBuf(7,Nodot_SMG[TP_temp%10]);
		}
		/*指示灯参数*/
		else if(P_Interface==2)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[3]);	//编号-3
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);	
			Nixie_SetBuf(7,Nodot_SMG[LP_temp]);//指示灯（4-8）
		}
	}
}

/*******主函数*******/
void main()
{
	Hardware_Init();
	while(1)
	{
		Task_Processing();
		USE_SMG();
		USE_KEY();
		if(led_run==1){led_run=0;USE_LED();}
	}
}

/*******中断函数*******/
void Timer0_Routine() interrupt 1
{
	static uc KEY_Count;
	static uc SMG_Count;
	static uc CLOCK_Count;
	static uc ADC_Count;
	static uc T_Count;
	static ui Dim_Count;
	static ui Lignt_Count;
	
	led_run=1;
	KEY_Count++;
	if(KEY_Count==10)
	{
		KEY_Count=0;
		KEY_flag=1;
	}
	SMG_Count++;
	if(SMG_Count==2)
	{
		SMG_Count=0;
		Nixie_Loop();
	}
	CLOCK_Count++;
	if(CLOCK_Count==50)
	{
		CLOCK_Count=0;
		CLOCK_Flag=1;
	}
	ADC_Count++;
	if(ADC_Count==100)
	{
		ADC_Count=0;
		ADC_Flag=1;
	}
	T_Count++;
	if(T_Count==200)
	{
		T_Count=0;
		T_Flag=1;
	}
	/*L3*/
	if(Mode==1)	//暗->亮
	{
		if(L3_Flag==0)
		{
			Dim_Count++;
			if(Dim_Count==3000)
			{
				Dim_Count=0;
				L3_Flag=1;
			}
		}
		else{Lignt_Count=0;}
	}
	else	//亮->暗
	{
		if(L3_Flag==1)
		{
			Lignt_Count++;
			if(Lignt_Count==3000)
			{
				Lignt_Count=0;
				L3_Flag=0;
			}
		}
		else{Dim_Count=0;}
	}
}