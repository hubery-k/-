#include "main.h"
#include "string.h"
#include <stdio.h>
/******����������******/
uc Interface_Data;
uc KeyNum;
f T;
ui Distance;
ui Change_Num;
c T_P=30;	//�¶Ȳ�������Χ��0~99��
c D_P=35;	//�����������Χ��0~99��
uc Send_Buf[20];
uc Rec_Buf[15];

bit Interface;	//�������־�����ݽ��桢�������棩
bit Interface_P;	//�ӽ����־
bit KEY_flag;	//������ֵ��ȡ��־
bit DS18B20_flag;	//�¶����ݲɼ���־
bit Sonic_flag;	//���������ɼ���־
bit AT24C02_flag;	//д��EEPROM��־
bit P_State;	//�����䶯״̬
bit Rec_flag;	//���ڷ��ͱ�־
bit DAC_flag;	//DAC���ر�־
bit K12_flag;	//K12���̰���־
bit K13_flag;	//K13���̰���־
bit L1_flag;	//L1�����־
bit L2_flag;	//L2�����־
bit led_run;
/******Ӳ����ʼ������******/
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
	/*DS18B20������*/
	if(DS18B20_flag==1)
	{
		DS18B20_flag=0;
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		/*L1*/
		if(T>T_P){L1_flag=1;}
		else{L1_flag=0;}
	}
	/*Sonic������*/
	if(Sonic_flag==1)
	{
		Sonic_flag=0;
		Distance=Sonic_Work();
		if(Distance>99){Distance=99;}
		/*L2*/
		if(Distance<D_P){L2_flag=1;}
		else{L2_flag=0;}
	}
	/*AT24C02������*/
	if(AT24C02_flag==1)
	{
		AT24C02_flag=0;
		P_State=0;	
		AT24C02_WriteByte(0X00,Change_Num);
		Delay5ms();
	}
	/*DAC������*/
	if(DAC_flag==0)		//����״̬
	{
		if(Distance<=D_P){PCF8591_WriteDA((2/5.0)*255);}
		else if(Distance>D_P){PCF8591_WriteDA((4/5.0)*255);}
	}
	else		//�ر�״̬
	{
		PCF8591_WriteDA((0.4/5.0)*255);
	}
	
	/*Uart������*/
	if(Rec_flag==1)
	{
		Rec_flag=0;
		/*��ѯ����ָ��*/
		if(strcmp(Rec_Buf,"ST")==0)	
		{
			sprintf(Send_Buf,"$%d,%.2f\r\n",Distance,T);
			Uart_SendString(Send_Buf);
		}
		/*��ѯ����ָ��*/
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
	sprintf(Send_Buf,"#%d,%d\r\n",D_P,T_P)��ӡ����;
	����ԭ��ԭ��%d��������ǽ�int������ת��Ϊʮ�����ַ������ã���Ȼ��int�ͣ�
	����ռ�ÿռ�Ϊ����2���ֽڣ���ô���Ƕ����D_P��T_P����char ռ�ռ�һ�ֽڣ�
	���Ժ�����ת��ʱ��������ת��temp�ռ����һ�ֽڿռ�����ݣ�����δ֪�����ݣ����Ի���ִ�ӡ���������
	*/
}
/*Ӧ��LED������Ϊ��ֹ��˸������ʱ�ر��жϣ�*/
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

/******Ӧ�ð�������������������һ���򴥷��������̰�������ʱ��û����1s���𴥷��̰���******/
void USE_KEY()
{
	if(KEY_flag==1)
	{
		KEY_flag=0;
		KeyNum=KEY();
		/* "�̰���(����δ����1s���𴥷�)"S13-�л������棨���ݡ������� */
		if(Cont==13&&K13_flag==0&&K13_Press==0)
		{
			Cont=0;
			Interface=~Interface;
			Interface_P=0;
			Interface_Data=0;
			/*�����䶯-����EEPROM����䶯����*/
			if(Interface==0)
			{
				if(P_State==1)
				{
					Change_Num++;
					AT24C02_flag=1;
				}
			}
		}
		/* "������(���³���1s����)"S13-�л�DAC������� */
		else if(Cont==13&&K13_flag==1)
		{
			Cont=0;
			K13_flag=0;
			DAC_flag=~DAC_flag;
		}
		
		/* "�̰���(����δ����1s���𴥷�)"S12-�л��ӽ��棨���ݡ������� */
		if(Cont==12&&K12_flag==0&&K12_Press==0)
		{
			Cont=0;
			if(Interface==0){Interface_Data++;Interface_Data%=3;}
			else if(Interface==1){Interface_P=~Interface_P;}
		}
		/* "������(���³���1s����)"S12-���ò����䶯���� */
		else if(Cont==12&&K12_flag==1)
		{
			Cont=0;
			K12_flag=0;
			Change_Num=0;
			AT24C02_flag=1;
		}
		
		/*S16-����������*/
		if(KeyNum==16)
		{
			if(Interface_P==0){T_P-=2;P_State=1;}
			else{D_P-=5;P_State=1;}
			
			/*������Χ����*/
			if(T_P<0){T_P=T_P+100;}
			if(D_P<0){D_P=D_P+100;}
			
		}
		/*S17-�����Ӱ���*/
		if(KeyNum==17)
		{
			if(Interface_P==0){T_P+=2;P_State=1;}
			else{D_P+=5;P_State=1;}
			
			/*������Χ����*/
			if(T_P>99){T_P=T_P-100;}
			if(D_P>99){D_P=D_P-100;}
		}
	}
}

/******Ӧ������ܺ���******/
void USE_SMG()
{
	/*���ݽ���*/
	if(Interface==0)
	{
		/*�¶�������ʾ*/
		if(Interface_Data==0)
		{
			Nixie_SetBuf(0,Nodot_SMG[12]);	//��ʶ��-C
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,Nodot_SMG[((ui)(T*100))/1000]);
			Nixie_SetBuf(5,dot_SMG[((ui)(T*100))/100%10]);
			Nixie_SetBuf(6,Nodot_SMG[((ui)(T*100))/10%10]);
			Nixie_SetBuf(7,Nodot_SMG[((ui)(T*100))%10]);
		}
		/*����������ʾ*/
		else if(Interface_Data==1)
		{
			Nixie_SetBuf(0,0XC7);	//��ʶ��-L
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,0XFF);
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[Distance/10]);
			Nixie_SetBuf(7,Nodot_SMG[Distance%10]);
		}
		/*���������ʾ*/
		else if(Interface_Data==2)
		{
			Nixie_SetBuf(0,0XC8);	//��ʶ��-n
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
	/*��������*/
	else
	{
		/*�¶Ȳ���*/
		if(Interface_P==0)
		{
			Nixie_SetBuf(0,0X8C);	//��ʶ��-P
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,Nodot_SMG[1]);	//���1
			Nixie_SetBuf(4,0XFF);
			Nixie_SetBuf(5,0XFF);
			Nixie_SetBuf(6,Nodot_SMG[T_P/10]);
			Nixie_SetBuf(7,Nodot_SMG[T_P%10]);
		}
		/*�������*/
		else
		{
			Nixie_SetBuf(0,0X8C);	//��ʶ��-P
			Nixie_SetBuf(1,0XFF);
			Nixie_SetBuf(2,0XFF);
			Nixie_SetBuf(3,Nodot_SMG[2]);	//���2
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
	
	led_run=1;	//LED���в����ж�Ӱ��
	
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
	/*K12���̰��ж�*/
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
	/*K13���̰��ж�*/
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
				Rec_flag=1;	//��ʾ���ݽ�����
				Rec_Buf[i]='\0';
				i=0;
				Rx_State=0;
			}
		}
	}
}