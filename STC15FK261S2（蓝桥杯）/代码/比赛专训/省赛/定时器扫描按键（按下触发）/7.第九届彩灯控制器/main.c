#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "KEY.h"
#include "Timer.h"
#include "AT24C02.h"
#include "PCF8591.h"
#include "Delay.h"

/*SMG����:0-F*/
uc code Nodot_SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};	
uc code dot_SMG[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

f adc_value;		//AD�ɼ��ĵ�ѹֵ
uc KeyNum;
uc Interface;		//��ʾ���棨0-SMGϨ��
uc S6_Set;
ui Flow_Interval;		//��ת�����400ms~1200ms��
uc Compare;		//����PWM�ıȽ�ֵ
uc Running_Mode=1;		//����ģʽ���
uc Light_Grade;		//LED���ȵȼ�
uc Work_State=1;	//(����״̬1~4)
uc i;
ui FlowCount;		//��ת״̬����
ui Temp;		//��ʱ����-���ڻ�ȡ���ж�EEPROM����ֵ���Ϲ涨

bit SW;		//0-��ת����  1-��תֹͣ	
bit garde_flag;
bit blink_flag;	//�����˸
bit LED_State;	//��0-��1-����-���ڵ�λ�����Ƶ�PWM�ƹ�
bit blink_State;	//��������0.8s��˸
bit Save_flag;		//1-д��EEPROM
bit AD_flag;		//1-AD��ʼ�ɼ���50ms�ɼ�һ�Σ�

/*Ӳ����ʼ������*/
void Hardware_Init(void)
{
	LED_Init();
	RB_Init();
	Timer0Init();
	
	Temp=AT24C02_ReadByte(0X01)*10;
	if(Temp>=400&&Temp<=1200){Flow_Interval=Temp;}
	else{Flow_Interval=400;}
}

/*PCF8591Ӧ�ú���*/
void USE_PCF8591()
{
	if(AD_flag==1)
	{
		adc_value=PCF8591_ReadAD()*(5.0/255);
		
		if(adc_value>0&&adc_value<=1.2){Light_Grade=1;Compare=5;}
		else if(adc_value>1.2&&adc_value<=2.4){Light_Grade=2;Compare=10;}
		else if(adc_value>2.4&&adc_value<=3.6){Light_Grade=3;Compare=15;}
		else if(adc_value>3.6&&adc_value<=5.0){Light_Grade=4;Compare=20;}
	}
}

/*AT24C02Ӧ�ú���*/
void USE_AT24C02()
{
	if(Save_flag==1)
	{
		Save_flag=0;
		//����һ����ַ���ֻ�ܴ�ȡ��ֵΪ256����������������г�10�ٴ洢
		AT24C02_WriteByte(0X01,Flow_Interval/10);
		Delay5ms();
	}
}

/*LEDӦ�ú���*/
void USE_LED(void)
{
	LED_Init();
	if(LED_State==1)
	{
		if(Running_Mode==1)		//��תģʽ1
		{
			P0=0XFF<<i;
		}
		else if(Running_Mode==2)		//��תģʽ2
		{
			P0=0XFF>>i;
		}
		else if(Running_Mode==3)		//��תģʽ3
		{
			if(Work_State==1)
			{
				L1=0;L8=0;
				L2=1;L3=1;L4=1;L5=1;L6=1;L7=1;
			}
			else if(Work_State==2)
			{
				L2=0;L7=0;
				L1=1;L3=1;L4=1;L5=1;L6=1;L8=1;
			}
			else if(Work_State==3)
			{
				L3=0;L6=0;
				L1=1;L2=1;L4=1;L5=1;L7=1;L8=1;
			}
			else if(Work_State==4)
			{
				L4=0;L5=0;
				L1=1;L2=1;L3=1;L6=1;L7=1;L8=1;
			}
		}
		else if(Running_Mode==4)		//��תģʽ4
		{
			if(Work_State==1)
			{
				L4=0;L5=0;
				L1=1;L2=1;L3=1;L6=1;L7=1;L8=1;
			}
			else if(Work_State==2)
			{
				L3=0;L6=0;
				L1=1;L2=1;L4=1;L5=1;L7=1;L8=1;
			}
			else if(Work_State==3)
			{
				L2=0;L7=0;
				L1=1;L3=1;L4=1;L5=1;L6=1;L8=1;
			}
			else if(Work_State==4)
			{
				L1=0;L8=0;
				L2=1;L3=1;L4=1;L5=1;L6=1;L7=1;
			}
		}
	}
	else
	{
		LED_Control(0XFF);
	}
	
	P2&=0X1F;
}	

/*����Ӧ�ú���*/
void USE_KEY(void)
{
	KeyNum=Key();
	if(KeyNum==7)		//S7-������ֹͣ����
	{
		SW=~SW;
	}
	if(KeyNum==6)		//S6-���ð���
	{
		Interface=1;		//���롰��ת��������ý���
		blink_flag=1;
		S6_Set++;
		if(S6_Set==3){S6_Set=0;Interface=0;blink_flag=0;Save_flag=1;}	//�˳����ý��沢���桱��ת�����������EEPROM��
	}
	
	if(Interface==1)	//�����ý�����
	{
		if(KeyNum==5)		//S5-�����ý����£�-�Ӱ���
		{
			i=0;
			FlowCount=0;
			Work_State=0;
			
			if(S6_Set==1)
			{
				Running_Mode+=1;
				if(Running_Mode==5){Running_Mode=1;}
			}
			else if(S6_Set==2)
			{
				Flow_Interval+=100;
				if(Flow_Interval>=1300){Flow_Interval=400;}
			}
		}
		if(KeyNum==4)	//S4-�����ý����£�-������
		{
			i=0;
			FlowCount=0;	//��ֹ�л�ʱLED״̬����
			Work_State=0;
			
			if(S6_Set==1)
			{
				Running_Mode-=1;
				if(Running_Mode==0){Running_Mode=4;}
			}
			else if(S6_Set==2)
			{
				Flow_Interval-=100;
				if(Flow_Interval<=300){Flow_Interval=1200;}
			}
		}
	}
	
	if(Interface==0)	//S4-������״̬�� ��ʾ���ȵȼ�
	{
		if(Cont&0X08){garde_flag=1;}	//S4����
		else{garde_flag=0;}	//S4�ɿ�
	}
}


/*SMGӦ�ú���*/
void USE_SMG(void)
{
	if(Interface==1)		//���ý����µ���ʾ
	{
		if(S6_Set==1)
		{
			Nixie_SetBuf(0,0XBF);
			if(blink_State==1){Nixie_SetBuf(1,Nodot_SMG[Running_Mode]);}
			else{Nixie_SetBuf(1,0XFF);}
			Nixie_SetBuf(2,0XBF);
			
			Nixie_SetBuf(3,0XFF);
			if(Flow_Interval>999){Nixie_SetBuf(4,Nodot_SMG[Flow_Interval/1000]);}
			else{Nixie_SetBuf(4,0XFF);}
			if(Flow_Interval>99){Nixie_SetBuf(5,Nodot_SMG[(Flow_Interval/100)%10]);}
			else{Nixie_SetBuf(5,0XFF);}
			if(Flow_Interval>9){Nixie_SetBuf(6,Nodot_SMG[(Flow_Interval/10)%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			Nixie_SetBuf(7,Nodot_SMG[Flow_Interval%10]);
		}
		else if(S6_Set==2)
		{
			if(blink_State==1)
			{
				if(Flow_Interval>999){Nixie_SetBuf(4,Nodot_SMG[Flow_Interval/1000]);}
				else{Nixie_SetBuf(4,0XFF);}
				if(Flow_Interval>99){Nixie_SetBuf(5,Nodot_SMG[(Flow_Interval/100)%10]);}
				else{Nixie_SetBuf(5,0XFF);}
				if(Flow_Interval>9){Nixie_SetBuf(6,Nodot_SMG[(Flow_Interval/10)%10]);}
				else{Nixie_SetBuf(6,0XFF);}
				Nixie_SetBuf(7,Nodot_SMG[Flow_Interval%10]);
			}
			else
			{
				Nixie_SetBuf(4,0XFF);
				Nixie_SetBuf(5,0XFF);
				Nixie_SetBuf(6,0XFF);
				Nixie_SetBuf(7,0XFF);
			}
			
			Nixie_SetBuf(0,0XBF);
			Nixie_SetBuf(2,0XBF);
			Nixie_SetBuf(1,Nodot_SMG[Running_Mode]);
		}
		else if(S6_Set==0)
		{
			Nixie_SetBuf(0,0XFF);Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);Nixie_SetBuf(7,0XFF);
		}
	}
	
	else if(Interface==0)	//�����ý����µ���ʾ
	{
		if(garde_flag==1)		//��ʾS4����
		{
			Nixie_SetBuf(6,0XBF);
			Nixie_SetBuf(7,Nodot_SMG[Light_Grade]);
		}
		else
		{
			Nixie_SetBuf(0,0XFF);Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,0XFF);Nixie_SetBuf(7,0XFF);
		}
	}
}


void main(void)
{
	Hardware_Init();
	while(1)
	{
		EA=0;
		USE_LED();
		EA=1;
		USE_SMG();
		USE_KEY();
		USE_PCF8591();
		USE_AT24C02();
	}
}

/*1ms�Ķ�ʱ�жϺ���*/
void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count0;
	static uc T0_Count1;
	static ui Blink_Count;		//PWM���ȿ��Ƽ���
	static uc AD_Count;
	static uc PwmCount;
	
	/*��ʱ��ɨ�谴���������*/
	T0_Count0++;
	T0_Count1++;
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
	
	/*���ý�����0.8s����˸*/
	if(blink_flag==1)
	{
		Blink_Count++;
		if(Blink_Count==800)
		{
			Blink_Count=0;
			blink_State=~blink_State;
		}
	}
	else{Blink_Count=0;blink_State=0;}
	
	/*50ms�ɼ�һ��AD*/
	AD_Count++;					
	if(AD_Count==50)		
	{
		AD_Count=0;
		AD_flag=1;
	}
	
	/*AD�ɼ���λ����ֵ����LED���ȣ�PWM��*/
	PwmCount++;
	if(PwmCount<Compare)
	{
		LED_State=1;
	}
	else if(PwmCount>=Compare)
	{
		LED_State=0;
	}
	PwmCount%=20;

	/*LED��תģʽ��״̬���л�*/
	if(SW==0)
	{
		FlowCount++;
		if(FlowCount==Flow_Interval)
		{
			FlowCount=0;
			if(Running_Mode==1 || Running_Mode==2)
			{
				i++;
				if(i==9){i=0;Running_Mode++;}
			}
			else if(Running_Mode==3 || Running_Mode==4)
			{
				Work_State++;
				if(Work_State==5){Work_State=1;Running_Mode++;}
				if(Running_Mode==5){Running_Mode=1;}
			}
		}
	}
}
