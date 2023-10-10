#include "RB.h"

//数据缓存区，通过在主循环中设置数据缓存区然后在定时器中不断扫描显示
unsigned char Nixie_Buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

/*Com端高电平导通，段选低电平导通*/
void Nixie_Init(unsigned char n)
{
	P2&=0X1F;
	if(n==1)		//Com端选
	{
		P0=0X00;		//都不选中
		P2=(P2&0X1F)|0XC0;
	}
	else if(n==2)		//段选
	{
		P0=0XFF;		//都不点亮
		P2=(P2&0X1F)|0XE0;
	}
}

void Nixie_Display(unsigned char pos,value)
{
	Nixie_Init(2);
	
	Nixie_Init(1);
	P0|=0x01<<pos;
	Nixie_Init(2);
	P0&=value;
	P2&=0X1F;
}

/*数码管循环函数。放在定时器扫描*/
void Nixie_Loop(void)
{
	static unsigned char i=0;		//局部变量调用之前需要赋初值
	Nixie_Display(i,Nixie_Buf[i]);
	i++;
	if(i>8){i=0;}
}

/*设置数据缓存区*/
void Nixie_SetBuf(unsigned char Location,Number)		
{
	Nixie_Buf[Location]=Number;
}