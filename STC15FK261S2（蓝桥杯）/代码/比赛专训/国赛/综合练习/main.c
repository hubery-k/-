#include "main.h"
#include "stdio.h"
#include "string.h"

/*变量定义区*/
bit KEY_flag;
bit C_flag;
bit T_flag;
bit ADC_flag;
bit grade;
bit Trg_flag;
bit state;
bit Sonic_flag;
bit Rec_flag;
bit P_State;	//参数变动状态
bit Write_flag;
bit K8_flag;
bit K9_flag;
bit L6_flag;
bit L7_flag;
bit L8_flag;
bit DAC_flag;
bit led_run;


uc Rec_Buf[5];
uc Send_Buf[10];
uc Time[3]={0x50,0x59,0x20};
uc Interface;
uc Dat_interface;	//数据界面
uc Rec_interface;	//数据记录界面
uc Par_interface;	//参数界面
uc KeyNum;
f T;
f adc;
ui Distance;
//i F_temp;
//i F;
uc T_Max=0;
uc T_Min=99;
f T_Ave;
uc Trg_Num;
c TP_temp=30;
c DP_temp=35;
c T_P=30;	//温度参数（范围：0~99）
c D_P=35;	//距离参数（范围：0~99）
uc Change_num;	//参数变动次数

/*硬件初始化*/
void Hardware_Init()
{
	LED_Init();
	RB_Init();
//	Timer0Init();
	Soinc_Init();
	Timer1Init();
	UartInit();
	DS1302_WriteTime(Time);
	DS18B20_ConvertT();
	Change_num=AT24C02_ReadByte(0X00);

	Delay750ms();
}

void Task_Processing()
{
	/*时间获取*/
	if(C_flag==1)
	{
		C_flag=0;
		DS1302_ReadTime(Time);
	}
	/*温度采集*/
	if(T_flag==1)
	{
		T_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
	}
	/*PCF8591采集与输出功能*/
	if(ADC_flag==1)
	{
		ADC_flag=0;
		adc=(PCF8591_ReadAD(0)/255.0)*5.0;
		/*亮度等级检测*/
		if(adc>2){grade=1;}
		else{grade=0;}
		/*亮状态到暗状态触发转换条件*/
		if(state==0&&adc>2){state=1;}
		else if(state==1&&adc<2){state=0;Trg_flag=1;L6_flag=1;}
		
		/*DAC输出功能*/
		if(DAC_flag==1)
		{
			PCF8591_WriteDA(255);
		}
		else
		{
			if(Distance<=10){PCF8591_WriteDA((1/5.0)*255);}
			else if(Distance>10 && Distance<80)
			{
				PCF8591_WriteDA(((((Distance-10)*(4/70.0))+1)/5.0)*255);
			}
			else if(Distance>=80){PCF8591_WriteDA(255);}
		}
		
	}
	/*超声波距离测量*/
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance=Sonic_Work();
		if(Distance>99){Distance=99;}
	}
	/*触发捕获温度*/
	if(Trg_flag==1)
	{
		Trg_flag=0;
		Trg_Num++;
		if(T_Max<T){T_Max=T;}
		if(T_Min>T){T_Min=T;}
		/*平均值*/
		if(Trg_Num==1){T_Ave=T;}
		else{T_Ave=((T+T_Ave)/2);}
	}
	/*串口功能*/
	if(Rec_flag==1)
	{
		Rec_flag=0;
		if(strcmp(Rec_Buf,"T")==0)//返回温度
		{
			sprintf(Send_Buf,"T:%.2f°C\r\n",T);
			Uart_SendString(Send_Buf);
		}
		else if(strcmp(Rec_Buf,"D")==0)//返回距离
		{
			sprintf(Send_Buf,"D:%dcm\r\n",Distance);
			Uart_SendString(Send_Buf);
		}
		else if(strcmp(Rec_Buf,"R")==0)//返回AD采集电压
		{
			sprintf(Send_Buf,"AD:%.2fV\r\n",adc);
			Uart_SendString(Send_Buf);
		}
		else
		{
			Uart_SendString("Error\r\n");
		}
	}
	
	/*EEPROM存取功能*/
	if(Write_flag==1)
	{
		Write_flag=0;
		AT24C02_WriteByte(0X00,Change_num);
		Delay5ms();//写字节最大需要5ms时间
	}
}

void USE_LED()
{
	LED_Init();
	/*界面提示灯*/
	if(Interface==0){L1=0;}
	else{L1=1;}
	if(Interface==1){L2=0;}
	else{L2=1;}
	if(Interface==2){L3=0;}
	else{L3=1;}
	/*参数灯*/
	if(T>T_P){L4=0;}
	else{L4=1;}
	if(Distance>D_P){L5=0;}
	else{L5=1;}
	/*状态触发灯*/
	if(L6_flag==1){L6=0;}	//采集温度状态触发
	else{L6=1;}
	if(L7_flag==1){L7=0;}	//S8长按状态触发
	else{L7=1;}
	if(L8_flag==1){L8=0;}	//S9长按状态触发
	else{L8=1;}
	P2&=0X1F;
}


void USE_KEY()
{
	if(KEY_flag==1)
	{
		KEY_flag=0;
		KeyNum=KEY();
		/*S4-切换界面按键*/
		if(KeyNum==4)
		{
			Interface++;
			Interface%=3;
			
			Dat_interface=0;
			Rec_interface=0;
			Par_interface=0;
			/*退出参数界面，进入数据界面。设置参数生效*/
			if(Interface==0)
			{
				T_P=TP_temp;
				D_P=DP_temp;
			}
			/*参数变动-触发EEPROM保存变动次数*/
			if(Interface==0)
			{
				if(P_State==1)
				{
					P_State=0;
					Change_num++;
					Write_flag=1;
				}
			}
		}
		/*S5-切换子界面按键*/
		if(KeyNum==5)
		{
			/*数据界面*/
			if(Interface==0)
			{
				Dat_interface++;
				Dat_interface%=5;
			}
			/*数据记录显示界面*/
			else if(Interface==1)
			{
				Rec_interface++;
				Rec_interface%=3;
			}
			/*参数界面*/
			else if(Interface==2)
			{
				Par_interface++;
				Par_interface%=3;
			}
		}
		/*S8-（短按:按下未超过1s弹起触发）参数加按键*/
		if(Cont==8&&K8_flag==0&&K8_Press==0)
		{
			Cont=0;
			/*参数界面下*/
			if(Interface==2)
			{
				if(Par_interface==0){TP_temp+=2;P_State=1;}
				else if(Par_interface==1){DP_temp+=5;P_State=1;}
			}
			if(TP_temp>99){TP_temp=TP_temp-100;}
			if(DP_temp>99){DP_temp=DP_temp-100;}
		}
		/*S8-（长按:按下超过1s触发）清除按键*/
		if(Cont==8&&K8_flag==1)
		{
			Cont=0;
			DAC_flag=~DAC_flag;	//DAC启动与停止状态切换
			L7_flag=1;	//L6亮3s
		}
		
		/*S9-（短按:按下未超过1s弹起触发）参数减按键*/
		if(Cont==9&&K9_flag==0&&K9_Press==0)
		{
			Cont=0;
			/*参数界面下*/
			if(Interface==2)
			{
				if(Par_interface==0){TP_temp-=2;P_State=1;}
				else if(Par_interface==1){DP_temp-=5;P_State=1;}
			}
			if(TP_temp<0){TP_temp=TP_temp+100;}
			if(DP_temp<0){DP_temp=DP_temp+100;}
		}
		
		/*S9-（长按:按下超过1s触发）清除按键*/
		if(Cont==9&&K9_flag==1)
		{
			Cont=0;
			T_Max=0;T_Min=99;T_Ave=0;
			Change_num=0;
			Write_flag=1;
			L8_flag=1;	//L7亮3s
		}
	}
}
void USE_SMG()
{
	/*数据界面*/
	if(Interface==0)
	{
		/*时间界面*/
		if(Dat_interface==0)
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
		/*温度界面*/
		else if(Dat_interface==1)
		{
			Nixie_SetBuf(0,Nodot_SMG[12]);	//标识符-C
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,Nodot_SMG[((ui)(T*10))/100]);
			Nixie_SetBuf(6,dot_SMG[((ui)(T*10))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(T*10))%10]);
		}
		/*光照电压界面*/
		else if(Dat_interface==2)
		{
			Nixie_SetBuf(0,Nodot_SMG[14]);	//标识符-E
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,dot_SMG[((ui)(adc*100))/100]);
			Nixie_SetBuf(4,Nodot_SMG[((ui)(adc*100))/10%10]);
			Nixie_SetBuf(5,Nodot_SMG[((ui)(adc*100))%10]);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,Nodot_SMG[grade]);
		}
		/*频率界面*/
		else if(Dat_interface==3)
		{
			Nixie_SetBuf(0,0XFF);
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
//			Nixie_SetBuf(0,Nodot_SMG[15]);	//标识符-F
//			Nixie_SetBuf(1,0XFF);
//			if(F>99999){Nixie_SetBuf(2,Nodot_SMG[F/100000]);}
//			else{Nixie_SetBuf(2,0XFF);}
//			if(F>9999){Nixie_SetBuf(3,Nodot_SMG[F/10000%10]);}
//			else{Nixie_SetBuf(3,0XFF);}
//			if(F>999){Nixie_SetBuf(4,Nodot_SMG[F/1000%10]);}
//			else{Nixie_SetBuf(4,0XFF);}
//			if(F>99){Nixie_SetBuf(5,Nodot_SMG[F/100%10]);}
//			else{Nixie_SetBuf(5,0XFF);}
//			if(F>9){Nixie_SetBuf(6,Nodot_SMG[F/10%10]);}
//			else{Nixie_SetBuf(6,0XFF);}
//			Nixie_SetBuf(7,Nodot_SMG[F%10]);
		}
		/*超声波测距界面*/
		else if(Dat_interface==4)
		{
			Nixie_SetBuf(0,0XC7);	//标识符-L
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);

			if(Distance>9){Nixie_SetBuf(6,Nodot_SMG[Distance/10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
	}
	/*数据记录界面*/
	else if(Interface==1)
	{
		if(Rec_interface==0)
		{
			Nixie_SetBuf(0,0X89);	//标识符-H
			Nixie_SetBuf(1,0XFE);	//上标
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[T_Max/10]);
			Nixie_SetBuf(7,Nodot_SMG[T_Max%10]);
		}
		else if(Rec_interface==1)
		{
			Nixie_SetBuf(0,0X89);	//标识符-H
			Nixie_SetBuf(1,0XBF); //中标
			Nixie_SetBuf(2,0XFF);	
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[T_Min/10]);
			Nixie_SetBuf(7,Nodot_SMG[T_Min%10]);
		}
		else if(Rec_interface==2)
		{
			Nixie_SetBuf(0,0X89);	//标识符-H
			Nixie_SetBuf(1,0XF7);	//下标
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,Nodot_SMG[((ui)(T_Ave*10))/100]);
			Nixie_SetBuf(6,dot_SMG[((ui)(T_Ave*10))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(T_Ave*10))%10]);
		}
	}
	/*参数界面*/
	else if(Interface==2)
	{
		if(Par_interface==0)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[1]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[TP_temp/10]);
			Nixie_SetBuf(7,Nodot_SMG[TP_temp%10]);
		}
		else if(Par_interface==1)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[2]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[DP_temp/10]);
			Nixie_SetBuf(7,Nodot_SMG[DP_temp%10]);
		}
		else if(Par_interface==2)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,Nodot_SMG[3]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[Change_num/10]);
			Nixie_SetBuf(7,Nodot_SMG[Change_num%10]);
		}
	}
}
void main()
{
	Hardware_Init();
	while(1)
	{
		if(led_run==1){USE_LED();led_run=0;}
		USE_SMG();
		USE_KEY();
		Task_Processing();
	}
}

//void Timer0_Routine() interrupt 1
//{
//	F_temp++;
//}

void Timer1_Routine() interrupt 3
{
	static uc SMG_count;
	static uc KEY_count;
	static uc C_count;
	static uc T_count;
	static uc ADC_count;
	static uc Sonic_count;
	static ui K9_Count;
	static ui K8_Count;
	static ui L6_Count;
	static ui L7_Count;
	static ui L8_Count;
//	static ui F_count;
	led_run=1;	//LED运行不受中断影响
	
	KEY_count++;
	if(KEY_count==10)
	{
		KEY_count=0;
		KEY_flag=1;
	}
	SMG_count++;
	if(SMG_count==2)
	{
		SMG_count=0;
		Nixie_Loop();
	}
	C_count++;
	if(C_count==50)
	{
		C_count=0;
		C_flag=1;
	}
	T_count++;
	if(T_count==100)
	{
		T_count=0;
		T_flag=1;
	}
	ADC_count++;
	if(ADC_count==100)
	{
		ADC_count=0;
		ADC_flag=1;
	}
	Sonic_count++;
	if(Sonic_count==200)
	{
		Sonic_count=0;
		Sonic_flag=1;
	}
	/*K9长短按判断*/
	if(K9_Press==1)
	{
		K9_Count++;
		if(K9_Count>=1000)
		{
			K9_flag=1;
			K9_Count=0;
		}
	}
	else{K9_Count=0;K9_flag=0;}
	
	/*K8长短按判断*/
	if(K8_Press==1)
	{
		K8_Count++;
		if(K8_Count>=1000)
		{
			K8_flag=1;
			K8_Count=0;
		}
	}
	else{K8_Count=0;K8_flag=0;}
	if(L6_flag==1)
	{
		L6_Count++;
		if(L6_Count>=3000)
		{
			L6_Count=0;
			L6_flag=0;
		}
	}
	if(L7_flag==1)
	{
		L7_Count++;
		if(L7_Count>=3000)
		{
			L7_Count=0;
			L7_flag=0;
		}
	}
	if(L8_flag==1)
	{
		L8_Count++;
		if(L8_Count>=3000)
		{
			L8_Count=0;
			L8_flag=0;
		}
	}
//	F_count++;
//	if(F_count==1000)
//	{
//		F_count=0;
//		F=F_temp;
//		F_temp=0;
//	}
}

void Uart_Routine(void) interrupt 4
{
	uc temp;
	bit Rx_State;
	static uc i;
	
	if(RI==1)
	{
		RI=0;
		temp=SBUF;
		if(Rx_State==0)
		{
			if(temp=='\r'){Rx_State=1;}
			else
			{
				Rec_Buf[i++]=temp;
			}
		}
		else
		{
			if(temp=='\n')
			{
				Rec_flag=1;	//表示数据接收完
				Rec_Buf[i]='\0';
				i=0;
				Rx_State=0;
			}
		}
	}
}