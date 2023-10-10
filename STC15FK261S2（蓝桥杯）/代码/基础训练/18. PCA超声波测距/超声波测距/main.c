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

bit trig_sign=1,echo_sign=0,time_out_sign=0;     //1：发射一次超声波标志位    1：捕获成功标志位      1：发射超时标志位 
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
/*************初始化PCA**************/
void PCA_init()
{
	P_SW1 &= 0xcf;	//(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
	CCON = 0;       //初始化PCA控制器
						//PCA定时器停止
						//清除溢持中断标志
						//清除捕获中断标志
	CL = 0;         //清除阵列寄存器
	CH = 0;
	CMOD = 0x01;    //设置PCA时钟源为SYSclk/12，允许溢出中断
	CCAPM0 = 0x10;  //PCA模块下降沿触发，关闭中断
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
/****************中断处理函数********************/
void PCA_isr() interrupt 7		//PCA中断处理函数
{	
	//捕获成功
	if (CCF0)
	{
		len_t = (CCAP0H<<8)|CCAP0L;		//保存本次的捕获值
		echo_sign = 1;
		CR = 0;							//PCA定时器停止工作
		CCAPM0 &= 0xfe;					//关闭中断
	}
	//超时
	else if (CF)
	{
		time_out_sign = 1;
		CR = 0;							//PCA定时器停止工作
		CCAPM0 &= 0xfe;					//关闭中断
	}
	CCF0 = 0;							//清理中断标志
	CF = 0;
}

void T0_isr() interrupt 1		//T0中断处理函数,每1000ms发射一次超声波
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
函数:trig_len()
*功能：发射超声波，开启PCA计时及中断
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
	CL = 0;		 //计时器清零				
	CH = 0;
	CCF0 = 0;				//清理中断标志
	CF = 0;
	CCAPM0 |= 0x01;		   //开启中断			
	CR = 1;				  //PCA定时器工作	
	
	trig_sign = 0;
}

/*************************************************
函数:echo_len()
*功能：计算距离，在数码管上显示
*************************************************/
void echo_len() 
{
	u8 i;
	if(echo_sign)            //发射成功
	{
		//距离计算
		len=len_t *0.017 *10;	//保留一位小数
		
		dis[3]=font[len/10000];
		dis[4]=font[len/1000%10];
		dis[5]=font[len/100%10];
		dis[6]=font[len/10%10]&0x7f;
		dis[7]=font[len%10];
		//消零
		for(i=3;dis[i]==font[0];i++) dis[i]=0xff;
	}
	else if(time_out_sign)      //超时
	{
		//超时设置
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
*函数:dis_smg()
*功能：数码管显示
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
