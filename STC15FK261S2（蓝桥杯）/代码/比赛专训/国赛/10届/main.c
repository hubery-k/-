#include "main.h"
#include "string.h"
#include <stdio.h>
/******变量定义区******/
uc Interface_Data;
uc KeyNum;
f T;
ui Distance;
ui Change_Num;
c T_P=30;	//温度参数（范围：0~99）
c D_P=35;	//距离参数（范围：0~99）
uc Send_Buf[20];
uc Rec_Buf[15];

bit Interface;	//父界面标志（数据界面、参数界面）
bit Interface_P;	//子界面标志
bit KEY_flag;	//按键键值读取标志
bit DS18B20_flag;	//温度数据采集标志
bit Sonic_flag;	//超声波测距采集标志
bit AT24C02_flag;	//写入EEPROM标志
bit P_State;	//参数变动状态
bit Rec_flag;	//串口发送标志
bit DAC_flag;	//DAC开关标志
bit K12_flag;	//K12长短按标志
bit K13_flag;	//K13长短按标志
bit L1_flag;	//L1亮灭标志
bit L2_flag;	//L2亮灭标志
bit led_run;
/******硬件初始化函数******/
void Hardware_Init()
{
	LED_Init();
	RB_Init();
	Timer0Init();
	Sonic_Init();
	UartInit();
	DS18B20_ConvertT();
	Delay750ms();
}

void Task_Processing()
{
	/*DS18B20任务处理*/
	if(DS18B20_flag==1)
	{
		DS18B20_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		/*L1*/
		if(T>T_P){L1_flag=1;}
		else{L1_flag=0;}
	}
	/*Sonic任务处理*/
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance=Sonic_Work();
		if(Distance>99){Distance=99;}
		/*L2*/
		if(Distance<D_P){L2_flag=1;}
		else{L2_flag=0;}
	}
	/*AT24C02任务处理*/
	if(AT24C02_flag==1)
	{
		AT24C02_flag=0;
		P_State=0;	
		AT24C02_WriteByte(0X00,Change_Num);
		Delay5ms();
	}
	/*DAC任务处理*/
	if(DAC_flag==0)		//启动状态
	{
		if(Distance<=D_P){PCF8591_WriteDA((2/5.0)*255);}
		else if(Distance>D_P){PCF8591_WriteDA((4/5.0)*255);}
	}
	else		//关闭状态
	{
		PCF8591_WriteDA((0.4/5.0)*255);
	}
	
	/*Uart任务处理*/
	if(Rec_flag==1)
	{
		Rec_flag=0;
		/*查询数据指令*/
		if(strcmp(Rec_Buf,"ST")==0)	
		{
			sprintf(Send_Buf,"$%d,%.2f\r\n",Distance,T);
			Uart_SendString(Send_Buf);
		}
		/*查询参数指令*/
		else if(strcmp(Rec_Buf,"PARA")==0)
		{
			sprintf(Send_Buf,"#%d,%d\r\n",(ui)D_P,(ui)T_P);
			Uart_SendString(Send_Buf);
		}
		else
		{
			Uart_SendString("ERROR\r\n");
		}
	}
	/*
	sprintf(Send_Buf,"#%d,%d\r\n",D_P,T_P)打印乱码;
	查找原因：原来%d，代表的是将int型数据转换为十进制字符的作用，既然是int型，
	那他占用空间为至少2个字节，那么我们定义的D_P和T_P仅是char 占空间一字节，
	所以函数在转换时，会错误的转换temp空间外的一字节空间的数据，这是未知的数据，所以会出现打印乱码情况。
	*/
}
/*应用LED函数（为防止闪烁，运行时关闭中断）*/
void USE_LED()
{
	LED_Init();
	if(L1_flag==1){L1=0;}
	else{L1=1;}
	if(L2_flag==1){L2=0;}
	else{L2=1;}
	if(DAC_flag==0){L3=0;}
	else{L3=1;}
	L4=1;L5=1;L6=1;L7=1;L8=1;
	
	P2&=0X1F;
}

/******应用按键函数（长按：超过一秒则触发长按。短按：按下时间没超过1s弹起触发短按）******/
void USE_KEY()
{
	if(KEY_flag==1)
	{
		KEY_flag=0;
		KeyNum=KEY();
		/* "短按键(按下未超过1s弹起触发)"S13-切换主界面（数据、参数） */
		if(Cont==13&&K13_flag==0&&K13_Press==0)
		{
			Cont=0;
			Interface=~Interface;
			Interface_P=0;
			Interface_Data=0;
			/*参数变动-触发EEPROM保存变动次数*/
			if(Interface==0)
			{
				if(P_State==1)
				{
					Change_Num++;
					AT24C02_flag=1;
				}
			}
		}
		/* "长按键(按下超过1s触发)"S13-切换DAC输出功能 */
		else if(Cont==13&&K13_flag==1)
		{
			Cont=0;
			K13_flag=0;
			DAC_flag=~DAC_flag;
		}
		
		/* "短按键(按下未超过1s弹起触发)"S12-切换子界面（数据、参数） */
		if(Cont==12&&K12_flag==0&&K12_Press==0)
		{
			Cont=0;
			if(Interface==0){Interface_Data++;Interface_Data%=3;}
			else if(Interface==1){Interface_P=~Interface_P;}
		}
		/* "长按键(按下超过1s触发)"S12-重置参数变动次数 */
		else if(Cont==12&&K12_flag==1)
		{
			Cont=0;
			K12_flag=0;
			Change_Num=0;
			AT24C02_flag=1;
		}
		
		/*S16-参数减按键*/
		if(KeyNum==16)
		{
			if(Interface_P==0){T_P-=2;P_State=1;}
			else{D_P-=5;P_State=1;}
			
			/*参数范围处理*/
			if(T_P<0){T_P=T_P+100;}
			if(D_P<0){D_P=D_P+100;}
			
		}
		/*S17-参数加按键*/
		if(KeyNum==17)
		{
			if(Interface_P==0){T_P+=2;P_State=1;}
			else{D_P+=5;P_State=1;}
			
			/*参数范围处理*/
			if(T_P>99){T_P=T_P-100;}
			if(D_P>99){D_P=D_P-100;}
		}
	}
}

/******应用数码管函数******/
void USE_SMG()
{
	/*数据界面*/
	if(Interface==0)
	{
		/*温度数据显示*/
		if(Interface_Data==0)
		{
			Nixie_SetBuf(0,Nodot_SMG[12]);	//标识符-C
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,Nodot_SMG[((ui)(T*100))/1000]);
			Nixie_SetBuf(5,dot_SMG[((ui)(T*100))/100%10]);
			Nixie_SetBuf(6,Nodot_SMG[((ui)(T*100))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(T*100))%10]);
		}
		/*距离数据显示*/
		else if(Interface_Data==1)
		{
			Nixie_SetBuf(0,0XC7);	//标识符-L
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[Distance/10]);
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
		/*变更次数显示*/
		else if(Interface_Data==2)
		{
			Nixie_SetBuf(0,0XC8);	//标识符-n
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			if(Change_Num>9999){Nixie_SetBuf(3,Nodot_SMG[Change_Num/10000]);}
			else{Nixie_SetBuf(3,0XFF);}
			if(Change_Num>999){Nixie_SetBuf(4,Nodot_SMG[Change_Num/1000%10]);}
			else{Nixie_SetBuf(4,0XFF);}
			if(Change_Num>99){Nixie_SetBuf(5,Nodot_SMG[Change_Num/100%10]);}
			else{Nixie_SetBuf(5,0XFF);}
			if(Change_Num>9){Nixie_SetBuf(6,Nodot_SMG[Change_Num/10%10]);}
			else{Nixie_SetBuf(6,0XFF);}
			
			Nixie_SetBuf(7,Nodot_SMG[Change_Num%10]);
		}
	}
	/*参数界面*/
	else
	{
		/*温度参数*/
		if(Interface_P==0)
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,Nodot_SMG[1]);	//编号1
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[T_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[T_P%10]);
		}
		/*距离参数*/
		else
		{
			Nixie_SetBuf(0,0X8C);	//标识符-P
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,Nodot_SMG[2]);	//编号2
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[D_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[D_P%10]);
		}
	}
}

void main()
{
	Hardware_Init();
	while(1)
	{
		if(led_run==1){led_run=0;USE_LED();}
		
		USE_SMG();
		USE_KEY();
		Task_Processing();
	}
}

void Timer0_Routine() interrupt 1
{
	static uc SMG_Count;
	static uc KEY_Count;
	static uc DS18B20_Count;
	static uc Sonic_Count;
	static ui K12_Count;
	static ui K13_Count;
	
	led_run=1;	//LED运行不受中断影响
	
	KEY_Count++;
	if(KEY_Count==10)
	{
		KEY_Count=0;
		KEY_flag=1;
	}
	SMG_Count++;
	if(SMG_Count==2)
	{
		SMG_Count=0;
		Nixie_Loop();
	}
	DS18B20_Count++;
	if(DS18B20_Count==200)
	{
		DS18B20_Count=0;
		DS18B20_flag=1;
	}
	Sonic_Count++;
	if(Sonic_Count==200)
	{
		Sonic_Count=0;
		Sonic_flag=1;
	}
	/*K12长短按判断*/
	if(K12_Press==1)
	{
		K12_Count++;
		if(K12_Count>=1000)
		{
			K12_flag=1;
			K12_Press=0;
			K12_Count=0;
		}
	}
	else{K12_Count=0;K12_flag=0;}
	/*K13长短按判断*/
	if(K13_Press==1)
	{
		K13_Count++;
		if(K13_Count>=1000)
		{
			K13_flag=1;
			K13_Press=0;
			K13_Count=0;
		}
	}
	else{K13_Count=0;K13_flag=0;}
}

void Uart_Routine(void) interrupt 4
{
	uc temp;
	bit Rx_State;
	static uc i;
	
	if(RI==1)
	{
		RI=0;
		temp=SBUF;
		
		if(Rx_State==0)
		{
			if(temp=='\r')
			{
				Rx_State=1;
			}
			else
			{
				Rec_Buf[i++]=temp;
			}
		}
		else if(Rx_State==1)
		{
			if(temp=='\n')
			{
				Rec_flag=1;	//表示数据接收完
				Rec_Buf[i]='\0';
				i=0;
				Rx_State=0;
			}
		}
	}
}