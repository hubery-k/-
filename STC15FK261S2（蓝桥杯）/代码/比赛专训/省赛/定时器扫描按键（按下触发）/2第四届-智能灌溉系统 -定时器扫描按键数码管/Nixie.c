#include "Nixie.h"

//数码管显示缓存区
unsigned char Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

void Nixie_Init(unsigned char n)
{
	P2&=0X1F;//锁存
	if(n==1)//Com端口清除
	{
		P0=0x00;//全都不选中（高电平选中）
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)//段选清除
	{
		P0=0xFF;//全都不选中（低电平选中）
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,unsigned char value)
{
	Nixie_Init(2);
	
	Nixie_Init(1);
	P0|=0x01<<pos;
	Nixie_Init(2);
	P0&=value;
	
	P2&=0X1F;//锁存
}

/**
  * @brief  数码管驱动函数，在中断中调用
  * @param  无
  * @retval 无
  */
void Nixie_Loop(void)
{
	static unsigned char i=0;
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	if(i>=8){i=0;}
}

/**
  * @brief  设置显示缓存区
  * @param  Location 要设置的位置，范围：1~8
  * @param  Number 要设置的数字，范围：段码表索引范围
  * @retval 无
  */
void Nixie_SetBuf(unsigned char Location,Number)
{
	Nixie_Buf[Location]=Number;
}