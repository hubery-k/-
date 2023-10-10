#include "main.h"

/***********变量定义*************/
uc KeyNum;
uc Dat_Interface;//数据子界面
uc Dat_Interface_Son;	//数据子子界面
uc Distance_P=10;		//距离参数范围（10~80）
uc Time_P=2;	//采集时间参数
uc TP_temp=2;	//采集时间临时变量
uc DP_temp=10;	//采集距离临时变量
uc Time[3]={0X01,0X20,0X20};
int Distance;
f adc;
ui Max_Distance=0;
ui Min_Distance=999;
f Ave_Distance;
ui Trg_Num;
uc D_Num;	//距离计次
uc temp=1;


bit temp_flag;
bit Interface;
bit P_Interface;
bit Mode;
bit CLOCK_Flag;
bit ADC_Flag;
bit SONIC_Flag;
bit KEY_flag;
bit C_Trg;	//触发模式触发采集
bit F_Trg;	//定时模式触发采集
bit State;	//亮、暗状态
bit led_run;
/***********硬件初始化函数*************/

void Hardware_Init()
{
	LED_Init();
	RB_Init();
	Timer0Init();
	DS1302_WriteTime(Time);
	Sonic_Init();
}

/***********任务处理函数*************/

void Task_Processing()
{
	if(CLOCK_Flag==1)
	{
		CLOCK_Flag=0;
		DS1302_ReadTime(Time);
		
		/*定时模式判断-秒值整除时间参数(由于1s可多次触发，所以加入限制条件)*/
		if(temp!=Time[0]/16*10+Time[0]%16){temp=Time[0]/16*10+Time[0]%16;temp_flag=1;}
		if(temp%Time_P==0&&temp_flag==1)
		{
			F_Trg=1;//触发一次
			temp_flag=0;
		}
	}
	if(ADC_Flag==1)
	{
		ADC_Flag=0;
		adc=(PCF8591_ReadAD(0)/255.0)*5.0;
		/*触发模式判断-亮到灭*/
		if(adc>2&&State==0)
		{
			State=1;
		}
		else if(adc<2&&State==1)
		{
			State=0;
			C_Trg=1;//触发一次
		}
	}
		
	if((Mode==0&&C_Trg==1) || (Mode==1&&F_Trg==1))
	{
		Trg_Num+=1;
		if(C_Trg==1){C_Trg=0;}
		if(F_Trg==1){F_Trg=0;}
		Distance=Sonic_Work();
		
		/*L5*/
		if(Mode==1)
		{
			if((Distance-Distance_P)>=0&&(Distance-Distance_P)<=5 || (Distance-Distance_P)>=-5&&(Distance-Distance_P)<=0)
			{D_Num+=1;}
			else{D_Num=0;}
		}
		/*最大值*/
		if(Distance>Max_Distance)
		{
			Max_Distance=Distance;
		}
		/*最小值*/
		if(Distance<Min_Distance)
		{
			Min_Distance=Distance;
		}
		/*平均值*/
		if(Trg_Num==1){Ave_Distance=Distance;}
		else{Ave_Distance=(((f)(Distance+Ave_Distance))/2);}
		/*DAC输出功能*/
		if(Distance<=10){PCF8591_WriteDA((1/5.0)*255);}
		else if(Distance>10&&Distance<80)
		{
			PCF8591_WriteDA(((Distance-10)*(4.0/70)+1)/5.0*255);
		}
		else if(Distance>=80){PCF8591_WriteDA(255);}
	}
}


void USE_LED()
{
	LED_Init();
	
	/*在数据界面下*/
	if(Interface==0)
	{
		if(Dat_Interface==0){L1=0;}
		else{L1=1;}
		if(Dat_Interface==1){L2=0;}
		else{L2=1;}
		if(Dat_Interface==2){L3=0;}
		else{L3=1;}
	}
	
	if(Mode==0){L4=0;L5=1;}
	else if(Mode==1)
	{
		L4=1;
		if(D_Num>=3){L5=0;}
		else{L5=1;}
	}
	if(State==1){L6=0;}
	else{L6=1;}
	P2&=0X1F;
}
/***********KEY应用函数*************/

void USE_KEY()
{
	if(KEY_flag==1)
	{
		KEY_flag=0;
		KeyNum=KEY();
		/*S4-界面按键*/
		if(KeyNum==4)
		{
			Interface=~Interface;
			
			Dat_Interface=0;
			P_Interface=0;
			Dat_Interface_Son=0;
			/*设置的参数，退出参数界面生效*/
			if(Interface==0)
			{
				Time_P=TP_temp;
				Distance_P=DP_temp;
			}
		}
		/*S5-切换按键*/
		if(KeyNum==5)
		{
			if(Interface==0)
			{
				Dat_Interface++;
				Dat_Interface%=3;
			}
			else if(Interface==1)
			{
				P_Interface=~P_Interface;
			}
		}
		/*S8-模式按键*/
		if(KeyNum==8)
		{
			if(Interface==0)
			{
				if(Dat_Interface==1)
				{
					Mode=~Mode;
				}
				else if(Dat_Interface==2)
				{
					Dat_Interface_Son++;
					Dat_Interface_Son%=3;
				}
			}
		}
		/*S9-调整按键*/
		if(KeyNum==9)
		{
			/*在参数设置界面下*/
			if(Interface==1)
			{
				/*在采集时间参数界面下*/
				if(P_Interface==0)
				{
					/*采集时间参数*/
					if(TP_temp==2){TP_temp+=1;}
					else if(TP_temp>=3&&TP_temp<9){TP_temp+=2;}
					else if(TP_temp==9){TP_temp=2;}
				}
				/*在距离参数界面下*/
				else
				{
					/*距离参数*/
					DP_temp+=10;
					if(DP_temp>80){DP_temp=10;}
				}
			}
		}
	}
}

/***********SMG应用函数*************/

void USE_SMG()
{
	/*数据显示界面*/
	if(Interface==0)
	{
		/*时间数据显示界面*/
		if(Dat_Interface==0)
		{
			Nixie_SetBuf(0,Nodot_SMG[Time[2]/16]);
			Nixie_SetBuf(1,Nodot_SMG[Time[2]%16]);
			Nixie_SetBuf(2,0xBF);
			Nixie_SetBuf(3,Nodot_SMG[Time[1]/16]);
			Nixie_SetBuf(4,Nodot_SMG[Time[1]%16]);
			Nixie_SetBuf(5,0xBF);
			Nixie_SetBuf(6,Nodot_SMG[Time[0]/16]);
			Nixie_SetBuf(7,Nodot_SMG[Time[0]%16]);
		}
		/*距离数据显示界面*/
		else if(Dat_Interface==1)
		{
			Nixie_SetBuf(0,0XC7);	//标识符-L
			if(Mode==0){Nixie_SetBuf(1,Nodot_SMG[12]);}//标识符-C
			else{Nixie_SetBuf(1,Nodot_SMG[15]);}//标识符-F
			Nixie_SetBuf(2,0xFF);
			Nixie_SetBuf(3,0xFF);
//			Nixie_SetBuf(4,0xFF);
			//测试用的（显示定时模式下相近距离参数+-5的触发次数）
			Nixie_SetBuf(4,Nodot_SMG[D_Num]);
			//用来测量AD值
//			Nixie_SetBuf(5,dot_SMG[((ui)(adc*100))/100]);
//			Nixie_SetBuf(6,Nodot_SMG[((ui)(adc*100))/10%10]);
//			Nixie_SetBuf(7,Nodot_SMG[((ui)(adc*100))%10]);
			
			if(Distance>99){Nixie_SetBuf(5,Nodot_SMG[Distance/100]);}
			else{Nixie_SetBuf(5,0xFF);}
			if(Distance>9){Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);}
			else{Nixie_SetBuf(6,0xFF);}
			
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
		/*数据记录显示界面*/
		else if(Dat_Interface==2)
		{
			/*最大值界面*/
			if(Dat_Interface_Son==0)
			{
				Nixie_SetBuf(0,0X89);	//标识符-H
				Nixie_SetBuf(1,~0X01);	//标识符上划线
				Nixie_SetBuf(2,0xFF);
				Nixie_SetBuf(3,0xFF);
				if(Max_Distance>999){Nixie_SetBuf(4,Nodot_SMG[Max_Distance/1000]);}
				else{Nixie_SetBuf(4,0xFF);}
				if(Max_Distance>99){Nixie_SetBuf(5,Nodot_SMG[Max_Distance/100%10]);}
				else{Nixie_SetBuf(5,0xFF);}
				if(Max_Distance>9){Nixie_SetBuf(6,Nodot_SMG[Max_Distance/10%10]);}
				else{Nixie_SetBuf(6,0xFF);}
				Nixie_SetBuf(7,Nodot_SMG[Max_Distance%10]);
			}
			/*平均值界面*/
			else if(Dat_Interface_Son==1)
			{
				Nixie_SetBuf(0,0X89);	//标识符-H
				Nixie_SetBuf(1,~0X40);	//标识符中划线
				Nixie_SetBuf(2,0xFF);
				Nixie_SetBuf(3,0xFF);
				if((Ave_Distance*10)>999){Nixie_SetBuf(4,Nodot_SMG[((ui)(Ave_Distance*10))/1000]);}
				else{Nixie_SetBuf(4,0xFF);}
				if((Ave_Distance*10)>99){Nixie_SetBuf(5,Nodot_SMG[((ui)(Ave_Distance*10))/100%10]);}
				else{Nixie_SetBuf(5,0XFF);}
				if((Ave_Distance*10)>9){Nixie_SetBuf(6,dot_SMG[((ui)(Ave_Distance*10))/10%10]);}
				else{Nixie_SetBuf(6,0XFF);}
				
				Nixie_SetBuf(7,Nodot_SMG[((ui)(Ave_Distance*10))%10]);
			}
			/*最小值界面*/
			else if(Dat_Interface_Son==2)
			{
				Nixie_SetBuf(0,0X89);	//标识符-H
				Nixie_SetBuf(1,~0X08);	//标识符下划线
				Nixie_SetBuf(2,0xFF);
				Nixie_SetBuf(3,0xFF);
				if(Min_Distance>999){Nixie_SetBuf(4,Nodot_SMG[Min_Distance/1000]);}
				else{Nixie_SetBuf(4,0xFF);}
				if(Min_Distance>99){Nixie_SetBuf(5,Nodot_SMG[Min_Distance/100%10]);}
				else{Nixie_SetBuf(5,0XFF);}
				if(Min_Distance>9){Nixie_SetBuf(6,Nodot_SMG[Min_Distance/10%10]);}
				else{Nixie_SetBuf(6,0XFF);}
				
				Nixie_SetBuf(7,Nodot_SMG[Min_Distance%10]);
			}
		}
	}
	/*参数设置界面*/
	else if(Interface==1)
	{
		/*采集时间设置界面*/
		if(P_Interface==0)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[1]);	//编号1
			Nixie_SetBuf(2,0xFF);
			Nixie_SetBuf(3,0xFF);
			Nixie_SetBuf(4,0xFF);
			Nixie_SetBuf(5,0xFF);
			Nixie_SetBuf(6,Nodot_SMG[TP_temp/10]);	//采集时间
			Nixie_SetBuf(7,Nodot_SMG[TP_temp%10]);
		}
		/*距离参数设置界面*/
		else if(P_Interface==1)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[2]);	//编号1
			Nixie_SetBuf(2,0xFF);
			Nixie_SetBuf(3,0xFF);
			Nixie_SetBuf(4,0xFF);
			Nixie_SetBuf(5,0xFF);
			Nixie_SetBuf(6,Nodot_SMG[DP_temp/10]);	//距离参数
			Nixie_SetBuf(7,Nodot_SMG[DP_temp%10]);
		}
	}
}

/***********主函数*************/

void main()
{
	Hardware_Init();
	while(1)
	{
		USE_SMG();
		USE_KEY();
		Task_Processing();
		if(led_run==1){led_run=0;USE_LED();}
	}
}

/***********中断函数*************/

void Timer0_Routine() interrupt 1
{
	static uc KEY_Count;
	static uc SMG_Count;
	static uc CLOCK_Count;
	static uc ADC_Count;
	
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
}