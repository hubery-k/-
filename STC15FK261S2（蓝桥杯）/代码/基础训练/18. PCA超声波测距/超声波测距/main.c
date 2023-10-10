#include <STC15F2K60S2.H>
#include <intrins.h>

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

#ifndef u32
#define u32 unsigned long
#endif


sbit Trig = P1^0;	
sbit Echo = P1^1;	

u8 code font[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
u8 code y4=0x80,y5=0xa0,y6=0xc0,y7=0xe0;
u8 dis[8]={0xc7,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

bit trig_sign=1,echo_sign=0,time_out_sign=0;     //1������һ�γ�������־λ    1������ɹ���־λ      1�����䳬ʱ��־λ 
u16 trig_cnt=1000;
u16 len,len_t;

void PCA_init();
void trig_len();
void echo_len();

void dis_smg();


void delay100us()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 39;
	do
	{
		while (--j);
	} while (--i);
}
void delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}
/*************��ʼ��PCA**************/
void PCA_init()
{
	P_SW1 &= 0xcf;	//(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
	CCON = 0;       //��ʼ��PCA������
						//PCA��ʱ��ֹͣ
						//�������жϱ�־
						//��������жϱ�־
	CL = 0;         //������мĴ���
	CH = 0;
	CMOD = 0x01;    //����PCAʱ��ԴΪSYSclk/12����������ж�
	CCAPM0 = 0x10;  //PCAģ���½��ش������ر��ж�
}

void Timer0Init(void)		//1ms@12.000MHz
{
	AUXR &= 0x7F;			
	TMOD &= 0xF0;			
	TL0 = 0x18;				
	TH0 = 0xFC;				
	TF0 = 0;				
	TR0 = 1;				
	
	ET0 = 1;				
}
/****************�жϴ�����********************/
void PCA_isr() interrupt 7		//PCA�жϴ�����
{	
	//����ɹ�
	if (CCF0)
	{
		len_t = (CCAP0H<<8)|CCAP0L;		//���汾�εĲ���ֵ
		echo_sign = 1;
		CR = 0;							//PCA��ʱ��ֹͣ����
		CCAPM0 &= 0xfe;					//�ر��ж�
	}
	//��ʱ
	else if (CF)
	{
		time_out_sign = 1;
		CR = 0;							//PCA��ʱ��ֹͣ����
		CCAPM0 &= 0xfe;					//�ر��ж�
	}
	CCF0 = 0;							//�����жϱ�־
	CF = 0;
}

void T0_isr() interrupt 1		//T0�жϴ�����,ÿ1000ms����һ�γ�����
{
	if(--trig_cnt == 0)
	{
		trig_cnt = 1000;
		trig_sign = 1;
	}
}

void main()
{
	Trig = 0;
	Timer0Init();
	PCA_init();	 
	EA = 1;
	while(1) 
	{
		dis_smg();
		if(trig_sign) trig_len();
		if(echo_sign | time_out_sign) echo_len();
	}
}

/*************************************************
����:trig_len()
*���ܣ����䳬����������PCA��ʱ���ж�
*************************************************/
void trig_len() 
{
	u8 i=8;
	while(i--)
	{
		Trig = 1;
		delay12us();
		Trig = 0;
		delay12us();
	}
	CL = 0;		 //��ʱ������				
	CH = 0;
	CCF0 = 0;				//�����жϱ�־
	CF = 0;
	CCAPM0 |= 0x01;		   //�����ж�			
	CR = 1;				  //PCA��ʱ������	
	
	trig_sign = 0;
}

/*************************************************
����:echo_len()
*���ܣ�������룬�����������ʾ
*************************************************/
void echo_len() 
{
	u8 i;
	if(echo_sign)            //����ɹ�
	{
		//�������
		len=len_t *0.017 *10;	//����һλС��
		
		dis[3]=font[len/10000];
		dis[4]=font[len/1000%10];
		dis[5]=font[len/100%10];
		dis[6]=font[len/10%10]&0x7f;
		dis[7]=font[len%10];
		//����
		for(i=3;dis[i]==font[0];i++) dis[i]=0xff;
	}
	else if(time_out_sign)      //��ʱ
	{
		//��ʱ����
		len=9999;
		dis[3]=font[9];
		dis[4]=font[9];
		dis[5]=font[9];
		dis[6]=font[9]&0x7f;
		dis[7]=font[9];
	}
	time_out_sign = 0;
	echo_sign = 0;
}

/*************************************************
*����:dis_smg()
*���ܣ��������ʾ
*************************************************/
void dis_smg() 
{
	u8 i;
	for(i=0;i<8;i++)
	{
		P2&=0x1f;
		P0=1<<i;
		P2|=y6;
		P2&=0x1f;
		P0=dis[i];
		P2|=y7;
		delay100us();
		P0=0xff;
	}
}
