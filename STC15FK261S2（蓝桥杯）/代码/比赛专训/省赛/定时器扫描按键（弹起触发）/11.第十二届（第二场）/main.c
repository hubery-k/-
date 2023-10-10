#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "Timer.h"
#include "KEY.h"
#include "ADC.h"

uc code Nodot_SMG[]={~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uc code dot_SMG[]={~0xbf,~0x86,~0xdb,~0xcf,~0xe6,~0xed,~0xfd,~0x87,~0xff,~0xef,~0xf7,~0xfc,~0xb9,~0xde,~0xf9,~0xf1};
uc KeyNum;
f adc_value;		//�ɼ��ĵ�ѹֵ����
ui adc_IntValue;
uc Interface;		//����
uc Serial_Num=1;		//���(Ĭ��ֵΪ1)
f Save_V;		//����ĵ�ѹ����
ui Save_F;		//�����Ƶ������
ui PressCount;		//S7����ʱ�����
uli F;		//Ƶ��
uli F_temp;  //��ʱ����
uli T;		//����

bit AD_Flag;		//AD�ɼ���־λ��1-�ɼ���0-���ɼ���
bit PressTime_flag;		//S7�������̰��жϱ�־λ��1-������0-�ϰ���
bit LED_enable;		//LEDʹ�ܣ�0-ʹ�� 1-ʧ�ܣ�
void Sys_Init()
{
	LED_Control(0X00);		//�ر�����LED
	RB_Control(0X00);		//�رշ������̵���
}

void USE_PCF8591()
{
	if(AD_Flag==1)		//50ms�ɼ�һ��AD����
	{
		AD_Flag=0;
		if(Serial_Num==1)
		{
			adc_value=ADC_Read('1')*(5.0/255);
		}
		else if(Serial_Num==3)
		{
			adc_value=ADC_Read('3')*(5.0/255);
		}
	}

	adc_IntValue=adc_value*100;
}

void USE_KEY()
{
	KeyNum=KEY();
	if(KeyNum==4)		//S4-�����л�
	{
		Interface++;
		if(Interface==3){Interface=0;}
		if(Interface==2){Serial_Num=1;}		//�����ڽ�������ѹ���涼��ʾͨ��1
	}
	if(KeyNum==5)		//S5-��ѹͨ���л�����
	{
		if(Interface==2)		//�ڵ�ѹ������
		{
			if(Serial_Num==1){Serial_Num=3;}
			else if(Serial_Num==3){Serial_Num=1;}
		}
	}
	if(KeyNum==6)		//S6-ͨ��3��ѹ���水�����κν����¶���Ч��
	{
		if(Serial_Num==3)
		{
			Save_V=adc_value;		//����ͨ��3��ǰ��ѹֵ
		}
	}
	if(KeyNum==7 && PressTime_flag==1)		//S7-Ƶ�ʻ��水��-���������ܣ��κν����¶���Ч��
	{
		Save_F=F;		//���浱ǰƵ��ֵ
		LED_enable=~LED_enable;		//S7����LED����������
		PressCount=0;		//������°���ʱ�����ֵ
		PressTime_flag=0;
	}
	if(KeyNum==7 && PressTime_flag==0)		//S7-Ƶ�ʻ��水��-�̰������ܣ��κν����¶���Ч��
	{
		Save_F=F;
		PressCount=0;		//������°���ʱ�����ֵ
	}
}

void USE_SMG()
{
	if(Interface==0)		//Ƶ����ʾ����
	{
		Nixie_SetBuf(0,Nodot_SMG[15]);		//��ʶ-F
		if(F>999999){Nixie_SetBuf(1,Nodot_SMG[F/1000000]);}
		else{Nixie_SetBuf(1,0XFF);}
		
		if(F>99999){Nixie_SetBuf(2,Nodot_SMG[F/100000%10]);}
		else{Nixie_SetBuf(2,0XFF);}
		
		if(F>9999){Nixie_SetBuf(3,Nodot_SMG[F/10000%10]);}
		else{Nixie_SetBuf(3,0XFF);}
		
		if(F>999){Nixie_SetBuf(4,Nodot_SMG[F/1000%10]);}
		else{Nixie_SetBuf(4,0XFF);}
		
		if(F>99){Nixie_SetBuf(5,Nodot_SMG[F/100%10]);}
		else{Nixie_SetBuf(5,0XFF);}
		
		if(F>9){Nixie_SetBuf(6,Nodot_SMG[F/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}
		
		Nixie_SetBuf(7,Nodot_SMG[F%10]);
	}
	else if(Interface==1)		//������ʾ����
	{
		Nixie_SetBuf(0,0XC8);		//��ʶ-��������U
		if(T>999999){Nixie_SetBuf(1,Nodot_SMG[T/1000000]);}
		else{Nixie_SetBuf(1,0XFF);}
		
		if(T>99999){Nixie_SetBuf(2,Nodot_SMG[T/100000%10]);}
		else{Nixie_SetBuf(2,0XFF);}
		
		if(T>9999){Nixie_SetBuf(3,Nodot_SMG[T/10000%10]);}
		else{Nixie_SetBuf(3,0XFF);}
		
		if(T>999){Nixie_SetBuf(4,Nodot_SMG[T/1000%10]);}
		else{Nixie_SetBuf(4,0XFF);}
		
		if(T>99){Nixie_SetBuf(5,Nodot_SMG[T/100%10]);}
		else{Nixie_SetBuf(5,0XFF);}
		
		if(T>9){Nixie_SetBuf(6,Nodot_SMG[T/10%10]);}
		else{Nixie_SetBuf(6,0XFF);}
		
		Nixie_SetBuf(7,Nodot_SMG[T%10]);
	}
	else if(Interface==2)		//��ѹ��ʾ����
	{
		Nixie_SetBuf(0,0XC1);		//��ʶ-U
		Nixie_SetBuf(1,0XBF);		//��ʶ- -
		Nixie_SetBuf(2,Nodot_SMG[Serial_Num]);
		Nixie_SetBuf(3,0XFF);
		Nixie_SetBuf(4,0XFF);
		Nixie_SetBuf(5,dot_SMG[adc_IntValue/100]);
		Nixie_SetBuf(6,Nodot_SMG[adc_IntValue/10%10]);
		Nixie_SetBuf(7,Nodot_SMG[adc_IntValue%10]);
	}

}

void USE_LED()
{
	if(LED_enable==0)
	{
		LED_Init();
		if(Serial_Num==3 && adc_value>Save_V){L1=0;}		//ͨ��3��ǰ��ѹֵ��������ĵ�ѹֵ
		else{L1=1;}
		if(F>Save_F){L2=0;}		//��ǰƵ��ֵ���������Ƶ��ֵ
		else{L2=1;}
			
		if(Interface==0){L3=0;L4=1;L5=1;}		//Ƶ�ʽ�����
		if(Interface==1){L4=0;L3=1;L5=1;}		//���ڽ�����
		if(Interface==2){L5=0;L4=1;L3=1;}		//��ѹ������
		P2&=0X1F;		//����
	}
	else if(LED_enable==1)
	{
		LED_Control(0X00);		//�ر�����LED
	}
}
/*�ⲿ�����жϺ���*/
void Timer0_Routine() interrupt 1
{
	F_temp++;	//�ⲿ����һ������ͻ���������ж�
}

/*1ms�Ķ�ʱ�жϺ���*/
void Timer1_Routine() interrupt 3
{
	static uc T1_Count0;		//�����ɨ��ʱ�����
	static uc T1_Count1;		//��������ʱ�����
	static uc AD_Count;		//AD�ɼ�ʱ�����
	static ui F_Count;		//��NE555��Ƶ�ʲɼ�ʱ�����
	T1_Count0++;T1_Count1++;
	if(T1_Count0==2)
	{
		T1_Count0=0;
		Nixie_Loop();
	}
	if(T1_Count1==10)
	{
		T1_Count1=0;
		KEY_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)		//50ms�ɼ�һ��AD����
	{
		AD_Count=0;
		AD_Flag=1;
	}
	
	F_Count++;
	if(F_Count==1000)		//�ﵽ1s
	{
		F=F_temp;
		T=1000000/F;		//(1000000��λΪus����ΪT�ĵ�λΪus)
		F_temp=0;
		F_Count=0;
	}

}
/*1ms�Ķ�ʱ�жϺ���*/
void Timer2_Routine() interrupt 12
{
	PressCount++;
	if(PressCount>1000){PressTime_flag=1;AUXR&=~0x10;}		//�ر�T2��ʱ������Ҫ�Ե�����λ���������ֱ��AUXR|=0X00;�����������Ӱ��������ʱ��
}

void main(void)
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	while(1)
	{
		EA=0;		
		USE_LED();
		EA=1;
		USE_KEY();
		USE_SMG();
		USE_PCF8591();
	}
}

/*Ϊ�˷�ֹ�жϸ���LED����ʾ�������ڵ���LED������ʱ��ر��ж�*/

/*���³������S7��������ʱ��*/
//		Nixie_SetBuf(0,Nodot_SMG[PressCount/1000]);
//		Nixie_SetBuf(1,Nodot_SMG[PressCount/100%10]);
//		Nixie_SetBuf(2,Nodot_SMG[PressCount/10%10]);
//		Nixie_SetBuf(3,Nodot_SMG[PressCount%10]);