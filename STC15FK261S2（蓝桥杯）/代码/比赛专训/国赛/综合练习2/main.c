#include "main.h"
#include "stdio.h"
#include "string.h"

/*����������*/
uc Rec_Buf[5];
uc Send_Buf[12];
uc Time[]={0X50,0X59,0X20};
uc Interface;
uc Dat_Interface;
uc Rec_Interface;
uc P_Interface;
uc KeyNum;
f T;
f adc_v;
ui Distance;
char TP_temp=30;	//�¶Ȳ�����ʱ����
char DP_temp=35;	//���������ʱ����
uc T_P=30;	//��Χ��0~99��
uc D_P=35;	//��Χ��0~99��
uc Change_num;	//�����䶯����
uc Trg_num;
uc T_Max=0;
uc T_Min=99;
f T_Ave;

bit Key_flag;
bit Clock_flag;
bit T_flag;
bit ADC_flag;
bit Sonic_flag;
bit grade;
bit P_state;
bit Write_flag;
bit Trg_flag;
bit Trg_State;
bit K8_flag;
bit K9_flag;
bit DAC_flag;
bit L6_flag;
bit L7_flag;
bit L8_flag;
bit Rec_flag;
bit Led_run;
bit Nixie_flag;


/*Ӳ����ʼ����*/
void Hardware_Init()
{
	LED_Init();
	RB_Init();
	DS1302_WriteTime(Time);
	Timer0Init();
	Sonic_Init();
	UartInit();
	DS18B20_ConvertT();
	Change_num=AT24C02_ReadByte(0X00);
	Delay750ms();
}

/*��������*/
void Task_Processing()
{
	/*ʱ���ȡ*/
	if(Clock_flag==1)
	{
		Clock_flag=0;
		DS1302_ReadTime(Time);
	}
	/*�¶Ȳɼ�*/
	if(T_flag==1)
	{
		T_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
	}
	/*PCF8591�ɼ����������*/
	if(ADC_flag==1)
	{
		ADC_flag=0;
		adc_v=PCF8591_ReadAD(0)/255.0*5.0;
		/*��->�������ɼ��¶�һ��*/
		if(adc_v>2&&Trg_State==0)	//��״̬
		{
			grade=1;
			Trg_State=1;
		}
		else if(adc_v<2&&Trg_State==1)	//��״̬
		{
			grade=0;
			Trg_State=0;
			Trg_flag=1;
			L6_flag=1;
		}
		/*DAC�������*/
		if(DAC_flag==1)
		{
			PCF8591_WriteDA(255);
		}
		else
		{
			if(Distance<=10){PCF8591_WriteDA((1/5.0)*255);}
			else if(Distance>10 && Distance<80)
			{
				PCF8591_WriteDA((((Distance-10)*(4.0/70)+1)/5.0)*255);
			}
			else if(Distance>80){PCF8591_WriteDA(255);}
		}
	}
	/*�����¶Ȳɼ�*/
	if(Trg_flag==1)
	{
		Trg_flag=0;
		Trg_num++;
		/*�ɼ��¶����ֵ*/
		if(T>T_Max){T_Max=T;}
		/*�ɼ��¶���Сֵ*/
		if(T<T_Min){T_Min=T;}
		/*�ɼ��¶�ƽ��ֵ*/
		if(Trg_num==1){T_Ave=T;}
		else{T_Ave=(T+T_Ave)/2;}
	}
	/*�������������*/
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance=Sonic_Work();
		if(Distance>99){Distance=99;}
	}
	/*EEPROM��ȡ����*/
	if(Write_flag==1)
	{
		Write_flag=0;
		AT24C02_WriteByte(0X00,Change_num);
		Delay5ms();
	}

	/*���ڹ���*/
	if(Rec_flag==1)
	{
		Rec_flag=0;
		if(strcmp(Rec_Buf,"T")==0)//�����¶�
		{
			sprintf(Send_Buf,"T:%.2f��C\r\n",T);
			Uart_SendString(Send_Buf);
		}
		else if(strcmp(Rec_Buf,"D")==0)//���ؾ���
		{
			sprintf(Send_Buf,"D:%dcm\r\n",Distance);
			Uart_SendString(Send_Buf);
		}
		else if(strcmp(Rec_Buf,"R")==0)//����AD�ɼ���ѹ
		{
			sprintf(Send_Buf,"AD:%.2fV\r\n",adc_v);
			Uart_SendString(Send_Buf);
		}
		else
		{
			Uart_SendString("Error\r\n");
		}
	}
}

void USE_LED()
{
	LED_Init();
	/*����ָʾ��*/
	if(Interface==0){L1=0;}
	else{L1=1;}
	if(Interface==1){L2=0;}
	else{L2=1;}
	if(Interface==2){L3=0;}
	else{L3=1;}
	/*״ָ̬ʾ��*/
	if(T>T_P){L4=0;}
	else{L4=1;}
	if(Distance>D_P){L5=0;}
	else{L5=1;}
	/*����ָʾ��*/
	if(L6_flag==1){L6=0;}
	else{L6=1;}
	if(L7_flag==1){L7=0;}
	else{L7=1;}
	if(L8_flag==1){L8=0;}
	else{L8=1;}
	
	P2&=0X1F;
}

/*Ӧ�ð�����*/
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
			
			Dat_Interface=0;
			Rec_Interface=0;
			P_Interface=0;
			
			/*�����䶯���˳�����ʱ��Ч*/
			if(Interface==0)
			{
				T_P=TP_temp;
				D_P=DP_temp;
			}
			/*�����䶯����EEPROM����䶯����*/
			if(Interface==0)
			{
				if(P_state==1)
				{
					P_state=0;
					Change_num++;
					Write_flag=1;
				}
			}
		}
		if(KeyNum==5)
		{
			if(Interface==0)
			{
				Dat_Interface++;
				Dat_Interface%=5;
			}
			else if(Interface==1)
			{
				Rec_Interface++;
				Rec_Interface%=3;
			}
			else if(Interface==2)
			{
				P_Interface++;
				P_Interface%=3;
			}
		}
		/*S8-���̰�:����δ����1s���𴥷��������Ӱ���*/
		if(Cont==8&&K8_flag==0&&K8_Press==0)
		{
			Cont=0;
			/*����������*/
			if(Interface==2)
			{
				if(P_Interface==0){TP_temp+=2;P_state=1;}
				if(P_Interface==1){DP_temp+=5;P_state=1;}
			}
			
			if(TP_temp>99){TP_temp=TP_temp-100;}
			if(DP_temp>99){DP_temp=DP_temp-100;}
		}
		/*S8-������:���³���1s�������������*/
		if(Cont==8&&K8_flag==1)
		{
			Cont=0;
			DAC_flag=~DAC_flag;
			L7_flag=1;
		}
		
		/*S9-���̰�:����δ����1s���𴥷�������������*/
		if(Cont==9&&K9_flag==0&&K9_Press==0)
		{
			Cont=0;
			/*����������*/
			if(Interface==2)
			{
				if(P_Interface==0){TP_temp-=2;P_state=1;}
				if(P_Interface==1){DP_temp-=5;P_state=1;}
			}
			
			if(TP_temp<0){TP_temp=TP_temp+100;}
			if(DP_temp<0){DP_temp=DP_temp+100;}
		}
		/*S9-������:���³���1s�������������*/
		if(Cont==9&&K9_flag==1)
		{
			Cont=0;
			T_Max=0;T_Min=99;T_Ave=0;
			Change_num=0;
			Write_flag=1;
			L8_flag=1;
		}
	}
}

/*Ӧ���������*/
void USE_SMG()
{
	if(Interface==0)
	{
		if(Dat_Interface==0)
		{
			Nixie_SetBuf(0,Nodot_SMG[Time[2]/16]);
			Nixie_SetBuf(1,Nodot_SMG[Time[2]%16]);
			Nixie_SetBuf(2,0XBF);	//�����
			Nixie_SetBuf(3,Nodot_SMG[Time[1]/16]);
			Nixie_SetBuf(4,Nodot_SMG[Time[1]%16]);
			Nixie_SetBuf(5,0XBF);	//�����
			Nixie_SetBuf(6,Nodot_SMG[Time[0]/16]);
			Nixie_SetBuf(7,Nodot_SMG[Time[0]%16]);
		}
		else if(Dat_Interface==1)
		{
			Nixie_SetBuf(0,Nodot_SMG[12]);	//��ʶ��-C
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,Nodot_SMG[((ui)(T*10))/100]);
			Nixie_SetBuf(6,dot_SMG[((ui)(T*10))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(T*10))%10]);
		}
		else if(Dat_Interface==2)
		{
			Nixie_SetBuf(0,Nodot_SMG[14]);	//��ʶ��-E
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,dot_SMG[((ui)(adc_v*100))/100]);
			Nixie_SetBuf(4,Nodot_SMG[((ui)(adc_v*100))/10%10]);
			Nixie_SetBuf(5,Nodot_SMG[((ui)(adc_v*100))%10]);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,Nodot_SMG[grade]);
		}
		else if(Dat_Interface==3)
		{
			Nixie_SetBuf(0,Nodot_SMG[15]);	//��ʶ��-F
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
			Nixie_SetBuf(0,0XC7);	//��ʶ��-L
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
	else if(Interface==1)
	{
		if(Rec_Interface==0)
		{
			Nixie_SetBuf(0,0X89);	//��ʶ��-H
			Nixie_SetBuf(1,0XFE);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[T_Max/10]);
			Nixie_SetBuf(7,Nodot_SMG[T_Max%10]);
		}
		else if(Rec_Interface==1)
		{
			Nixie_SetBuf(0,0X89);	//��ʶ��-H
			Nixie_SetBuf(1,0XBF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,Nodot_SMG[((ui)(T_Ave*10))/100]);
			Nixie_SetBuf(6,dot_SMG[((ui)(T_Ave*10))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(T_Ave*10))%10]);
		}
		else if(Rec_Interface==2)
		{
			Nixie_SetBuf(0,0X89);	//��ʶ��-H
			Nixie_SetBuf(1,0XF7);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[T_Min/10]);
			Nixie_SetBuf(7,Nodot_SMG[T_Min%10]);
		}
	}
	else if(Interface==2)
	{
		if(P_Interface==0)
		{
			Nixie_SetBuf(0,0X8C);	//��ʶ��-P
			Nixie_SetBuf(1,Nodot_SMG[1]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[TP_temp/10]);
			Nixie_SetBuf(7,Nodot_SMG[TP_temp%10]);
		}
		else if(P_Interface==1)
		{
			Nixie_SetBuf(0,0X8C);	//��ʶ��-P
			Nixie_SetBuf(1,Nodot_SMG[2]);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[DP_temp/10]);
			Nixie_SetBuf(7,Nodot_SMG[DP_temp%10]);
		}
		else if(P_Interface==2)
		{
			Nixie_SetBuf(0,0X8C);	//��ʶ��-P
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

/*��������*/
void main()
{
	Hardware_Init();
	while(1)
	{
		Task_Processing();
		if(Led_run==1){Led_run=0;USE_LED();}
		USE_SMG();
		USE_KEY();
	}
}

void Timer0_Routine(void) interrupt 1
{
	static uc Key_count;
	static uc SMG_count;
	static uc Clock_count;
	static uc T_count;
	static uc ADC_count;
	static uc Sonic_count;
	static ui S8_Count;
	static ui S9_Count;
	static ui L6_count;
	static ui L7_count;
	static ui L8_count;
	Led_run=1;
	
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
	Clock_count++;
	if(Clock_count==100)
	{
		Clock_count=0;
		Clock_flag=1;
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
	/*S8���̰��ж�*/
	if(K8_Press==1)
	{
		S8_Count++;
		if(S8_Count>=1000)
		{
			S8_Count=0;
			K8_flag=1;
		}
	}
	else{S8_Count=0;K8_flag=0;}
	/*S9���̰��ж�*/
	if(K9_Press==1)
	{
		S9_Count++;
		if(S9_Count>=1000)
		{
			S9_Count=0;
			K9_flag=1;
		}
	}
	else{S9_Count=0;K9_flag=0;}
	/*L6*/
	if(L6_flag==1)
	{
		L6_count++;
		if(L6_count>=3000)
		{
			L6_count=0;
			L6_flag=0;
		}
	}
	/*L7*/
	if(L7_flag==1)
	{
		L7_count++;
		if(L7_count>=3000)
		{
			L7_count=0;
			L7_flag=0;
		}
	}
	/*L8*/
	if(L8_flag==1)
	{
		L8_count++;
		if(L8_count>=3000)
		{
			L8_count=0;
			L8_flag=0;
		}
	}
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
				Rec_flag=1;	//��ʾ���ݽ�����
				Rec_Buf[i]='\0';
				i=0;
				Rx_State=0;
			}
		}
	}
}