#include "Delay.h"
#include "intrins.h"

//进入函数和退出函数各4个机械周期
void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 216;
	j = 37;
	do
	{
		while (--j);
	} while (--i);
}


void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}


void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
