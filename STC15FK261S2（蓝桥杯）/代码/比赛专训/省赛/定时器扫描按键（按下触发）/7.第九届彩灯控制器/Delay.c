#include "Delay.h"

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
void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}
