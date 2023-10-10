#include "main.h"

uc Time[3]={0X05,0X03,0X13};
uc Interface;//����
uc Interface_son;	//���Խ����µ��ӽ���
bit Interface_TH;	//��ʪ�Ƚ���
bit Interface_flag;	//��1-����ʪ�Ƚ��� 0-������ʪ�Ƚ��棩

uc KeyNum;	//��ֵ
f T;	//�¶�����
char T_P=30;//�¶Ȳ�������Χ��0~99��
uc T_Max;	//����¶�
f T_Ave;	//ƽ���¶�
f T_temp;	//�¶���ʱ�����������洢��һ�ζ�ȡ���¶�ֵ��
uc H;	//ʪ��
uc H_Max;	//���ʪ��
f H_Ave;	//ƽ��ʪ��
f H_temp;	//ʪ����ʱ�����������洢��һ�ζ�ȡ��ʪ��ֵ��
f adc_v;	//ͨ��AD��ȡ�Ĺ���ֵת��Ϊ��0~5V����ѹ
ui F;	//NE555���Ƶ��
ui F_temp;	//Ƶ����ʱ����

uc Trg_Count;		//��������
uc Trg_Time[3];		//����ʱ��
bit Trg_flag;	//������־λ

bit CLOCK_flag;	//ʱ�����ݶ�ȡ��־λ
bit Temperature_flag;	//�¶ȶ�ȡ��־λ
bit AD_flag;	//ADֵ��ȡ��־λ
bit Invalid_data;	//��Ч���ݱ�־λ
bit L4_flag;	//L4��־λ
bit L4_blink;
bit L6_flag;	//L6��־λ
bit S9_flag;	//����S9��־λ������-1 �ɿ�-0��
bit KEY_flag;

/*��ʱ����*/
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


/*Ӳ����ʼ������*/
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


/*��������*/
void Task_Processing()
{
	//��ȡʱ������ ��50ms�ɼ�һ��ʱ��ֵ��
	if(CLOCK_flag==1)
	{
		CLOCK_flag=0;
		DS1302_ReadTime(Time);
	}
	
	//��ȡAD�ɼ�������ֵ��50ms�ɼ�һ�ε�ѹֵ��
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=PCF8591_ReadAD(0)*(5.0/255);
	}
	
	//��->�� ����һ����ʪ�����ݲɼ�����
	if(Trg_flag==0 && adc_v>=1.0 && Interface_flag==0)	//��״̬(���Է�ֹ3s���ظ�����)
	{
		Trg_flag=1;
		T_temp=T;
		H_temp=H;
	}
	else if(adc_v<1.0 && Trg_flag==1)	//��״̬
	{
		AUXR |= 0x10;	//�򿪶�ʱ��2��ʱ3s
		Interface_TH=1;	//������ʪ��ҳ��
		Trg_Time[1]=Time[1];	//���津���ɼ���ʱ��
		Trg_Time[2]=Time[2];
		Interface_flag=1;
		
		/*�¶Ȳɼ�*/
		if(Temperature_flag==1)
		{
			Temperature_flag=0;
			DS18B20_ConvertT();
			T=DS18B20_ReadT();
			Trg_flag=0;	//ֻ����һ��
			Trg_Count+=1;	//����������1
			
			if(Trg_Count==1){T_Ave=(T+T_Ave);}	//ƽ���¶ȼ���
			else{T_Ave=(T+T_Ave)/2;}
			if(T_Max < T){T_Max=(uc)T;}	//����¶ȼ���
		}
		
		/*ʪ�Ȳɼ�*/
		if(F <= 200){H=10;Invalid_data=1;}
		else if(F >= 2000){H=90;Invalid_data=1;}
		else if(F<2000 && F>200)
		{
			Invalid_data=0;	//��Ч����
			H=(uc)((F-200)*(80.0/1800))+10;
		}
		if(Trg_Count==1){H_Ave=(H+H_Ave);}	//ƽ��ʪ�ȼ���
		else{H_Ave=(H+H_Ave)/2;}
		if(H_Max < H){H_Max=H;}	//���ʪ�ȼ���
		
		/*L4״̬�ж�*/
		if(T>T_P){L4_flag=1;}	
		else{L4_flag=0;}
		
		/*L6״̬�ж�*/
		if(Trg_Count==1){T_temp=T;H_temp=H;}	//��һ��״̬
		else if(Trg_Count>=2)	//��һ��״̬
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
//	/*��������*/
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
	/*����ָʾ��*/
	if(Interface==0 && Interface_flag==0){L1=0;}	//ʱ�������
	else{L1=1;}
	if(Interface==1 && Interface_flag==0){L2=0;}	//���Խ�����
	else{L2=1;}
	if(Interface_TH==1){L3=0;}	//��ʪ�Ƚ���
	else{L3=1;}
	
	/*����ָʾ��*/
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
	if(Interface_TH==1)	//��ʪ�Ƚ���
	{
		Nixie_SetBuf(0,Nodot_SMG[14]);//��ʶ��-E
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,Nodot_SMG[(ui)T_Ave/10]);	//�¶�
		Nixie_SetBuf(4,Nodot_SMG[(ui)T_Ave%10]);
		Nixie_SetBuf(5,0XBF);
		if(Invalid_data==0)
		{
			Nixie_SetBuf(6,Nodot_SMG[H/10]);	//ʪ��
			Nixie_SetBuf(7,Nodot_SMG[H%10]);
		}
		else
		{
			Nixie_SetBuf(6,Nodot_SMG[10]);	//ʪ��
			Nixie_SetBuf(7,Nodot_SMG[10]);
		}
	}
	else
	{
		if(Interface==0)	//ʱ�����
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
		else if(Interface==1)	//���Խ���
		{
			if(Interface_son==0)	//�¶Ȼ���
			{
				Nixie_SetBuf(0,Nodot_SMG[12]);//��ʶ��-C
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
					Nixie_SetBuf(2,Nodot_SMG[T_Max/10]);	//����¶�
					Nixie_SetBuf(3,Nodot_SMG[T_Max%10]);
					Nixie_SetBuf(4,0XBF);
					Nixie_SetBuf(5,Nodot_SMG[((ui)(T_Ave*10))/100]);//ƽ���¶�
					Nixie_SetBuf(6,dot_SMG[((ui)(T_Ave*10))/10%10]);
					Nixie_SetBuf(7,Nodot_SMG[((ui)(T_Ave*10))%10]);
				}
			}
			else if(Interface_son==1)	//ʪ�Ȼ���
			{
				Nixie_SetBuf(0,0X89);//��ʶ��-H
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
					Nixie_SetBuf(2,Nodot_SMG[H_Max/10]);	//���ʪ��
					Nixie_SetBuf(3,Nodot_SMG[H_Max%10]);
					Nixie_SetBuf(4,0XBF);
					Nixie_SetBuf(5,Nodot_SMG[((ui)(H_Ave*10))/100]);//ƽ��ʪ��
					Nixie_SetBuf(6,dot_SMG[((ui)(H_Ave*10))/10%10]);
					Nixie_SetBuf(7,Nodot_SMG[((ui)(H_Ave*10))%10]);
				}
			}
			else if(Interface_son==2)	//ʱ�����
			{
				Nixie_SetBuf(0,Nodot_SMG[15]);//��ʶ��-F
				Nixie_SetBuf(1,Nodot_SMG[Trg_Count/10]);		//��������
				Nixie_SetBuf(2,Nodot_SMG[Trg_Count%10]);
				if(Trg_Count==0)	//��������Ϊ0ʱ����ʾ
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
		else if(Interface==2)	//��������
		{
			Nixie_SetBuf(0,0X8C);//��ʶ��-P
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
	if(F_Count==1000)	//�ﵽ1s
	{
		F_Count=0;
		F=F_temp;
		F_temp=0;
	}
	
	if(L4_flag==1)
	{
		L4_Count++;
		if(L4_Count==100)	//�ﵽ0.1s
		{
			L4_Count=0;
			L4_blink=~L4_blink;
		}
	}
	else{L4_Count=0;L4_blink=0;}
	
	/*����*/
	if(S9_flag==1)
	{
		S9_Count++;
		if(S9_Count==2000)	//�ﵽ2s
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
	if(T2_Count==3000)	//�ﵽ3s
	{
		T2_Count=0;
		Interface_TH=0;	//�˳���ʪ��ҳ��
		Interface_flag=0;
		AUXR &= ~0x10;	//��ͣ��ʱ
	}
}