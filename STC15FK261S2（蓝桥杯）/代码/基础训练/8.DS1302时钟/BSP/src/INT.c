#include "INT.h"
/*breif：IAP15F2K61S2（STC15系列）单片机。
				根据原理图可知，外部中断INT0由P32上的S5按键触发。
				外部中断INT1由P33上的S4按键触发。
				外部中断INT2由P36IO口触发。
				外部中断INT3由P37IO口触发。
*/
				
void INT0_Init(void)
{
	IT0=1;//IT0=0下降沿和上升沿都可以触发，IT0=1下降沿触发
	IE0=0;//中断标志位清0（请求中断由硬件置1，中断响应后由硬件清0）
	EX0=1;
	EA=1;
}
void INT1_Init(void)
{
	IT1=1;//IT0=0下降沿和上升沿都可以触发，IT0=1下降沿触发
	IE1=0;//中断标志位清0（请求中断由硬件置1，中断响应后由硬件清0）
	EX1=1;
	EA=1;
}

/*
//外部中断0的中断函数
void INT0_Routine(void) interrupt 0
{
	//中断标志位IE0中断响应后被清0
}*/
/*
//外部中断1的中断函数
void INT0_Routine(void) interrupt 1
{
	//中断标志位IE0中断响应后被清0
}*/