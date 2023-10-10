#include "main.h"

/***********变量定义*************/
uc KeyNum;
uc Interface;
uc Interface_P;	//参数子界面

ui F_temp;
ui F;
ui F_P=15;	//频率参数（调整范围：1.0KHz - 12.0KHz）

f adc_v;
uc dac_v;

uc Humidity;
uc H_P=10;		//湿度参数（调整范围：10% - 60%）

ui Distance_CM;
ui D_P=1;		//距离参数（调整范围：0.1M - 1.2M）
bit F_Mode;
bit D_Mode;
bit led_run;
bit ADC_flag;
bit Sonic_flag;
bit F_flag;
bit H_flag;
bit D_flag;
bit RELAY_Flag;
bit S7_Flag;
bit Key_flag;
bit K7_Press;
bit PWM_State;
uc PWM;
uc PWM_J3=0X00;

uc RELAY_Count;

/***********硬件初始化函数*************/

void Hardware_Init(void)
{
	LED_Init();
	RB_Init();
	Timer0Init();
	Timer2Init();
	Sonic_Init();
}

/***********任务处理函数*************/

void Task_Processing()
{
	
	if(ADC_flag==1)
	{
		/*采集RB2电压值转换为湿度值*/
		ADC_flag=0;
		adc_v=PCF8591_ReadAD(1)*(5.0/255);
		Humidity=(uc)(adc_v*(100/5.0));
		
		/*DAC输出电压值*/
		if(Humidity<=H_P){dac_v=(uc)(1.0/5.0*255);}
		else if(Humidity>=80){dac_v=255;}
		else if(Humidity>H_P && Humidity<80)
		{
			dac_v=((uc)((Humidity-H_P)*(4.0/(80-H_P)))+1)/5.0*255;
		}
		PCF8591_WriteDA(dac_v);
	}
	/*采集超声波测距值*/
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance_CM=Sonic_Work();
		/*继电器控制功能*/
		if(Distance_CM > (D_P*10) && RELAY_Flag==0)
		{
			RB_Init();RELAY=1;P2&=0X1F;
			RELAY_Flag=1;
		}
		else if(Distance_CM <= (D_P*10) && RELAY_Flag==1)
		{
			RB_Init();RELAY=0;P2&=0X1F;
			RELAY_Flag=0;
			/*将继电器开关次数写入E2PROM中*/
			AT24C02_WriteByte(0X00,++RELAY_Count);
			Delay5ms();
		}
	}

	
	/*PWM输出脉冲信号*/
	if(F > F_P*100)
	{
		PWM_State=1;//输出1KHz80%占空比的信号
	}
	else
	{
		PWM_State=0;//输出1KHz20%占空比的信号
	}
}

/***********LED应用函数*************/

void USE_LED()
{
	LED_Init();
	
	if(F_flag==1){L1=0;}
	else{L1=1;}
	if(H_flag==1){L2=0;}
	else{L2=1;}
	if(D_flag==1){L3=0;}
	else{L3=1;}
	
	if(F > F_P*100){L4=0;}	//由于F_P与F单位不统一且F-P为了方便操作被默认放大10倍，所以F_P再乘以100由KHz转换为Hz
	else{L4=1;}
	if(Humidity > H_P){L5=0;}
	else{L5=1;}
	if(Distance_CM > D_P*10){L6=0;}	//由于Distance_CM与D_P单位不统一且D_P为了方便操作被默认放大10倍，所以D_P再乘以10由M转换为Cm
	else{L6=1;}
	
	L7=1;L8=1;
	P2&=0X1F;
}

/***********KEY应用函数*************/

void USE_KEY()
{
	if(Key_flag==1)
	{
		Key_flag=0;
		KeyNum=KEY();
		/*S4-界面切换按键*/
		if(KeyNum==4)
		{
			Interface++;
			Interface%=4;
		}
		/*S5-参数子界面切换按键*/
		if(KeyNum==5)
		{
			if(Interface==3)
			{
				Interface_P++;
				Interface_P%=3;
			}
		}
		/*S6-参数加按键*/
		if(KeyNum==6)
		{
			/*参数界面下*/
			if(Interface==3)
			{
				if(Interface_P==0){F_P+=5;}
				else if(Interface_P==1){H_P+=10;}
				else if(Interface_P==2){D_P+=1;}
				/*边界判断*/
				if(F_P > 120){F_P = 10;}
				if(H_P > 60){H_P = 10;}
				if(D_P > 12){D_P = 1;}
			}
			/*测距界面下*/
			if(Interface==2)
			{
				D_Mode=!D_Mode;
			}
		}
		/*S7-参数减按键*/
		if(KeyNum==7)
		{
			/*参数界面下*/
			if(Interface==3)
			{
				if(Interface_P==0){F_P-=5;}
				else if(Interface_P==1){H_P-=10;}
				else if(Interface_P==2){D_P-=1;}
				/*边界判断*/
				if(F_P < 10){F_P = 120;}
				if(H_P < 10){H_P = 60;}
				if(D_P < 1){D_P = 12;}
			}
			/*频率界面下*/
			if(Interface==0)
			{
				F_Mode=!F_Mode;
			}
		}
		
		/*S7-长按功能*/
		if(Interface==1)
		{
			if(Cont==7){K7_Press=1;}
			else{K7_Press=0;}
		}
	}

}


/***********SMG应用函数*************/

void USE_SMG()
{
	/*频率界面*/
	if(Interface==0)
	{
		Nixie_SetBuf(0,Nodot_SMG[15]);	//标识符-F
		Nixie_SetBuf(1,0XFF);
		/*Hz*/
		if(F_Mode==0)
		{
			if(F>99999){Nixie_SetBuf(2,Nodot_SMG[F/100000]);}
			else{Nixie_SetBuf(2,0XFF);}
			if(F>9999){Nixie_SetBuf(3,Nodot_SMG[F/10000%10]);}
			else{Nixie_SetBuf(3,0XFF);}
			if(F>999){Nixie_SetBuf(4,Nodot_SMG[F/1000%10]);}
			else{Nixie_SetBuf(4,0XFF);}
			if(F>99){Nixie_SetBuf(5,Nodot_SMG[F/100%10]);}
			else{Nixie_SetBuf(5,0XFF);}
			if(F>9){Nixie_SetBuf(6,Nodot_SMG[F/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[F%10]);
		}
		/*KHz*/
		else
		{
			if(F>99999){Nixie_SetBuf(2,Nodot_SMG[F/100000]);}
			else{Nixie_SetBuf(2,0XFF);}
			if(F>9999){Nixie_SetBuf(3,Nodot_SMG[F/10000%10]);}
			else{Nixie_SetBuf(3,0XFF);}
			Nixie_SetBuf(4,dot_SMG[F/1000%10]);
			Nixie_SetBuf(5,Nodot_SMG[F/100%10]);
			Nixie_SetBuf(6,Nodot_SMG[F/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[F%10]);
		}

	}
	/*湿度界面*/
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0X89);	//标识符-H
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XFF);
		Nixie_SetBuf(6,Nodot_SMG[Humidity/10]);
		Nixie_SetBuf(7,Nodot_SMG[Humidity%10]);
	}
	/*测距界面*/
	else if(Interface==2)
	{
		Nixie_SetBuf(0,Nodot_SMG[10]);	//标识符-A
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		/*cm*/
		if(D_Mode==0)
		{
			if(Distance_CM>99){Nixie_SetBuf(5,Nodot_SMG[Distance_CM/100]);}
			else{Nixie_SetBuf(5,0XFF);}
			if(Distance_CM>9){Nixie_SetBuf(6,Nodot_SMG[Distance_CM/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[Distance_CM%10]);
		}
		/*m*/
		else
		{
			Nixie_SetBuf(5,dot_SMG[Distance_CM/100]);
			Nixie_SetBuf(6,Nodot_SMG[Distance_CM/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[Distance_CM%10]);
		}
	}
	/*参数界面*/
	else if(Interface==3)
	{
		Nixie_SetBuf(0,0X8C);	//标识符-P
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		//测试用（继电器开关次数）
//		Nixie_SetBuf(3,Nodot_SMG[RELAY_Count/10]);
//		Nixie_SetBuf(4,Nodot_SMG[RELAY_Count%10]);
		/*P1-频率参数界面*/
		if(Interface_P==0)
		{
			Nixie_SetBuf(1,Nodot_SMG[1]);
			if(F_P>99){Nixie_SetBuf(5,Nodot_SMG[F_P/100]);}
			else{Nixie_SetBuf(5,0XFF);}
			if(F_P>9){Nixie_SetBuf(6,dot_SMG[F_P/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[F_P%10]);
		}
		/*P2-湿度参数界面*/
		else if(Interface_P==1)
		{
			Nixie_SetBuf(1,Nodot_SMG[2]);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[H_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[H_P%10]);
		}
		/*P3-距离参数界面*/
		else if(Interface_P==2)
		{
			Nixie_SetBuf(1,Nodot_SMG[3]);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,dot_SMG[D_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[D_P%10]);
		}
	}
}


/***********主函数*************/
void main()
{
	Hardware_Init();
	while(1)
	{
		if(led_run==1){led_run=0;USE_LED();}

		USE_SMG();
		USE_KEY();
		Task_Processing();
	}
}

/***********中断函数*************/
/*T0用作NE555*/
void Timer0_Routine(void) interrupt 1
{
	F_temp++;
}

/*T2用作扫描按键和数码管*/
void Timer2_Routine(void) interrupt 12
{
	static uc KEY_Count;
	static uc SMG_Count;
	static ui ADC_Count;
	static ui F_Count;
	static ui S7_Count;
	static ui Sonic_Count;
	static ui Interface_FCount;
	static ui Interface_HCount;
	static ui Interface_DCount;
	
	led_run=1;		//LED运行不受中断影响
	/*扫描按键和数码管*/
	KEY_Count++;
	if(KEY_Count==100)
	{
		KEY_Count=0;
		Key_flag=1;
//		KEY_Loop();
	}
	SMG_Count++;
	if(SMG_Count==20)
	{
		SMG_Count=0;
		Nixie_Loop();
	}
	/*100ms采集一次AD数据*/
	ADC_Count++;
	if(ADC_Count==1000)
	{
		ADC_Count=0;
		ADC_flag=1;
	}
	/*200ms采集一次测距值数据*/
	Sonic_Count++;
	if(Sonic_Count==2000)
	{
		Sonic_Count=0;
		Sonic_flag=1;
	}
	/*1s采集一次频率值*/
	F_Count++;
	if(F_Count==10000)
	{
		F_Count=0;
		F=F_temp;
		F_temp=0;
	}
	/*S7长按1s*/
	if(K7_Press==1)
	{
		S7_Count++;
		if(S7_Count==10000)
		{
			S7_Count=0;
			RELAY_Count=0;
		}
	}
	else{S7_Count=0;}
	/*L1以0.1s闪烁*/
	if(Interface==0)
	{
		Interface_FCount++;
		if(Interface_FCount==1000)
		{
			Interface_FCount=0;
			F_flag=~F_flag;
		}
	}
	else{F_flag=0;Interface_FCount=0;}
	/*L2以0.1s闪烁*/
	if(Interface==1)
	{
		Interface_HCount++;
		if(Interface_HCount==1000)
		{
			Interface_HCount=0;
			H_flag=~H_flag;
		}
	}
	else{H_flag=0;Interface_HCount=0;}
	/*L3以0.1s闪烁*/
	if(Interface==2)
	{
		Interface_DCount++;
		if(Interface_DCount==1000)
		{
			Interface_DCount=0;
			D_flag=~D_flag;
		}
	}
	else{D_flag=0;Interface_DCount=0;}
	
	/*PWM输出部分*/
	PWM++;
	if(PWM_State==1)
	{
		if(PWM>=8){PWM_J3|=0X20;}
		else{PWM_J3&=~0X20;}
	}
	else
	{
		if(PWM>=2){PWM_J3|=0X20;}
		else{PWM_J3&=~0X20;}
	}
	PWM%=10;
}