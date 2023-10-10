#include "LED.h"
#include "RELAY.h"
#include "BUZZER.h"
#include "Nixie.h"
#include "KEY.h"
#include "Ds18B20.h"
#include "Timer.h"
#include "Delay.h"

/*ʹ�õ���Ӳ����LED���̵���������ܡ�������̡�DS18B20�¶ȴ�����*/
unsigned char code Nodot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code dot_SMG[]={0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10};

unsigned char T_Int;
unsigned char T_Max=30;		//ϵͳĬ������30�����99��
unsigned char T_Min=20;		//ϵͳĬ������20����С0��
unsigned char T_Scope;		//�¶����䣨0~2��
unsigned char Interface;	//��ʾ����
unsigned char T_Limit[4];		//���������¶���ֵ����
unsigned char temp;

bit L2_State;
bit L1_State;
bit RELAY_State;

void Sys_Init(void)
{
	LED_Close(0XFF);
	RELAY_Close();
	BUZZER_Close();
	Nixie_NoDisplay();
	P2=P2&0X1F;//����
}

void USE_DS18B20(void)
{
	/*��Ŀ��Ҫ���¶����ݱ���Ϊ��������*/
	/*���¶���ֵ���÷�ΧΪ0~99�����Բ��ٽ��������¶��ж�*/
	DS18B20_Convert();
	T_Int=DS18B20_ReadT();
	
	if(T_Int<T_Min&&T_Max>T_Min){T_Scope=0;}
	else if(T_Int<=T_Max&&T_Int>=T_Min&&T_Max>T_Min){T_Scope=1;}
	else if(T_Int>T_Max&&T_Max>T_Min){T_Scope=2;}
	else if(T_Max<T_Min){T_Scope=10;}//��ʾ�¶����ݳ�������	
}

/*Ŀǰ���������⣺�Ѹþ��󰴼���������KEY.c�ļ��У����ݹ���ʹ��һ�κ�KeyNum����״̬�������������ʹ�õ�����κ���������*/
void USE_MatrixKEY(void)
{
	MatrixKey();

	if(KeyNum==10)		//���ð�������
	{
		if(Interface==0)
		{
			Interface=1;		//����������ý���
		}		
		else if(Interface==1)
		{
			if(temp!=0)
			{
				T_Max=T_Limit[0]*10+T_Limit[1];		//�����¶�����
				T_Min=T_Limit[2]*10+T_Limit[3];
				
				temp=0;
				T_Limit[0]=T_Limit[1]=T_Limit[2]=T_Limit[3]=0;		//�������¶�����Ҳ������������
			}		
			if(T_Max<T_Min)//�����¶����ݳ����������¶���ʾ���棬L2����
			{
				Interface=1;	
				L2_State=1;
				L1_State=0;					
			}
			else		//�����¶���ʾ����
			{
				Interface=0;
				L2_State=0;
			}		
		}		
		KeyNum=20;		//û�м�ֵ��Ӧ20���˴���Ϊ�������������״̬
	}
	
	if(Interface==1)		//�ڲ������ý�����
	{
		if(KeyNum<10)		
		{
			if(temp<4)
			{
				T_Limit[temp++]=KeyNum;		//�����¶�������
			}
			KeyNum=20;
		}
		
		if(KeyNum==11)		//�����������
		{
			T_Limit[0]=T_Limit[1]=T_Limit[2]=T_Limit[3]=0;
			temp=0;
			KeyNum=20;
		}
	}
}

void USE_SMG(void)
{
	if(Interface==0)
	{
		Nixie_Display(0,0XBF);
		Nixie_Display(1,Nodot_SMG[T_Scope]);
		Nixie_Display(2,0XBF);
		
		Nixie_Display(6,Nodot_SMG[T_Int/10]);
		Nixie_Display(7,Nodot_SMG[T_Int%10]);
	}
	else if(Interface==1)
	{
		Nixie_Display(0,0XBF);
		Nixie_Display(5,0XBF);
		if(temp==1)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
		}
		else if(temp==2)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
			Nixie_Display(2,Nodot_SMG[T_Limit[1]]);
		}
		else if(temp==3)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
			Nixie_Display(2,Nodot_SMG[T_Limit[1]]);
			Nixie_Display(6,Nodot_SMG[T_Limit[2]]);
		}
		else if(temp==4)
		{
			Nixie_Display(1,Nodot_SMG[T_Limit[0]]);
			Nixie_Display(2,Nodot_SMG[T_Limit[1]]);
			Nixie_Display(6,Nodot_SMG[T_Limit[2]]);
			Nixie_Display(7,Nodot_SMG[T_Limit[3]]);
		}
		else if(temp==0)
		{
			Nixie_Display(1,0XFF);
			Nixie_Display(2,0XFF);
			Nixie_Display(6,0XFF);
			Nixie_Display(7,0XFF);
		}
	}
	
	Nixie_All();		//��ֹSMG���һλ������
}

void USE_LED(void)
{
	if(L2_State==1){LED_Open(0X02);}
	else{LED_Close(0X02);}
	
	if(L1_State==1){LED_Open(0X01);}
	else{LED_Close(0X01);}
}

void USE_RELAY(void)
{
	if(RELAY_State==1){RELAY_Open();}
	else{RELAY_Close();}
}

/*��ʱ��0�жϺ�����10ms��һ���жϣ�*/
void Timer0_Routine(void) interrupt 1
{
	static unsigned char count;
	
	if(T_Scope==0)
	{
		count++;
		if(count==80)
		{
			count=0;
			RELAY_State=0;
			L1_State=~L1_State;
		}
	}
	else if(T_Scope==1)
	{
		count++;
		if(count==40)
		{
			count=0;
			RELAY_State=0;
			L1_State=~L1_State;
		}
	}
	else if(T_Scope==2)
	{
		count++;
		if(count==20)
		{
			count=0;
			RELAY_State=1;
			L1_State=~L1_State;
		}
	}
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	DS18B20_Convert();//�ϵ��һ����ת��һ�����ݣ���ֹһ�ϵ�����ܲ���������ʾ�������¶�����
	Delay1000ms();//�ȴ��¶�ת��������ϵ���ʾ
	while(1)
	{
		USE_DS18B20();
		USE_SMG();
		USE_MatrixKEY();
		USE_LED();
		USE_RELAY();
		if(L1_State==1){LED_Open(0X01);}
		else{LED_Close(0X01);}
	}
}