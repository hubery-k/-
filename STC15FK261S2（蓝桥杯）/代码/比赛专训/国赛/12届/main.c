#include "main.h"

/***********��������*************/
uc KeyNum;
uc Dat_Interface;//�����ӽ���
uc Dat_Interface_Son;	//�������ӽ���
uc Distance_P=10;		//���������Χ��10~80��
uc Time_P=2;	//�ɼ�ʱ�����
uc TP_temp=2;	//�ɼ�ʱ����ʱ����
uc DP_temp=10;	//�ɼ�������ʱ����
uc Time[3]={0X01,0X20,0X20};
int Distance;
f adc;
ui Max_Distance=0;
ui Min_Distance=999;
f Ave_Distance;
ui Trg_Num;
uc D_Num;	//����ƴ�
uc temp=1;


bit temp_flag;
bit Interface;
bit P_Interface;
bit Mode;
bit CLOCK_Flag;
bit ADC_Flag;
bit SONIC_Flag;
bit KEY_flag;
bit C_Trg;	//����ģʽ�����ɼ�
bit F_Trg;	//��ʱģʽ�����ɼ�
bit State;	//������״̬
bit led_run;
/***********Ӳ����ʼ������*************/

void Hardware_Init()
{
	LED_Init();
	RB_Init();
	Timer0Init();
	DS1302_WriteTime(Time);
	Sonic_Init();
}

/***********��������*************/

void Task_Processing()
{
	if(CLOCK_Flag==1)
	{
		CLOCK_Flag=0;
		DS1302_ReadTime(Time);
		
		/*��ʱģʽ�ж�-��ֵ����ʱ�����(����1s�ɶ�δ��������Լ�����������)*/
		if(temp!=Time[0]/16*10+Time[0]%16){temp=Time[0]/16*10+Time[0]%16;temp_flag=1;}
		if(temp%Time_P==0&&temp_flag==1)
		{
			F_Trg=1;//����һ��
			temp_flag=0;
		}
	}
	if(ADC_Flag==1)
	{
		ADC_Flag=0;
		adc=(PCF8591_ReadAD(0)/255.0)*5.0;
		/*����ģʽ�ж�-������*/
		if(adc>2&&State==0)
		{
			State=1;
		}
		else if(adc<2&&State==1)
		{
			State=0;
			C_Trg=1;//����һ��
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
		/*���ֵ*/
		if(Distance>Max_Distance)
		{
			Max_Distance=Distance;
		}
		/*��Сֵ*/
		if(Distance<Min_Distance)
		{
			Min_Distance=Distance;
		}
		/*ƽ��ֵ*/
		if(Trg_Num==1){Ave_Distance=Distance;}
		else{Ave_Distance=(((f)(Distance+Ave_Distance))/2);}
		/*DAC�������*/
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
	
	/*�����ݽ�����*/
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
/***********KEYӦ�ú���*************/

void USE_KEY()
{
	if(KEY_flag==1)
	{
		KEY_flag=0;
		KeyNum=KEY();
		/*S4-���水��*/
		if(KeyNum==4)
		{
			Interface=~Interface;
			
			Dat_Interface=0;
			P_Interface=0;
			Dat_Interface_Son=0;
			/*���õĲ������˳�����������Ч*/
			if(Interface==0)
			{
				Time_P=TP_temp;
				Distance_P=DP_temp;
			}
		}
		/*S5-�л�����*/
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
		/*S8-ģʽ����*/
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
		/*S9-��������*/
		if(KeyNum==9)
		{
			/*�ڲ������ý�����*/
			if(Interface==1)
			{
				/*�ڲɼ�ʱ�����������*/
				if(P_Interface==0)
				{
					/*�ɼ�ʱ�����*/
					if(TP_temp==2){TP_temp+=1;}
					else if(TP_temp>=3&&TP_temp<9){TP_temp+=2;}
					else if(TP_temp==9){TP_temp=2;}
				}
				/*�ھ������������*/
				else
				{
					/*�������*/
					DP_temp+=10;
					if(DP_temp>80){DP_temp=10;}
				}
			}
		}
	}
}

/***********SMGӦ�ú���*************/

void USE_SMG()
{
	/*������ʾ����*/
	if(Interface==0)
	{
		/*ʱ��������ʾ����*/
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
		/*����������ʾ����*/
		else if(Dat_Interface==1)
		{
			Nixie_SetBuf(0,0XC7);	//��ʶ��-L
			if(Mode==0){Nixie_SetBuf(1,Nodot_SMG[12]);}//��ʶ��-C
			else{Nixie_SetBuf(1,Nodot_SMG[15]);}//��ʶ��-F
			Nixie_SetBuf(2,0xFF);
			Nixie_SetBuf(3,0xFF);
//			Nixie_SetBuf(4,0xFF);
			//�����õģ���ʾ��ʱģʽ������������+-5�Ĵ���������
			Nixie_SetBuf(4,Nodot_SMG[D_Num]);
			//��������ADֵ
//			Nixie_SetBuf(5,dot_SMG[((ui)(adc*100))/100]);
//			Nixie_SetBuf(6,Nodot_SMG[((ui)(adc*100))/10%10]);
//			Nixie_SetBuf(7,Nodot_SMG[((ui)(adc*100))%10]);
			
			if(Distance>99){Nixie_SetBuf(5,Nodot_SMG[Distance/100]);}
			else{Nixie_SetBuf(5,0xFF);}
			if(Distance>9){Nixie_SetBuf(6,Nodot_SMG[Distance/10%10]);}
			else{Nixie_SetBuf(6,0xFF);}
			
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
		/*���ݼ�¼��ʾ����*/
		else if(Dat_Interface==2)
		{
			/*���ֵ����*/
			if(Dat_Interface_Son==0)
			{
				Nixie_SetBuf(0,0X89);	//��ʶ��-H
				Nixie_SetBuf(1,~0X01);	//��ʶ���ϻ���
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
			/*ƽ��ֵ����*/
			else if(Dat_Interface_Son==1)
			{
				Nixie_SetBuf(0,0X89);	//��ʶ��-H
				Nixie_SetBuf(1,~0X40);	//��ʶ���л���
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
			/*��Сֵ����*/
			else if(Dat_Interface_Son==2)
			{
				Nixie_SetBuf(0,0X89);	//��ʶ��-H
				Nixie_SetBuf(1,~0X08);	//��ʶ���»���
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
	/*�������ý���*/
	else if(Interface==1)
	{
		/*�ɼ�ʱ�����ý���*/
		if(P_Interface==0)
		{
			Nixie_SetBuf(0,0X8C);	//��ʶ��-P
			Nixie_SetBuf(1,Nodot_SMG[1]);	//���1
			Nixie_SetBuf(2,0xFF);
			Nixie_SetBuf(3,0xFF);
			Nixie_SetBuf(4,0xFF);
			Nixie_SetBuf(5,0xFF);
			Nixie_SetBuf(6,Nodot_SMG[TP_temp/10]);	//�ɼ�ʱ��
			Nixie_SetBuf(7,Nodot_SMG[TP_temp%10]);
		}
		/*����������ý���*/
		else if(P_Interface==1)
		{
			Nixie_SetBuf(0,0X8C);	//��ʶ��-P
			Nixie_SetBuf(1,Nodot_SMG[2]);	//���1
			Nixie_SetBuf(2,0xFF);
			Nixie_SetBuf(3,0xFF);
			Nixie_SetBuf(4,0xFF);
			Nixie_SetBuf(5,0xFF);
			Nixie_SetBuf(6,Nodot_SMG[DP_temp/10]);	//�������
			Nixie_SetBuf(7,Nodot_SMG[DP_temp%10]);
		}
	}
}

/***********������*************/

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

/***********�жϺ���*************/

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