#include <STC15F2K60S2.H>
#include "LED.h"
#include "RELAY.h"
#include "BUZZER.h"
#include "KEY.h"
#include "Nixie.h"
#include "AD.h"
#include "Timer.h"
#include "Delay.h"

unsigned char code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};
/*ʹ��Ӳ������������������ܡ�AD���̵�����LED*/


float adc_vol;									//ADת����ѹ��������
//unsigned int smg_vol;
float F_Price;									//�۸񸡵�����ʽ��������
unsigned char W_Price;					//�۸�������ʽ��������
unsigned int Water_10ML;				//��ˮ������������
unsigned char count_2;						//�жϼ�����������
unsigned char count_1;						//�жϼ�����������
unsigned char count_0;						//�жϼ�����������
unsigned char SMG_State;				//�������ʾ״̬��������
bit Over;

/*�ر���������*/
void Sys_Init(void)
{
	LED_Close();
	BUZZER_Close();
	RELAY_Close();
	Nixie_NoDisplay();
	P2&=0X1F;//����
}

/*����Ӧ�ú���*/
void USE_KEY(void)
{
	if(KeyNum==4)//S7(��ˮ��ť)����
	{
		RELAY_Open();		//�̵�����
		BUZZER_Close();
		SMG_State=1;		//����ܳ�ˮ״̬
		KeyNum=0;		//�������״̬
		TR0=1;	//��ʼ��ʱ
	}
	else if(KeyNum==3||Over==1)//S6(ͣˮ��ť)����
	{
		RELAY_Close();		//�̵����ر�
		BUZZER_Close();
		
		SMG_State=2;		//�����ͣˮ״̬
		KeyNum=0;//�������״̬
		
		count_2=0;
		Water_10ML=0;
		TR0=0;	//ֹͣ��ʱ
		Over=0;
	}
}

/*�����Ӧ����ʾ����*/
void USE_SMG(void)
{
	/*��ʾ����*/
	Nixie_Display(1,dot_duanma[0]);
	Nixie_Display(2,Nodot_duanma[5]);
	Nixie_Display(3,Nodot_duanma[0]);
	
	/*��ʾ��ˮ��������ˮʱ�����ܼۣ�ͣˮʱ��*/
	if(SMG_State==1)//��ˮʱ
	{
		Nixie_Display(4,Nodot_duanma[Water_10ML/1000]);
		Nixie_Display(5,dot_duanma[(Water_10ML/100)%10]);
		Nixie_Display(6,Nodot_duanma[(Water_10ML/10)%10]);
		Nixie_Display(7,Nodot_duanma[Water_10ML%10]);
	}
	else if(SMG_State==2)//ͣˮʱ
	{
		Nixie_Display(4,Nodot_duanma[W_Price/1000]);
		Nixie_Display(5,dot_duanma[(W_Price/100)%10]);
		Nixie_Display(6,Nodot_duanma[(W_Price/10)%10]);
		Nixie_Display(7,Nodot_duanma[W_Price%10]);
	}
}

/*ADӦ��ת������*/
void USE_AD(void)
{
	adc_vol=AD_Read()*(5.0/255);
	if(adc_vol<1.25){LED_Open(0X01);}
	else{LED_Close();}
	
	/*
	//ͨ������ܲ鿴������ѹֵ
	smg_vol=adc_vol*100;
	Nixie_Display(4,dot_duanma[smg_vol/100]);
	Nixie_Display(5,Nodot_duanma[smg_vol/10%10]);
	Nixie_Display(6,Nodot_duanma[smg_vol%10]);
	Nixie_Display(7,0XC1);
	*/
}

/*��ʱ�жϺ���0��10ms����һ�Σ�*/
void Timer0_Routine(void) interrupt 1
{
	count_2++;
	if(count_2>=10)
	{
		count_2=0;
		Water_10ML+=1;	//100ms-10ML
	}
		//�۸�ת��
	F_Price=Water_10ML*0.005;
	W_Price=F_Price*100;

}


/*�綨��Χ����*/
void Def_Scope(void)
{
	if(Water_10ML>=9999)
	{
		Over=1;
		TR0 = 0;
	}
}

void main()
{
	Sys_Init();
	Timer0Init();

	while(1)
	{
		KEY_Press();
		USE_KEY();
		USE_SMG();
		USE_AD();
		Def_Scope();
	}
}
