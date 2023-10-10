#include "Delay.h"
#include "intrins.h"

void Delay1000ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 46;
	j = 153;
	k = 245;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay20ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 234;
	j = 115;
	do
	{
		while (--j);
	} while (--i);
}
void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
