#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "AT24C02.h"
#include "PCF8591.h"
#include "Delay.h"
uc code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[]={~0xbf,~0x86,~0xdb,~0xcf,~0xe6,~0xed,~0xfd,~0x87,~0xff,~0xef,~0xf7,~0xfc,~0xb9,~0xde,~0xf9,~0xf1};

uc KeyNum;		//������ֵ
uc Interface;		//��ʾ�������
f adc_v;		//�ɼ����ĵ�ѹֵ
ui SMG_v;		//���������������ʾ����ѹֵ*100��
f V_parameter=3.0;		//��ѹ����
uli Num;		//����ֵ
ui T1_Count0;		//��ʱ��1��������������L1��
uc faults;		//��������Ч��������
bit AD_flag;		//AD�ɼ����ݱ�־λ
bit L1_flag;		//LED1��־λ
bit Save_flag;		//AT24C02�����ѹ������־λ
uc State;
f temp;

/*ϵͳ��ʼ������*/
void Sys_Init()
{
	LED_Control(0X00);		//�ر�LED
	RB_Control(0X00);		//�رշ��������̵���
}

/*PCF8591Ӧ�ú���-��Ҫ����AD���ݲɼ�*/
void USE_PCF8591()
{
	/*AD���ݲɼ���ת��*/
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_v=AD_Read()*(5.0/255);
	}
	SMG_v=adc_v*100;
	
	/*L1������Ϩ��*/
	if(adc_v<V_parameter){TR1=1;}
	else if(adc_v>=V_parameter){TR1=0;L1_flag=0;T1_Count0=0;}
	
	/*Num��������*/
	switch(State)
	{
		case 0:if(adc_v>V_parameter){State=1;} break;
		case 1:if(adc_v<V_parameter){State=0;Num++;} break;
	}
}

/*AT24C02Ӧ�ú���-���ڱ����ѹ���������粻��ʧ��*/
void USE_AT24C02()
{
	if(Save_flag==1)
	{
		Save_flag=0;
		EEPROM_Write(0X00,V_parameter*10);
		Delay5ms();		//д�������Ҫ5ms
	}
}

/*LEDӦ�ú���*/
void USE_LED()
{
	LED_Init();
	if(L1_flag==1){L1=0;}
	else{L1=1;}
	
	if(Num%2!=0){L2=0;}
	else{L2=1;}
	
	if(faults>=3){L3=0;}
	else{L3=1;}
	
	P2&=0X1F;		//����
}

/*����Ӧ�ú���*/
void USE_KEY()
{
	KeyNum=KEY_Matrix();
	
	/*��Ч����*/
	if(KeyNum==12)		//S12-��ʾ�����л�����
	{
		Interface++;
		if(Interface==3){Interface=0;}
		if(Interface==2){Save_flag=1;}		//�Ӳ��������˳�����ѹ����*10������
		faults=0;
	}
	if(Interface==2)		//�ڼ���������������Ч
	{
		if(KeyNum==13)		//S13-�������㰴��
		{
			Num=0;
			faults=0;
		}
	}
	if(Interface==1)		//�ڲ������ý�����S16��S17��Ч
	{
		if(KeyNum==16)		//S16-��ѹ�����Ӱ�����ÿ������0.5V��
		{
			V_parameter+=0.5;
			if(V_parameter>5.0){V_parameter=0;}
			faults=0;
		}	
		if(KeyNum==17)		//S17-��ѹ������������ÿ�μ�С0.5V��
		{
			V_parameter-=0.5;
			if(V_parameter<0){V_parameter=5.0;}
			faults=0;
		}
	}
	
	/*�����*/
	if(Interface!=1)
	{
		if(KeyNum==16||KeyNum==17){faults+=1;}
	}
	if(Interface!=2)
	{
		if(KeyNum==13){faults+=1;}
	}
}

void USE_SMG()
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XC1);		//��ʶ��-U
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[SMG_v/100]);
		Nixie_SetBuf(6,Nodot_SMG[SMG_v/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[SMG_v%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0X8C);		//��ʶ��-P
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[(ui)(V_parameter*100)/100]);
		Nixie_SetBuf(6,Nodot_SMG[(ui)(V_parameter*100)/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[(ui)(V_parameter*100)%10]);
	}
	else if(Interface==2)
	{
		Nixie_SetBuf(0,0X89);		//��ʶ��-N����ʾ����Nֻ����ʾH��
		if(Num>999999){Nixie_SetBuf(1,Nodot_SMG[Num/1000000]);}
		else{Nixie_SetBuf(1,0XFF);}
		
		if(Num>99999){Nixie_SetBuf(2,Nodot_SMG[Num/100000%10]);}
		else{Nixie_SetBuf(2,0XFF);}
		
		if(Num>9999){Nixie_SetBuf(3,Nodot_SMG[Num/10000%10]);}
		else{Nixie_SetBuf(3,0XFF);}
		
		if(Num>999){Nixie_SetBuf(4,Nodot_SMG[Num/1000%10]);}
		else{Nixie_SetBuf(4,0XFF);}
		
		if(Num>99){Nixie_SetBuf(5,Nodot_SMG[Num/100%10]);}
		else{Nixie_SetBuf(5,0XFF);}
		
		if(Num>9){Nixie_SetBuf(6,Nodot_SMG[Num/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}

		Nixie_SetBuf(7,Nodot_SMG[Num%10]);
	}
}


void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count0;
	static uc T0_Count1;
	static uc AD_Count;
	
	T0_Count0++;T0_Count1++;
	if(T0_Count0==2)
	{
		T0_Count0=0;
		Nixie_Loop();
	}
	if(T0_Count1==10)
	{
		T0_Count1=0;
		KEY_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)
	{
		AD_flag=1;
	}
}

void Timer1_Routine(void) interrupt 3
{
	T1_Count0++;
	if(T1_Count0>5000)		//����5s
	{
		T1_Count0=0;
		L1_flag=1;
		TR1=0;		//ֹͣ��ʱ
	}
}

void main()
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	temp=EEPROM_Read(0X00)/10.0;//����10��ʱ�����Ҫ����.0
	if(temp>=0&&temp<=5.0){V_parameter=temp;}
	else{V_parameter=3.0;}
	
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_AT24C02();
		USE_PCF8591();
		USE_KEY();
		USE_SMG();
	}
}
/*����LED����ʱ�ر��ж���Ϊ�����жϲ�Ӱ��LED�����У�����LED����״���쳣������*/