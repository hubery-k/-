#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "KEY.h"
#include "Ds18B20.h"
#include "Timer.h"
#include "Delay.h"

/*使用到的硬件：LED、继电器、数码管、矩阵键盘、DS18B20温度传感器*/
unsigned char code Nodot_SMG[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char code dot_SMG[]={0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10};

unsigned char KeyNum;
unsigned char T_Int;
unsigned char T_Max=30;		//系统默认上限30（最大99）
unsigned char T_Min=20;		//系统默认下限20（最小0）
unsigned char T_Scope;		//温度区间（0~2）
unsigned char Interface;	//显示界面
unsigned char T_Limit[4];		//定义输入温度阈值数组
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
	LED_Control(0X00);		//关闭所有LED
	RB_Control(0X00);//关闭继电器蜂鸣器
	P2&=0X1F;//锁存
}

/*DS18B20温度读取与阈值判断函数*/
void USE_DS18B20(void)
{
	/*题目中要求温度数据保存为整数数据*/
	/*且温度阈值设置范围为0~99，所以不再进行正负温度判断*/
	
	if(T_flag==1)		//达到100ms读取一次温度数据
	{
		T_flag=0;
		DS18B20_Convert();		
		T_Int=DS18B20_ReadT();
	}
	
	if(T_Int<T_Min&&T_Max>T_Min){T_Scope=0;RELAY_State=0;L2_State=0;}
	else if(T_Int<=T_Max&&T_Int>=T_Min&&T_Max>T_Min){T_Scope=1;RELAY_State=0;L2_State=0;}
	else if(T_Int>T_Max&&T_Max>T_Min){T_Scope=2;RELAY_State=1;L2_State=0;}
	else if(T_Max<=T_Min){T_Scope=10;RELAY_State=0;L1_State=0;L2_State=1;}		//10-该数值没有使用表示错误的温度区间
}

/*按键触发函数*/
void USE_MatrixKEY(void)
{
	KeyNum=KEY();
	if(KeyNum==10)		//设置按键按下
	{
		if(Interface==0)
		{
				Interface=1;		//进入参数设置界面
		}		
		else if(Interface==1)
		{
			if(temp!=0)
			{
				T_Max=T_Limit[0]*10+T_Limit[1];		//保存温度数据
				T_Min=T_Limit[2]*10+T_Limit[3];
				
				temp=0;
				T_Limit[0]=T_Limit[1]=T_Limit[2]=T_Limit[3]=0;		//保存完温度数据也将输入参数清除
			}		
			if(T_Max<=T_Min)//输入温度数据出错，不返回温度显示界面，L2常亮
			{
					Interface=1;					
			}
			else		//返回温度显示界面
			{
					Interface=0;
			}		
		}		
	}
	
	else if(Interface==1)		//在参数设置界面下
	{
		if(KeyNum<10)		
		{
			if(temp<4)
			{
					T_Limit[temp++]=KeyNum;		//输入温度上下限
			}
		}
		if(KeyNum==11)		//清除按键按下
		{
			T_Limit[0]=T_Limit[1]=T_Limit[2]=T_Limit[3]=0;
			temp=0;
		}
	}
}

/*数码管显示函数*/
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

/*LED显示函数*/
void USE_LED(void)
{
	if(L1_State==1&&L2_State==0){LED_Control(0X01);}
	else if(L1_State==0&&L2_State==1){LED_Control(0X02);}
	else {LED_Control(0X00);}
}

/*继电器开关函数*/
void USE_RB(void)
{
	if(RELAY_State==1){RB_Control(0X10);}	//打开继电器
	else{RB_Control(0X00);}		//关闭继电器
}


/*定时器1中断函数（1ms进一次中断）*/
void Timer1_Routine(void) interrupt 3
{
	static unsigned char T_Count;
	/*扫描数码管和按键*/
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
	
			/*温度阈值的处理*/
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
		if(T0_Count>=200)		//（ >= ）大于等于的目的是以防切换过程中T_Count数值超过当前限定值
		{
			T0_Count=0;
			L1_State=~L1_State;
		}
	}
	T_Count++;
	if(T_Count==100)		//达到100ms
	{
		T_Count=0;
		T_flag=1;
	}
}

/*主函数*/
void main(void)
{
	Sys_Init();
	Timer1Init();
	DS18B20_Convert();
	Delay750ms();
	while(1)
	{
		EA=0;		//防止中断影响
		USE_LED();
		USE_RB();
		EA=1;
		USE_DS18B20();//100ms读取一次温度数据且与定时中断分开工作，不受定时中断影响。
		USE_SMG();
		USE_MatrixKEY();
	}
}

/*遇到的问题：*/
/*
	1. DS18B20单总线协议：上电第一次先转换一次数据存在暂存器中，此过程最高需要消耗750ms。
	为了避免不断使用延时，所以先上电转换一次温度数据存在暂存器中。然后在主循环中再次开启转换温度后再读取温度，此时读取的是第一次转换的温度。
*/
/*
	2. 由于软件模拟时序传输数据时会被定时中断影响，所以会使数码管程序出现闪烁现象。所以在读取温度数据时关闭中断且加上100ms稳定温度数据读取。
	解决方法：在中断函数中定义标志变量，当到达100ms后该标志置1在主循环中读取温度数据，这样可以省去100ms的延时且避免了与中断发送冲突*/
/*
	3. 在操作LED、继电器、蜂鸣器的时候，正好碰到定时器中断，导致P0口数据不稳定，导致LED、继电器、蜂鸣器异常。
	解决方法：运行到LED和继电器蜂鸣器的时候不打开中断。
*/