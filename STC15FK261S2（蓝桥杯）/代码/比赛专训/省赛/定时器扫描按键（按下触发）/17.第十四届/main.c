#include "main.h"

uc Time[3]={0X05,0X03,0X13};
uc Interface;//界面
uc Interface_son;	//回显界面下的子界面
bit Interface_TH;	//温湿度界面
bit Interface_flag;	//（1-是温湿度界面 0-不是温湿度界面）

uc KeyNum;	//键值
f T;	//温度数据
char T_P=30;//温度参数（范围：0~99）
uc T_Max;	//最大温度
f T_Ave;	//平均温度
f T_temp;	//温度临时变量（用来存储上一次读取的温度值）
uc H;	//湿度
uc H_Max;	//最大湿度
f H_Ave;	//平均湿度
f H_temp;	//湿度临时变量（用来存储上一次读取的湿度值）
f adc_v;	//通过AD读取的光敏值转换为（0~5V）电压
ui F;	//NE555测的频率
ui F_temp;	//频率临时变量

uc Trg_Count;		//触发次数
uc Trg_Time[3];		//触发时间
bit Trg_flag;	//触发标志位

bit CLOCK_flag;	//时间数据读取标志位
bit Temperature_flag;	//温度读取标志位
bit AD_flag;	//AD值读取标志位
bit Invalid_data;	//无效数据标志位
bit L4_flag;	//L4标志位
bit L4_blink;
bit L6_flag;	//L6标志位
bit S9_flag;	//按键S9标志位（按下-1 松开-0）
bit KEY_flag;

/*延时函数*/
void Delay750ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


/*硬件初始化函数*/
void Hardware_Init()
{
	LED_Init();
	RB_Init();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	DS1302_WriteTime(Time); 
	DS18B20_ConvertT();
	Delay750ms();
}


/*任务处理函数*/
void Task_Processing()
{
	//获取时间数据 （50ms采集一次时间值）
	if(CLOCK_flag==1)
	{
		CLOCK_flag=0;
		DS1302_ReadTime(Time);
	}
	
	//获取AD采集光敏数值（50ms采集一次电压值）
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=PCF8591_ReadAD(0)*(5.0/255);
	}
	
	//光->暗 触发一次温湿度数据采集功能
	if(Trg_flag==0 && adc_v>=1.0 && Interface_flag==0)	//亮状态(可以防止3s内重复触发)
	{
		Trg_flag=1;
		T_temp=T;
		H_temp=H;
	}
	else if(adc_v<1.0 && Trg_flag==1)	//暗状态
	{
		AUXR |= 0x10;	//打开定时器2计时3s
		Interface_TH=1;	//进入温湿度页面
		Trg_Time[1]=Time[1];	//保存触发采集的时间
		Trg_Time[2]=Time[2];
		Interface_flag=1;
		
		/*温度采集*/
		if(Temperature_flag==1)
		{
			Temperature_flag=0;
			DS18B20_ConvertT();
			T=DS18B20_ReadT();
			Trg_flag=0;	//只触发一次
			Trg_Count+=1;	//触发次数加1
			
			if(Trg_Count==1){T_Ave=(T+T_Ave);}	//平均温度计算
			else{T_Ave=(T+T_Ave)/2;}
			if(T_Max < T){T_Max=(uc)T;}	//最大温度计算
		}
		
		/*湿度采集*/
		if(F <= 200){H=10;Invalid_data=1;}
		else if(F >= 2000){H=90;Invalid_data=1;}
		else if(F<2000 && F>200)
		{
			Invalid_data=0;	//有效数据
			H=(uc)((F-200)*(80.0/1800))+10;
		}
		if(Trg_Count==1){H_Ave=(H+H_Ave);}	//平均湿度计算
		else{H_Ave=(H+H_Ave)/2;}
		if(H_Max < H){H_Max=H;}	//最大湿度计算
		
		/*L4状态判断*/
		if(T>T_P){L4_flag=1;}	
		else{L4_flag=0;}
		
		/*L6状态判断*/
		if(Trg_Count==1){T_temp=T;H_temp=H;}	//上一个状态
		else if(Trg_Count>=2)	//下一个状态
		{
			if(T > T_temp && H > H_temp){L6_flag=1;}
			else{L6_flag=0;}
			T_temp=T;
			H_temp=H;
		}	
	}
}

void USE_KEY()
{
	if(KEY_flag==1)
	{
		KEY_flag=0;
		
		KeyNum=KEY();
		if(KeyNum==4)
		{
			Interface++;
			Interface_son=0;
			Interface%=3;
		}
		if(KeyNum==5)
		{
			if(Interface==1)
			{
				Interface_son++;
				Interface_son%=3;
			}
		}
		if(Interface==2)
		{
			if(KeyNum==8){T_P+=1;}
			if(KeyNum==9){T_P-=1;}
			if(T_P>99){T_P=0;}
			if(T_P<0){T_P=99;}
		}
	}
//	KeyNum=KEY();
//	if(KeyNum==4)
//	{
//		Interface++;
//		Interface_son=0;
//		Interface%=3;
//	}
//	if(KeyNum==5)
//	{
//		if(Interface==1)
//		{
//			Interface_son++;
//			Interface_son%=3;
//		}
//	}
//	
//	if(Interface==2)
//	{
//		if(KeyNum==8){T_P+=1;}
//		if(KeyNum==9){T_P-=1;}
//		if(T_P>99){T_P=0;}
//		if(T_P<0){T_P=99;}
//	}
//	/*长按功能*/
//	if(Interface==1)	
//	{
//		if(Interface_son==2)
//		{
//			if(Cont&0X44){S9_flag=1;}
//			else{S9_flag=0;}
//		}
//	}
}

void USE_LED()
{
	LED_Init();
	/*界面指示灯*/
	if(Interface==0 && Interface_flag==0){L1=0;}	//时间界面下
	else{L1=1;}
	if(Interface==1 && Interface_flag==0){L2=0;}	//回显界面下
	else{L2=1;}
	if(Interface_TH==1){L3=0;}	//温湿度界面
	else{L3=1;}
	
	/*报警指示灯*/
	if(L4_blink==1){L4=0;}
	else{L4=1;}
	if(Invalid_data==1){L5=0;}
	else{L5=1;}
	if(L6_flag==1){L6=0;}
	else{L6=1;}

	P2&=0X1F;
}

void USE_SMG()
{
	if(Interface_TH==1)	//温湿度界面
	{
		Nixie_SetBuf(0,Nodot_SMG[14]);//标识符-E
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,Nodot_SMG[(ui)T_Ave/10]);	//温度
		Nixie_SetBuf(4,Nodot_SMG[(ui)T_Ave%10]);
		Nixie_SetBuf(5,0XBF);
		if(Invalid_data==0)
		{
			Nixie_SetBuf(6,Nodot_SMG[H/10]);	//湿度
			Nixie_SetBuf(7,Nodot_SMG[H%10]);
		}
		else
		{
			Nixie_SetBuf(6,Nodot_SMG[10]);	//湿度
			Nixie_SetBuf(7,Nodot_SMG[10]);
		}
	}
	else
	{
		if(Interface==0)	//时间界面
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
		else if(Interface==1)	//回显界面
		{
			if(Interface_son==0)	//温度回显
			{
				Nixie_SetBuf(0,Nodot_SMG[12]);//标识符-C
				Nixie_SetBuf(1,0XFF);
				if(Trg_Count==0)
				{
					Nixie_SetBuf(2,0XFF);
					Nixie_SetBuf(3,0XFF);
					Nixie_SetBuf(4,0XFF);
					Nixie_SetBuf(5,0XFF);
					Nixie_SetBuf(6,0XFF);
					Nixie_SetBuf(7,0XFF);
				}
				else
				{
					Nixie_SetBuf(2,Nodot_SMG[T_Max/10]);	//最大温度
					Nixie_SetBuf(3,Nodot_SMG[T_Max%10]);
					Nixie_SetBuf(4,0XBF);
					Nixie_SetBuf(5,Nodot_SMG[((ui)(T_Ave*10))/100]);//平均温度
					Nixie_SetBuf(6,dot_SMG[((ui)(T_Ave*10))/10%10]);
					Nixie_SetBuf(7,Nodot_SMG[((ui)(T_Ave*10))%10]);
				}
			}
			else if(Interface_son==1)	//湿度回显
			{
				Nixie_SetBuf(0,0X89);//标识符-H
				Nixie_SetBuf(1,0XFF);
				if(Trg_Count==0)
				{
					Nixie_SetBuf(2,0XFF);
					Nixie_SetBuf(3,0XFF);
					Nixie_SetBuf(4,0XFF);
					Nixie_SetBuf(5,0XFF);
					Nixie_SetBuf(6,0XFF);
					Nixie_SetBuf(7,0XFF);
				}
				else
				{
					Nixie_SetBuf(2,Nodot_SMG[H_Max/10]);	//最大湿度
					Nixie_SetBuf(3,Nodot_SMG[H_Max%10]);
					Nixie_SetBuf(4,0XBF);
					Nixie_SetBuf(5,Nodot_SMG[((ui)(H_Ave*10))/100]);//平均湿度
					Nixie_SetBuf(6,dot_SMG[((ui)(H_Ave*10))/10%10]);
					Nixie_SetBuf(7,Nodot_SMG[((ui)(H_Ave*10))%10]);
				}
			}
			else if(Interface_son==2)	//时间回显
			{
				Nixie_SetBuf(0,Nodot_SMG[15]);//标识符-F
				Nixie_SetBuf(1,Nodot_SMG[Trg_Count/10]);		//触发次数
				Nixie_SetBuf(2,Nodot_SMG[Trg_Count%10]);
				if(Trg_Count==0)	//触发次数为0时不显示
				{
					Nixie_SetBuf(3,0XFF);
					Nixie_SetBuf(4,0XFF);
					Nixie_SetBuf(5,0XFF);
					Nixie_SetBuf(6,0XFF);
					Nixie_SetBuf(7,0XFF);
				}
				else
				{
					Nixie_SetBuf(3,Nodot_SMG[Trg_Time[2]/16]);
					Nixie_SetBuf(4,Nodot_SMG[Trg_Time[2]%16]);
					Nixie_SetBuf(5,0XBF);
					Nixie_SetBuf(6,Nodot_SMG[Trg_Time[1]/16]);
					Nixie_SetBuf(7,Nodot_SMG[Trg_Time[1]%16]);
				}
			}
		}
		else if(Interface==2)	//参数界面
		{
			Nixie_SetBuf(0,0X8C);//标识符-P
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[T_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[T_P%10]);
		}
	}
}


void main()
{
	Hardware_Init();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_SMG();
		USE_KEY();
		Task_Processing();
	}
}

void Timer0_Routine(void) interrupt 1
{
	F_temp++;
}

void Timer1_Routine(void) interrupt 3
{
	static uc KEY_Count;
	static uc SMG_Count;
	static uc CLOCK_Count;
	static uc Temperature_Count;
	static uc AD_Count;
	static ui F_Count;
	static uc L4_Count;
	static ui S9_Count;
	
	SMG_Count++;
	if(SMG_Count==2){SMG_Count=0;Nixie_Loop();}

	KEY_Count++;
	if(KEY_Count==10){KEY_Count=0;KEY_flag=1;}

	CLOCK_Count++;
	if(CLOCK_Count==50){CLOCK_Count=0;CLOCK_flag=1;}

	AD_Count++;
	if(AD_Count==50){AD_Count=0;AD_flag=1;}

	Temperature_Count++;
	if(Temperature_Count==100){Temperature_Count=0;Temperature_flag=1;}

	F_Count++;
	if(F_Count==1000)	//达到1s
	{
		F_Count=0;
		F=F_temp;
		F_temp=0;
	}
	
	if(L4_flag==1)
	{
		L4_Count++;
		if(L4_Count==100)	//达到0.1s
		{
			L4_Count=0;
			L4_blink=~L4_blink;
		}
	}
	else{L4_Count=0;L4_blink=0;}
	
	/*长按*/
	if(S9_flag==1)
	{
		S9_Count++;
		if(S9_Count==2000)	//达到2s
		{
			S9_Count=0;
			Trg_Count=0;
			H_Max=0;T_Max=0;
			H_Ave=0;T_Ave=0;
			T_temp=0;H_temp=0;
			Trg_flag=0;
			L4_flag=0;
			L6_flag=0;
			Invalid_data=0;
			Interface_flag=0;
		}
	}
	else
	{
		S9_Count=0;
	}
}

void Timer2_Routine(void) interrupt 12
{
	static ui T2_Count;
	T2_Count++;
	if(T2_Count==3000)	//达到3s
	{
		T2_Count=0;
		Interface_TH=0;	//退出温湿度页面
		Interface_flag=0;
		AUXR &= ~0x10;	//暂停计时
	}
}