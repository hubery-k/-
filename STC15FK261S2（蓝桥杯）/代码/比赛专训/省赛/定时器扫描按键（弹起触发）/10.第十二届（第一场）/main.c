#include "LED.h"
#include "RB.h"
#include "Timer.h"
#include "NIXIE.h"
#include "KEY.h"
#include "Delay.h"
#include "DS18B20.h"
#include "DA.h"
unsigned char code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
unsigned char code dot_SMG[]={~0xBF,~0x86,~0xDB,~0xCF,~0xE6,~0xED,~0xFD,~0x87,~0xFF,~0xEF,~0xF7,~0xFC,~0xB9,~0xDE,~0xF9,~0xF1};

unsigned char Interface;
float T;
unsigned char T_Int;	//�¶���������
unsigned char T_De;		//�¶�С������
float T_temp;		//�¶���ʱ����
unsigned char KeyNum;
signed char T_parameter=25;		//�¶Ȳ�����ʼֵ25���Զ��巶Χ��0~50��
unsigned int DAC_V;
bit Mode;
bit DS18B20_Flag;
void Sys_Init()
{
	LED_Control(0X00);	//�ر�LED
	RB_Control(0X00);		//�رշ������̵���
}

void USE_DS18B20(void)
{
	if(DS18B20_Flag==1)
	{
		DS18B20_Flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		T_Int=DS18B20_ReadT();
		T_De=(unsigned char)(DS18B20_ReadT()*100)%100;		//ȡ������λ�¶�����
	}
}

void USE_DAC(void)
{
	if(Mode==0)		//ģʽ1
	{
		if(T<T_parameter){DAC_Write(0);DAC_V=0;}
		else{DAC_Write(255);DAC_V=500;}
		
	}
	else if(Mode==1)		//ģʽ2������y=kx -> k=3V/20��C ˵������1���ѹ����0.15V
	{
		if(T<40){DAC_Write((1.0/5.0)*255);DAC_V=100;}		//���1.0V
		if(T>40){DAC_Write((4.0/5.0)*255);DAC_V=400;}		//���4.0V
		if(T>=20&&T<=40)		//����1���ѹ����0.15V
		{
			T_temp=T-20;
			DAC_V=100+(T_temp*0.15)*100;
			DAC_Write((DAC_V/100.0/5)*255);
		}		
	}
}

/*1ms�Ķ�ʱ�жϺ���*/
void Timer0_Routine() interrupt 1
{
	static unsigned char T0Count1;
	static unsigned char T0Count2;
	static unsigned char T_Count;
	
	T0Count1++;T0Count2++;
	if(T0Count1==2)
	{
		T0Count1=0;
		Nixie_Loop();
	}
	if(T0Count2==10)
	{
		T0Count2=0;
		KEY_Loop();
	}
	T_Count++;
	if(T_Count==100)		//100ms��ȡһ���¶�����
	{
		T_Count=0;
		DS18B20_Flag=1;
	}
	
	
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-���水��
	{
		Interface++;
		if(Interface==3){Interface=0;}
	}
	if(Interface==1)		//�������ý�����
	{
		if(KeyNum==8)		//S8-���������¼��������¶Ȳ�����1
		{
			T_parameter-=1;
			if(T_parameter<0){T_parameter=50;}
		}
		if(KeyNum==9)		//S9-���������¼Ӱ������¶Ȳ�����1
		{
			T_parameter+=1;
			if(T_parameter>50){T_parameter=0;}
		}
	}
	if(KeyNum==5)		//S5-ģʽ�л�����
	{
		Mode=~Mode;
	}
	
}

void USE_SMG()
{
	if(Interface==0)		//�¶���ʾ����
	{
		Nixie_SetBuf(0,Nodot_SMG[12]);		//��ʶ-C
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,Nodot_SMG[T_Int/10]);
		Nixie_SetBuf(5,dot_SMG[T_Int%10]);
		Nixie_SetBuf(6,Nodot_SMG[T_De/10]);
		Nixie_SetBuf(7,Nodot_SMG[T_De%10]);
	}
	else if(Interface==1)		//�������ý���
	{
		Nixie_SetBuf(0,0X8C);		//��ʶ-P
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,0XFF);
		Nixie_SetBuf(6,Nodot_SMG[T_parameter/10]);
		Nixie_SetBuf(7,Nodot_SMG[T_parameter%10]);
	}
	else if(Interface==2)		//DAC�������
	{
		Nixie_SetBuf(0,Nodot_SMG[10]);		//��ʶ-A
		Nixie_SetBuf(1,0XFF);
		Nixie_SetBuf(2,0XFF);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		
		Nixie_SetBuf(5,dot_SMG[DAC_V/100]);
		Nixie_SetBuf(6,Nodot_SMG[DAC_V/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[DAC_V%10]);
	}
}

void USE_LED()
{
	if(Mode==0&&Interface==0){LED_Control(0X03);}
	else if(Mode==0&&Interface==1){LED_Control(0X05);}
	else if(Mode==0&&Interface==2){LED_Control(0X09);}
	else{LED_Control(0X00);}
}


void main()
{
	Sys_Init();
	DS18B20_ConvertT();
	Delay750ms();		//�¶�����ת�������Ҫ750msʱ��
	Timer0Init();
	while(1)
	{
		EA=0;	
		USE_LED();
		EA=1;
		USE_DAC();
		USE_SMG();
		USE_DS18B20();
		USE_KEY();
	}
}
/*����LED����ʱ�ر����жϣ����ý��������Ϊ�˽��LED΢΢������*/