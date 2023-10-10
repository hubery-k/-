#include <STC15F2K60S2.H>
#include "LED.h"
#include "RB.h"
#include "KEY.h"
#include "Nixie.h"
#include "AD.h"
#include "Timer.h"
#include "Delay.h"


/*��������Ϊ�ڱ������н�����ܺͰ������붨ʱ���н���ɨ�衣���漰��������������whileѭ���л�������⡣
���磺��LED����ʾ����whileѭ���л����LED��������������̵�������while�л���ֲ�������Ȼ��Ͽ������󡢷�����Ҳ����ˡ�*/


unsigned char code Nodot_duanma[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code dot_duanma[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};
/*ʹ��Ӳ������������������ܡ�AD���̵�����LED*/

unsigned char KeyNum;
float adc_vol;									//ADת����ѹ��������
float F_Price;									//�۸񸡵�����ʽ��������
unsigned char W_Price;					//�۸�������ʽ��������
unsigned int Water_10ML;				//��ˮ������������
unsigned char count_2;						//�жϼ�����������
unsigned char count_1;						//�жϼ�����������
unsigned char count_0;						//�жϼ�����������
unsigned char SMG_State;				//�������ʾ״̬��������
bit Over;
bit RELAY_State;
bit BUZZER_State;
bit LED_State;
bit AD_flag;

/*�ر���������*/
void Sys_Init(void)
{
	LED_Control(0X00);
	RB_Control(0X00);
	P2&=0X1F;//����
}

/*����Ӧ�ú���*/
void USE_KEY(void)
{
	KeyNum=KEY();
	if(KeyNum==7)//S7(��ˮ��ť)����
	{
		RELAY_State=1;		//�̵�����
		SMG_State=1;		//����ܳ�ˮ״̬
		TR0=1;	//��ʼ��ʱ
	}
	else if(KeyNum==6||Over==1)//S6(ͣˮ��ť)����
	{
		RELAY_State=0;		//�̵����ر�
		SMG_State=2;		//�����ͣˮ״̬
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
	Nixie_SetBuf(1,dot_duanma[0]);
	Nixie_SetBuf(2,Nodot_duanma[5]);
	Nixie_SetBuf(3,Nodot_duanma[0]);

	/*��ʾ��ˮ��������ˮʱ�����ܼۣ�ͣˮʱ��*/
	if(SMG_State==1)//��ˮʱ
	{
		Nixie_SetBuf(4,Nodot_duanma[Water_10ML/1000]);
		Nixie_SetBuf(5,dot_duanma[(Water_10ML/100)%10]);
		Nixie_SetBuf(6,Nodot_duanma[(Water_10ML/10)%10]);
		Nixie_SetBuf(7,Nodot_duanma[Water_10ML%10]);
	}
	else if(SMG_State==2)//ͣˮʱ
	{
		Nixie_SetBuf(4,Nodot_duanma[W_Price/1000]);
		Nixie_SetBuf(5,dot_duanma[(W_Price/100)%10]);
		Nixie_SetBuf(6,Nodot_duanma[(W_Price/10)%10]);
		Nixie_SetBuf(7,Nodot_duanma[W_Price%10]);
	}
}

/*ADӦ��ת������*/
void USE_AD(void)
{
	if(AD_flag==1)
	{
		AD_flag=0;
		adc_vol=AD_Read()*(5.0/255);
	}
	if(adc_vol<1.25){LED_State=1;}
	else{LED_State=0;}
}

void USE_LED(void)
{
	if(LED_State==1){LED_Control(0X01);}
	else{LED_Control(0X00);}
}

void USE_RB(void)
{
	if(RELAY_State==1&&BUZZER_State==0){RB_Control(0X10);}
	else{RB_Control(0X00);}
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


/*��ʱ�жϺ���1��1ms����һ�Σ�*/
void Timer1_Routine(void) interrupt 3
{
	static unsigned char AD_Count;
	count_0++;
	count_1++;
	if(count_0==2)//2msɨ����ʾ�����1��
	{
		count_0=0;
		Nixie_Loop();
	}
	if(count_1==20)//20msɨ����ʾ����1��
	{
		count_1=0;
		KEY_Loop();
	}
	
	AD_Count++;
	if(AD_Count==50)		//�ﵽ50ms�ɼ�һ��AD����
	{
		AD_Count=0;
		AD_flag=1;
	}
}

/*�綨��Χ����*/
void Def_Scope(void)
{
	if(Water_10ML>=99)		//��ĿҪ��9999��Ϊ�˲���д����99
	{
		Over=1;
		TR0 = 0;
	}
}

void main()
{
	Sys_Init();
	Timer0Init();
	Timer1Init();
	while(1)
	{
		EA=0;		//��ֹ�ж�Ӱ��
		USE_RB();
		USE_LED();
		EA=1;
		USE_AD();
		Def_Scope();
		USE_SMG();
		USE_KEY();
	}
}

/*�����ж�����Ϊ������LED�������������������ܵ�ɨ��Ž��˶�ʱ�ж��У����ص��������л���LED��˸�����������ٶȹ������Ի����LED�����������*/