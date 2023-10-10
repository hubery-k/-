#include "main.h"

/*变量定义区*/
uc Interface;
uc P_Interface;
uc KeyNum;
ui F_temp;
ui F;
ui Distance;
f adc_v;
uc Humidity;
uc Relay_count;
uc F_P=90;//频率参数
uc H_P=40;//湿度参数
uc D_P=6;//距离参数

bit Hz_Interface;//(0-Hz，1-KHz)
bit Cm_Interface;//(0-Cm，1-M)
bit Key_flag;
bit Sonic_flag;
bit ADC_flag;
bit Write_flag;
//bit Led_run;
bit L1_state;
bit L2_state;
bit L3_state;
bit L1_flag;
bit L2_flag;
bit L3_flag;
bit L5_flag;
bit L6_flag;
bit Relay_flag;
bit S7_flag;
bit PWM_flag;
bit PWM_state;

/*硬件初始化函数*/
void Hardware_Init()
{
	LED_Init();
	RB_Init();
	Timer0Init();
	Timer2Init();
	Sonic_Init();
	
//	Relay_count=AT24C02_ReadByte(0X00);
}

/*任务处理函数*/
void Task_Processing()
{
	/*超声波测距任务*/
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance=Sonic_Work();
		/*继电器控制功能任务*/
		if((Distance/10.0)>D_P && Relay_flag==0)	//单位统一
		{
			RB_Init();RELAY=1;P2&=0X1F;	//吸合
			Relay_flag=1;
			L6_flag=1;
		}
		else if((Distance/10.0)<=D_P && Relay_flag==1)
		{
			RB_Init();RELAY=0;P2&=0X1F;	//断开
			Relay_flag=0;
			L6_flag=0;
			Relay_count+=1;
			Write_flag=1;
		}
	}
	/*电压采集模拟湿度任务*/
	if(ADC_flag==1)
	{
		ADC_flag=0;
		adc_v=PCF8591_ReadAD(1)/255.0*5;
		Humidity=adc_v/5.0*100;
		/*L5*/
		if(Humidity>H_P){L5_flag=1;}
		else{L5_flag=0;}
		
		/*DAC输出功能任务*/
		if(Humidity<=H_P){PCF8591_WriteDA(1.0/5*255);}
		else if(Humidity>H_P&&Humidity<80)
		{
			PCF8591_WriteDA((((Humidity-H_P)*(4.0/(80-H_P)))+1)/5.0*255);
		}
		else if(Humidity>=80){PCF8591_WriteDA(255);}
	}
	/*EEPROM功能任务*/
	if(Write_flag==1)
	{
		Write_flag=0;
		AT24C02_WriteByte(0X00,Relay_count);
		Delay5ms();
	}
	
	/*脉冲输出功能任务*/
	if(F>(F_P*100)){PWM_flag=1;}
	else{PWM_flag=0;}
}

void USE_LED()
{
	if(Interface==0){L1_flag=1;L2_flag=0;L3_flag=0;L1_state=0;L2_state=0;L3_state=0;}
	else if(Interface==1){L1_flag=0;L2_flag=1;L3_flag=0;L1_state=0;L2_state=0;L3_state=0;}
	else if(Interface==2){L1_flag=0;L2_flag=0;L3_flag=1;L1_state=0;L2_state=0;L3_state=0;}
	else if(Interface==3)
	{
		if(P_Interface==0){L1_state=1;L2_state=0;L3_state=0;}//频率参数界面下
		else if(P_Interface==1){L1_state=0;L2_state=1;L3_state=0;}//湿度参数界面下
		else if(P_Interface==2){L1_state=0;L2_state=0;L3_state=1;}//距离参数界面下
	}
	
	LED_Init();
	/*界面指示灯*/
	if(L1_flag==1){L1=0;}
	else{L1=1;}
	if(L2_flag==1){L2=0;}
	else{L2=1;}
	if(L3_flag==1){L3=0;}
	else{L3=1;}
	
	
	/*测量指示灯*/
	if(F>(F_P*100)){L4=0;}
	else{L4=1;}
	if(L5_flag==1){L5=0;}
	else{L5=1;}
	if(L6_flag==1){L6=0;}
	else{L6=1;}
	L7=1;
	L8=1;
	
	P2&=0X1F;
}


/*应用按键函数*/
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
			
			P_Interface=0;
		}
		/*S5-参数界面切换按键*/
		if(KeyNum==5)
		{
			if(Interface==3)
			{
				P_Interface++;
				P_Interface%=3;
			}
		}
		/*S6-参数加按键*/
		if(KeyNum==6)
		{
			/*参数界面下*/
			if(Interface==3)
			{
				if(P_Interface==0){F_P+=5;}
				if(P_Interface==1){H_P+=10;}
				if(P_Interface==2){D_P+=1;}
				
				/*范围界定*/
				if(F_P>120){F_P=10;}
				if(H_P>60){H_P=10;}
				if(D_P>12){D_P=1;}
			}
			/*测距界面下*/
			if(Interface==2)
			{
				Cm_Interface=!Cm_Interface;
			}
		}
		/*S7-(短按)参数减按键*/
		if(KeyNum==7&&S7_flag==0)
		{
			/*参数界面下*/
			if(Interface==3)
			{
				if(P_Interface==0){F_P-=5;}
				if(P_Interface==1){H_P-=10;}
				if(P_Interface==2){D_P-=1;}
				
				/*范围界定*/
				if(F_P<10){F_P=120;}
				if(H_P<10){H_P=60;}
				if(D_P<1){D_P=12;}
			}
			/*频率界面下*/
			if(Interface==0)
			{
				Hz_Interface=!Hz_Interface;
			}
		}
		/*S7-(长按)继电器开关次数清零*/
		if(Cont==7&&S7_flag==1)
		{
			Cont=0;
			Relay_count=0;
			Write_flag=1;
		}
	}
}
/*应用数码管函数*/
void USE_SMG()
{
	/*频率界面*/
	if(Interface==0)
	{
			Nixie_SetBuf(0,Nodot_SMG[15]);
			Nixie_SetBuf(1,0XFF);
			if(F>99999){Nixie_SetBuf(2,Nodot_SMG[F/100000]);}
			else{Nixie_SetBuf(2,0XFF);}
			if(F>9999){Nixie_SetBuf(3,Nodot_SMG[F/10000%10]);}
			else{Nixie_SetBuf(3,0XFF);}
			if(F>999){Nixie_SetBuf(4,Nodot_SMG[F/1000%10]);}
			else{Nixie_SetBuf(4,0XFF);}
			if(F>99){Nixie_SetBuf(5,Nodot_SMG[F/100%10]);}
			else{Nixie_SetBuf(5,0XFF);}
		if(Hz_Interface==0)
		{
			if(F>9){Nixie_SetBuf(6,Nodot_SMG[F/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[F%10]);
		}
		else
		{
			if(F>9){Nixie_SetBuf(6,dot_SMG[F/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[F%10]);
		}
	}
	/*湿度界面*/
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0X89);
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
		Nixie_SetBuf(0,Nodot_SMG[10]);
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		/*测试：继电器吸合次数*/
//		Nixie_SetBuf(2,Nodot_SMG[Relay_count/10]);
//		Nixie_SetBuf(3,Nodot_SMG[Relay_count%10]);
		if(Cm_Interface==0)
		{
			if(Distance>99){Nixie_SetBuf(5,Nodot_SMG[Distance/100]);}
			else{Nixie_SetBuf(5,0XFF);}
			if(Distance>9){Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
		else
		{
			Nixie_SetBuf(5,dot_SMG[Distance/100]);
			Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
	}
	/*参数界面*/
	else if(Interface==3)
	{
		if(P_Interface==0)
		{
			Nixie_SetBuf(0,0X8C);
			Nixie_SetBuf(1,Nodot_SMG[1]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			
			if(F_P>99){Nixie_SetBuf(5,Nodot_SMG[F_P/100]);}
			else{Nixie_SetBuf(5,0XFF);}
			if(F_P>9){Nixie_SetBuf(6,dot_SMG[F_P/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[F_P%10]);
		}
		else if(P_Interface==1)
		{
			Nixie_SetBuf(0,0X8C);
			Nixie_SetBuf(1,Nodot_SMG[2]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[H_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[H_P%10]);
		}
		else if(P_Interface==2)
		{
			Nixie_SetBuf(0,0X8C);
			Nixie_SetBuf(1,Nodot_SMG[3]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,dot_SMG[D_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[D_P%10]);
		}
	}
}
/*主函数*/
void main()
{
	Hardware_Init();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		Task_Processing();
		USE_SMG();
		USE_KEY();
	}
}
/*中断函数*/
void Timer0_Routine(void) interrupt 1
{
	F_temp++;
}

void Timer2_Routine(void) interrupt 12
{
	static uc Key_count;
	static uc SMG_count;
	static ui F_count;
	static ui Sonic_count;
	static ui ADC_count;
	static ui S7_count;
	static ui L1_count;
	static ui L2_count;
	static ui L3_count;
//	static uc PWM_count;
	Key_count++;
	if(Key_count==100)
	{
		Key_count=0;
		Key_flag=1;
	}
	SMG_count++;
	if(SMG_count==20)
	{
		SMG_count=0;
		Nixie_Loop();
	}
	F_count++;
	if(F_count==10000)
	{
		F_count=0;
		F=F_temp;
		F_temp=0;
	}
	Sonic_count++;
	if(Sonic_count==2000)
	{
		Sonic_count=0;
		Sonic_flag=1;
	}
	ADC_count++;
	if(ADC_count==1000)
	{
		ADC_count=0;
		ADC_flag=1;
	}
	if(S7_Press==1)
	{
		S7_count++;
		if(S7_count>=10000)
		{
			S7_count=0;
			S7_flag=1;
		}
	}
	else{S7_count=0;S7_flag=0;}
	if(L1_state==1)
	{
		L1_count++;
		if(L1_count==1000)
		{
			L1_count=0;
			L1_flag=~L1_flag;
		}
	}
	else{L1_count=0;L1_flag=0;}
	if(L2_state==1)
	{
		L2_count++;
		if(L2_count==1000)
		{
			L2_count=0;
			L2_flag=~L2_flag;
		}
	}
	else{L2_count=0;L2_flag=0;}
	if(L3_state==1)
	{
		L3_count++;
		if(L3_count==1000)
		{
			L3_count=0;
			L3_flag=~L3_flag;
		}
	}
	else{L3_count=0;L3_flag=0;}
	
//	PWM_count++;
//	if(PWM_flag==1)
//	{
//		if(PWM_count>8){RB_Init();PWM_J36=1;P2&=0X1F;}
//		else{RB_Init();PWM_J36=0;P2&=0X1F;}
//	}
//	else
//	{
//		if(PWM_count>=2){RB_Init();PWM_J36=1;P2&=0X1F;}
//		else{RB_Init();PWM_J36&=0;P2&=0X1F;}
//	}
//	PWM_count%=10;
}	