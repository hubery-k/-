#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "AD.h"
#include "Clock.h"
#include "EEPROM.h"
#include "KEY.h"
#include "Timer.h"
#include "Delay.h"

/*������ʹ��Ӳ����LED�����������̵���������ܡ�AD(PCF8591)��EEPROM(AT24C02)��������DS1302ʱ��*/
typedef unsigned int uint;
typedef unsigned char uchar;

uchar code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
uchar code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

/*BCD��ĺô���ʱ���϶Ա�16���ƿ�������ֱ�ۣ�8ʱ30��0�룩*/
/*����ʱ������鲻�ܼ���code����ROM�У���Ϊ��ȡ��ʱ����һֱ���ĵġ�����ROM(����洢��)�е����ݽ����ܱ�����*/
uchar Time[]={0X00,0X30,0X08};



uchar KeyNum;
float adc_vol;
uchar humidity;
uchar Hum_Limit=50;//ʪ����ֵ��ʼΪ50%
uchar Red_HumLimit;
uchar temp;

bit Wor_State;
bit Buz_State;
bit Rel_State;
bit S6_State;
bit Buz_flag=1;		//���������ѹ��ܣ�Ĭ�ϴ򿪣�
bit L1_State;
bit L2_State;

bit AD_flag;
bit C_flag;
bit Save_flag;

void Sys_Init(void)
{
	LED_Control(0X00);		//�ر�����LED
	RB_Control(0X00);//�رռ̵���������
	P2&=0X1F;//����
}

void USE_DS1302(void)
{
	if(C_flag==1)
	{
		C_flag=0;
		Read_Time(Time);
	}
}

void USE_AD(void)
{
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_vol=AD_Read()*(5.0/255);//ת��Ϊ0-5V��ѹֵ
		humidity=(adc_vol/5.0)*99;//ת��Ϊʪ��ֵ	��Ҫ�����������if�з���ˢ��̫��������⣩
	}
}

void USE_EEPROM(void)
{
	if(Save_flag==1)
	{
		Save_flag=0;
		AT24C02_WriteByte(0X01,Hum_Limit);//д�������5ms������ÿ��д�붼Ҫ��һ��5ms����ʱ
		Delay5ms();
	}
}

void USE_KEY(void)
{
	KeyNum=Key();
	
	if(KeyNum==7)//S7-ϵͳ����״̬�л�����(0-�Զ���1-�ֶ�)
	{
		Wor_State=~Wor_State;
	}
	
	/*S6��S5��S4�ֶ�����״̬��*/
	if(Wor_State==1)
	{
		if(KeyNum==6)//S6-�ر���򿪷��������ѹ���(0-�رգ�1-����)
		{
			Buz_flag=~Buz_flag;
		}
		else if(KeyNum==5)//S5-�򿪹��ϵͳ
		{
			Rel_State=1;
		}
		else if(KeyNum==4)//S4-�رչ��ϵͳ
		{
			Rel_State=0;
		}
	}
	
	/*S6��S5��S4�Զ�����״̬��*/
	else if(Wor_State==0)
	{
		if(KeyNum==6)//S6-ʪ����ֵ��������һ�ΰ��½���ʪ����ֵ�������棬�ڶ��ΰ��±�������ֵ��EEPROM�У�
		{
			S6_State=~S6_State;		//(0-�����˻�����ҳ�棬1-ʪ����ֵ����ҳ��)
			if(S6_State==0){Save_flag=1;}
		}
		else if(KeyNum==5)//S5-ʪ����ֵ��1����ʪ����ֵ������������Ч��
		{
			if(S6_State==1){Hum_Limit++;}
		}
		else if(KeyNum==4)//S4-ʪ����ֵ��1����ʪ����ֵ������������Ч��
		{
			if(S6_State==1){Hum_Limit--;}
		}
	}
}

void USE_SMG(void)
{
	//��ʼ��ʾ״̬
	if(S6_State==0)
	{
		Nixie_SetBuf(0,Nodot_duanma[Time[2]/16]);
		Nixie_SetBuf(1,Nodot_duanma[Time[2]%16]);
		Nixie_SetBuf(2,0xBF);
		Nixie_SetBuf(3,Nodot_duanma[Time[1]/16]);
		Nixie_SetBuf(4,Nodot_duanma[Time[1]%16]);
		Nixie_SetBuf(5,0xFF);
		Nixie_SetBuf(6,Nodot_duanma[humidity/10]);
		Nixie_SetBuf(7,Nodot_duanma[humidity%10]);
	}
	
	
	//ʪ����ֵ�������棨�Զ�����ģʽ�£�
	if(S6_State==1)
	{
		Nixie_SetBuf(0,0xBF);
		Nixie_SetBuf(1,0xBF);
		
		Nixie_SetBuf(2,0xFF);
		Nixie_SetBuf(3,0xFF);
		Nixie_SetBuf(4,0xFF);
		Nixie_SetBuf(5,0xFF);

		Nixie_SetBuf(6,Nodot_duanma[Hum_Limit/10]);
		Nixie_SetBuf(7,Nodot_duanma[Hum_Limit%10]);
	}
}

void Work_State(void)
{
	//�Զ�����״̬(ϵͳ�ϵ�Ĭ���Զ�����״̬)
	if(Wor_State==0)
	{
		L1_State=1;		//L1����
		L2_State=0;		//L2Ϩ��
		Buz_State=0;		//�Զ�״̬�¹رշ�����
		if(humidity<Hum_Limit){Rel_State=1;}
		else{Rel_State=0;}
	}
	//�ֶ�����״̬
	else if(Wor_State==1)
	{
		L2_State=1;		//L2����
		L1_State=0;		//L1Ϩ��
		S6_State=0;		//�ֶ�״̬�²���ʾ��ֵ����ҳ��
		if(humidity<Hum_Limit&&Buz_flag==1){Buz_State=1;}
		else if(humidity>=Hum_Limit||Buz_flag==0){Buz_State=0;}
	}
}


void USE_LED(void)
{
	LED_Init();
	
	if(L1_State==1){L1=0;}		//L1��
	else{L1=1;}
	if(L2_State==1){L2=0;}		//L1��
	else{L2=1;}
	
	P2&=0x1F;
}

void USE_RB(void)
{
	RB_Init();
	
	if(Rel_State==1){Relay=1;}
	else{Relay=0;}
	if(Buz_State==1){Buzzer=1;}
	else{Buzzer=0;}
	
	P2&=0X1F;		//����	
}

/*1ms�Ķ�ʱ�жϺ���*/
void Timer1_Routine(void) interrupt 3
{
	static unsigned char T1_Count1;
	static unsigned char T1_Count2;
	static unsigned char AD_Count;
	static unsigned char C_Count;
	
	T1_Count1++;T1_Count2++;
	if(T1_Count1==2)
	{
		T1_Count1=0;
		Nixie_Loop();
	}
	if(T1_Count2==10)
	{
		T1_Count2=0;
		Key_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)		//�ﵽ50ms�ɼ�AD����һ��
	{
		AD_Count=0;
		AD_flag=1;
	}
	
	C_Count++;
	if(C_Count==50)		//�ﵽ50ms�ɼ�ʱ������һ��
	{
		C_Count=0;
		C_flag=1;
	}
}

void main(void)
{
	Sys_Init();
	Set_Time(Time);
	temp=AT24C02_Read(0X01);
	if(temp>0&&temp<100){Hum_Limit=temp;}	//��ֹ�����ϵ��ȡ��ֵ�ֵ
	else{Hum_Limit=50;}
	Timer1Init();
	while(1)
	{
		EA=0;
		USE_RB();
		USE_LED();
		EA=1;
		USE_AD();
		USE_EEPROM();
		USE_DS1302();
		USE_KEY();
		USE_SMG();
		Work_State();
	}
}


/*���ֵ����⼰���������*/

/*
	1. �ڲ���LED���̵�������������ʱ������������ʱ���жϣ�����P0�����ݲ��ȶ�������LED���̵������������쳣��
	������������е�LED�ͼ̵�����������ʱ�򲻴��жϡ�
*/

/*
	2.�жϻ�Ӱ�������ģ��ʱ������ݴ��䡣����������ڵ���ʱ��������ݴ���ʱ���ж�EA��0�رգ��ȴ����ݶ�ȡ��д��ɹ�����1�������жϡ�
��õĽ�������ǣ����ж��ж����־λ�����ﵽһ��ʱ����־λ��1���������������жϵ���־λΪ1ʱ��ȡ���ݡ���������ʡȥDelay�Ĺ��̻��������жϳ�ͻ��
*/