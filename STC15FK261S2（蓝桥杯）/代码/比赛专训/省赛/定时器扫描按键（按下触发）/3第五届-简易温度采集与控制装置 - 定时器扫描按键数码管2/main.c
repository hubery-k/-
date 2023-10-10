#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "KEY.h"
#include "Ds18B20.h"
#include "Timer.h"
#include "Delay.h"

/*ʹ�õ���Ӳ����LED���̵���������ܡ�������̡�DS18B20�¶ȴ�����*/
unsigned char code Nodot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code dot_SMG[]={0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10};

unsigned char KeyNum;
unsigned char T_Int;
unsigned char T_Max=30;		//ϵͳĬ������30�����99��
unsigned char T_Min=20;		//ϵͳĬ������20����С0��
unsigned char T_Scope;		//�¶����䣨0~2��
unsigned char Interface;	//��ʾ����
unsigned char T_Limit[4];		//���������¶���ֵ����
unsigned char temp;
unsigned int T0_Count;
unsigned char T1_count0;
unsigned char T1_count1;
unsigned char led_temp;

bit L2_State;
bit L1_State;
bit RELAY_State;
bit T_flag;
bit Key_flag;


void Sys_Init(void)
{
	LED_Control(0X00);		//�ر�����LED
	RB_Control(0X00);//�رռ̵���������
	P2&=0X1F;//����
}

/*DS18B20�¶ȶ�ȡ����ֵ�жϺ���*/
void USE_DS18B20(void)
{
	/*��Ŀ��Ҫ���¶����ݱ���Ϊ��������*/
	/*���¶���ֵ���÷�ΧΪ0~99�����Բ��ٽ��������¶��ж�*/
	
	if(T_flag==1)		//�ﵽ100ms��ȡһ���¶�����
	{
		T_flag=0;
		DS18B20_Convert();		
		T_Int=DS18B20_ReadT();
	}
	
	if(T_Int<T_Min&&T_Max>T_Min){T_Scope=0;RELAY_State=0;L2_State=0;}
	else if(T_Int<=T_Max&&T_Int>=T_Min&&T_Max>T_Min){T_Scope=1;RELAY_State=0;L2_State=0;}
	else if(T_Int>T_Max&&T_Max>T_Min){T_Scope=2;RELAY_State=1;L2_State=0;}
	else if(T_Max<=T_Min){T_Scope=10;RELAY_State=0;L1_State=0;L2_State=1;}		//10-����ֵû��ʹ�ñ�ʾ������¶�����
}

/*������������*/
void USE_MatrixKEY(void)
{
	KeyNum=KEY();
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
			if(T_Max<=T_Min)//�����¶����ݳ����������¶���ʾ���棬L2����
			{
					Interface=1;					
			}
			else		//�����¶���ʾ����
			{
					Interface=0;
			}		
		}		
	}
	
	else if(Interface==1)		//�ڲ������ý�����
	{
		if(KeyNum<10)		
		{
			if(temp<4)
			{
					T_Limit[temp++]=KeyNum;		//�����¶�������
			}
		}
		if(KeyNum==11)		//�����������
		{
			T_Limit[0]=T_Limit[1]=T_Limit[2]=T_Limit[3]=0;
			temp=0;
		}
	}
}

/*�������ʾ����*/
void USE_SMG(void)
{
	if(Interface==0)
	{
		Nixie_SetBuf(0,0XBF);
		Nixie_SetBuf(1,Nodot_SMG[T_Scope]);
		Nixie_SetBuf(2,0XBF);
		
		Nixie_SetBuf(6,Nodot_SMG[T_Int/10]);
		Nixie_SetBuf(7,Nodot_SMG[T_Int%10]);
	}
	else if(Interface==1)
	{
		Nixie_SetBuf(0,0XBF);
		Nixie_SetBuf(5,0XBF);
		if(temp==1)
		{
			Nixie_SetBuf(1,Nodot_SMG[T_Limit[0]]);
		}
		else if(temp==2)
		{
			Nixie_SetBuf(1,Nodot_SMG[T_Limit[0]]);
			Nixie_SetBuf(2,Nodot_SMG[T_Limit[1]]);
		}
		else if(temp==3)
		{
			Nixie_SetBuf(1,Nodot_SMG[T_Limit[0]]);
			Nixie_SetBuf(2,Nodot_SMG[T_Limit[1]]);
			Nixie_SetBuf(6,Nodot_SMG[T_Limit[2]]);
		}
		else if(temp==4)
		{
			Nixie_SetBuf(1,Nodot_SMG[T_Limit[0]]);
			Nixie_SetBuf(2,Nodot_SMG[T_Limit[1]]);
			Nixie_SetBuf(6,Nodot_SMG[T_Limit[2]]);
			Nixie_SetBuf(7,Nodot_SMG[T_Limit[3]]);
		}
		
		else if(temp==0)
		{
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(6,0XFF);
			Nixie_SetBuf(7,0XFF);
		}
	}
}

/*LED��ʾ����*/
void USE_LED(void)
{
	if(L1_State==1&&L2_State==0){LED_Control(0X01);}
	else if(L1_State==0&&L2_State==1){LED_Control(0X02);}
	else {LED_Control(0X00);}
}

/*�̵������غ���*/
void USE_RB(void)
{
	if(RELAY_State==1){RB_Control(0X10);}	//�򿪼̵���
	else{RB_Control(0X00);}		//�رռ̵���
}


/*��ʱ��1�жϺ�����1ms��һ���жϣ�*/
void Timer1_Routine(void) interrupt 3
{
	static unsigned char T_Count;
	/*ɨ������ܺͰ���*/
	T1_count0++;
	T1_count1++;
	if(T1_count0==2)
	{
		T1_count0=0;
		Nixie_Loop();
	}
	if(T1_count1==10)
	{
		T1_count1=0;
		KEY_Loop();
	}
	
			/*�¶���ֵ�Ĵ���*/
	if(T_Scope==0)
	{
		T0_Count++;
		if(T0_Count>=800)
		{
			T0_Count=0;
			L1_State=~L1_State;
		}
	}
	if(T_Scope==1)
	{
		T0_Count++;
		if(T0_Count>=400)
		{
			T0_Count=0;
			L1_State=~L1_State;
		}
	}
	if(T_Scope==2)
	{
		T0_Count++;
		if(T0_Count>=200)		//�� >= �����ڵ��ڵ�Ŀ�����Է��л�������T_Count��ֵ������ǰ�޶�ֵ
		{
			T0_Count=0;
			L1_State=~L1_State;
		}
	}
	T_Count++;
	if(T_Count==100)		//�ﵽ100ms
	{
		T_Count=0;
		T_flag=1;
	}
}

/*������*/
void main(void)
{
	Sys_Init();
	Timer1Init();
	DS18B20_Convert();
	Delay750ms();
	while(1)
	{
		EA=0;		//��ֹ�ж�Ӱ��
		USE_LED();
		USE_RB();
		EA=1;
		USE_DS18B20();//100ms��ȡһ���¶��������붨ʱ�жϷֿ����������ܶ�ʱ�ж�Ӱ�졣
		USE_SMG();
		USE_MatrixKEY();
	}
}

/*���������⣺*/
/*
	1. DS18B20������Э�飺�ϵ��һ����ת��һ�����ݴ����ݴ����У��˹��������Ҫ����750ms��
	Ϊ�˱��ⲻ��ʹ����ʱ���������ϵ�ת��һ���¶����ݴ����ݴ����С�Ȼ������ѭ�����ٴο���ת���¶Ⱥ��ٶ�ȡ�¶ȣ���ʱ��ȡ���ǵ�һ��ת�����¶ȡ�
*/
/*
	2. �������ģ��ʱ��������ʱ�ᱻ��ʱ�ж�Ӱ�죬���Ի�ʹ����ܳ��������˸���������ڶ�ȡ�¶�����ʱ�ر��ж��Ҽ���100ms�ȶ��¶����ݶ�ȡ��
	������������жϺ����ж����־������������100ms��ñ�־��1����ѭ���ж�ȡ�¶����ݣ���������ʡȥ100ms����ʱ�ұ��������жϷ��ͳ�ͻ*/
/*
	3. �ڲ���LED���̵�������������ʱ������������ʱ���жϣ�����P0�����ݲ��ȶ�������LED���̵������������쳣��
	������������е�LED�ͼ̵�����������ʱ�򲻴��жϡ�
*/