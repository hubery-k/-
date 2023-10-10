#include "LED.h"
#include "RELAY.h"
#include "BUZZER.h"
#include "Nixie.h"
#include "AD.h"
#include "Clock.h"
#include "EEPROM.h"
#include "KEY.h"
#include "Delay.h"

/*������ʹ��Ӳ����LED�����������̵���������ܡ�AD(PCF8591)��EEPROM(AT24C02)��������DS1302ʱ��*/


unsigned char code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

/*BCD��ĺô���ʱ���϶Ա�16���ƿ�������ֱ�ۣ�8ʱ30��0�룩*/
/*����ʱ������鲻�ܼ���code����ROM�У���Ϊ��ȡ��ʱ����һֱ���ĵġ�����ROM(����洢��)�е����ݽ����ܱ�����*/
unsigned char Time[]={0X00,0X30,0X08};


float adc_vol;
unsigned char humidity;
unsigned char Hum_Limit=50;//ʪ����ֵ��ʼΪ50%
unsigned char Red_HumLimit;
//unsigned int smg_vol;
bit Wor_State;
bit Buz_State;
bit Rel_State;
bit S6_State;
bit Buz_flag;
void Sys_Init(void)
{
	RELAY_Close();
	BUZZER_Close();
	LED_Close();
	Nixie_NoDisplay();
	P2=(P2&0x1F)|0X00;//�������趼��ѡ��
}

void USE_SMG(void)
{
	//��ʼ��ʾ״̬
	if(S6_State==0)
	{
		Nixie_Display(0,Nodot_duanma[Time[2]/16]);
		Nixie_Display(1,Nodot_duanma[Time[2]%16]);
		Nixie_Display(2,0xBF);
		Nixie_Display(3,Nodot_duanma[Time[1]/16]);
		Nixie_Display(4,Nodot_duanma[Time[1]%16]);
		
		Nixie_Display(6,Nodot_duanma[humidity/10]);
		Nixie_Display(7,Nodot_duanma[humidity%10]);
		
	}
	
	
	//ʪ����ֵ�������棨�Զ�����ģʽ�£�
	if(S6_State==1)
	{
		Nixie_Display(0,0xBF);
		Nixie_Display(1,0xBF);
		Nixie_Display(6,Nodot_duanma[Hum_Limit/10]);
		Nixie_Display(7,Nodot_duanma[Hum_Limit%10]);
	}
}

void USE_AD(void)
{
	adc_vol=AD_Read()*(5.0/255);//ת��Ϊ0-5V��ѹֵ
	humidity=(adc_vol/5.0)*99;//ת��Ϊʪ��ֵ
}

void USE_KEY(void)
{
	if(KeyNum==4)//S7-ϵͳ����״̬�л�����(0-�Զ���1-�ֶ�)
	{
		if(Wor_State==0){Wor_State=1;}
		else if(Wor_State==1){Wor_State=0;}
		KeyNum=0;//����״̬���
	}
	
	/*S6��S5��S4�ֶ�����״̬��*/
	if(Wor_State==1)
	{
		if(KeyNum==3)//S6-�ر���򿪷��������ѹ���(0-�رգ�1-����)
		{
			if(Buz_flag==0){Buz_flag=1;}
			else if(Buz_flag==1){Buz_flag=0;}
			KeyNum=0;//����״̬���
		}
		else if(KeyNum==2)//S5-�򿪹��ϵͳ
		{
			Rel_State=1;
			KeyNum=0;
		}
		else if(KeyNum==1)//S4-�رչ��ϵͳ
		{
			Rel_State=0;
			KeyNum=0;
		}
	}
	
	/*S6��S5��S4�Զ�����״̬��*/
	else if(Wor_State==0)
	{
		if(KeyNum==3)//S6-ʪ����ֵ��������һ�ΰ��½���ʪ����ֵ�������棬�ڶ��ΰ��±�������ֵ��EEPROM�У�
		{
			if(S6_State==0){S6_State=1;}//(0-�����˻�����ҳ�棬1-ʪ����ֵ����ҳ��)
			else if(S6_State==1){S6_State=0;}
			KeyNum=0;
		}
		else if(KeyNum==2)//S5-ʪ����ֵ��1����ʪ����ֵ������������Ч��
		{
			if(S6_State==1)
			{
				Hum_Limit++;
			}
			KeyNum=0;
		}
		else if(KeyNum==1)//S4-ʪ����ֵ��1����ʪ����ֵ������������Ч��
		{
			if(S6_State==1)
			{
				Hum_Limit--;
			}
			KeyNum=0;
		}
	}
}

void Work_State(void)
{
	//�Զ�����״̬(ϵͳ�ϵ�Ĭ���Զ�����״̬)
	if(Wor_State==0)
	{
		LED_Open(0X01);//L1����
		Buz_State=0;//�Զ�״̬�¹رշ�����
		if(humidity<Hum_Limit){Rel_State=1;}
		else{Rel_State=0;}
	}
	//�ֶ�����״̬
	else if(Wor_State==1)
	{
		LED_Open(0X02);//L2����
		S6_State=0;//�ֶ�״̬�²���ʾ��ֵ����ҳ��
		if(humidity<Hum_Limit&&Buz_flag==0){Buz_State=1;}
		else if(Buz_flag==1||humidity>=Hum_Limit){Buz_State=0;}
	}
}

void USE_EEPROM(void)
{
	if(S6_State==1)//д������
	{
		AT24C02_WriteByte(0X01,Hum_Limit);//д�������5ms������ÿ��д�붼Ҫ��һ��5ms����ʱ
		Delay5ms();
	}
	else if(S6_State==0)//��ȡ����
	{
		Red_HumLimit=AT24C02_Read(0X01);
		Hum_Limit=Red_HumLimit;
	}
}

void USE_BUZZER(void)
{
	if(Buz_State==1){BUZZER_Open();}
	else if(Buz_State==0){BUZZER_Close();}
}
void USE_RELAY(void)
{
	if(Rel_State==1){RELAY_Open();}
	else if(Rel_State==0){RELAY_Close();}
}


void main(void)
{
	Sys_Init();
	Set_Time(Time);
	while(1)
	{
		Read_Time(Time);
		KEY_Press();
		USE_KEY();
		USE_RELAY();
		USE_BUZZER();
		USE_AD();
		USE_SMG();
		Work_State();
		USE_EEPROM();
	}
}


	/*
	AD�ɼ�������ͨ���������ʾ
	smg_vol=adc_vol*100;

	Nixie_Display(1,Nodot_duanma[][smg_vol/100]);
	Nixie_Display(2,dot_duanma[][(smg_vol/10)%10]);
	Nixie_Display(3,dot_duanma[][smg_vol%10]);
	*/