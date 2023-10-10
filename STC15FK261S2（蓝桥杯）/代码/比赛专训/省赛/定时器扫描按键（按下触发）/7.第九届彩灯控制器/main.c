#include "LED.h"
#include "RB.h"
#include "Nixie.h"
#include "KEY.h"
#include "Timer.h"
#include "AT24C02.h"
#include "PCF8591.h"
#include "Delay.h"

/*SMG段码:0-F*/
uc code Nodot_SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};	
uc code dot_SMG[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};

f adc_value;		//AD采集的电压值
uc KeyNum;
uc Interface;		//显示界面（0-SMG熄灭）
uc S6_Set;
ui Flow_Interval;		//流转间隔（400ms~1200ms）
uc Compare;		//用于PWM的比较值
uc Running_Mode=1;		//运行模式编号
uc Light_Grade;		//LED亮度等级
uc Work_State=1;	//(工作状态1~4)
uc i;
ui FlowCount;		//流转状态计数
ui Temp;		//临时变量-用于获取并判断EEPROM的数值符合规定

bit SW;		//0-流转启动  1-流转停止	
bit garde_flag;
bit blink_flag;	//间隔闪烁
bit LED_State;	//（0-灭，1-亮）-用于电位器控制的PWM灯光
bit blink_State;	//用于设置0.8s闪烁
bit Save_flag;		//1-写入EEPROM
bit AD_flag;		//1-AD开始采集（50ms采集一次）

/*硬件初始化函数*/
void Hardware_Init(void)
{
	LED_Init();
	RB_Init();
	Timer0Init();
	
	Temp=AT24C02_ReadByte(0X01)*10;
	if(Temp>=400&&Temp<=1200){Flow_Interval=Temp;}
	else{Flow_Interval=400;}
}

/*PCF8591应用函数*/
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

/*AT24C02应用函数*/
void USE_AT24C02()
{
	if(Save_flag==1)
	{
		Save_flag=0;
		//由于一个地址最大只能存取数值为256的数，所以这里进行除10再存储
		AT24C02_WriteByte(0X01,Flow_Interval/10);
		Delay5ms();
	}
}

/*LED应用函数*/
void USE_LED(void)
{
	LED_Init();
	if(LED_State==1)
	{
		if(Running_Mode==1)		//流转模式1
		{
			P0=0XFF<<i;
		}
		else if(Running_Mode==2)		//流转模式2
		{
			P0=0XFF>>i;
		}
		else if(Running_Mode==3)		//流转模式3
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
		else if(Running_Mode==4)		//流转模式4
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

/*按键应用函数*/
void USE_KEY(void)
{
	KeyNum=Key();
	if(KeyNum==7)		//S7-启动、停止按键
	{
		SW=~SW;
	}
	if(KeyNum==6)		//S6-设置按键
	{
		Interface=1;		//进入“流转间隔”设置界面
		blink_flag=1;
		S6_Set++;
		if(S6_Set==3){S6_Set=0;Interface=0;blink_flag=0;Save_flag=1;}	//退出设置界面并保存”流转间隔“参数在EEPROM中
	}
	
	if(Interface==1)	//在设置界面下
	{
		if(KeyNum==5)		//S5-（设置界面下）-加按键
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
		if(KeyNum==4)	//S4-（设置界面下）-减按键
		{
			i=0;
			FlowCount=0;	//防止切换时LED状态卡死
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
	
	if(Interface==0)	//S4-非设置状态下 显示亮度等级
	{
		if(Cont&0X08){garde_flag=1;}	//S4按下
		else{garde_flag=0;}	//S4松开
	}
}


/*SMG应用函数*/
void USE_SMG(void)
{
	if(Interface==1)		//设置界面下的显示
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
	
	else if(Interface==0)	//非设置界面下的显示
	{
		if(garde_flag==1)		//表示S4按下
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

/*1ms的定时中断函数*/
void Timer0_Routine(void) interrupt 1
{
	static uc T0_Count0;
	static uc T0_Count1;
	static ui Blink_Count;		//PWM亮度控制计数
	static uc AD_Count;
	static uc PwmCount;
	
	/*定时器扫描按键、数码管*/
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
	
	/*设置界面下0.8s的闪烁*/
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
	
	/*50ms采集一次AD*/
	AD_Count++;					
	if(AD_Count==50)		
	{
		AD_Count=0;
		AD_flag=1;
	}
	
	/*AD采集电位器的值控制LED亮度（PWM）*/
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

	/*LED流转模式与状态的切换*/
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
