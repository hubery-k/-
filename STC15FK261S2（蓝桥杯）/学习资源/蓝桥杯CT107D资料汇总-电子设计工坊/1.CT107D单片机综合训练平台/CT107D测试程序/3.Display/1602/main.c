#include "reg52.h"
#include "intrins.h"

unsigned char text1[] = "LCD 1602 TEST" ;
unsigned char text2[] = "WWW.GXCT.NET" ;

sbit RS = P2^0;
sbit RW = P2^1; 
sbit EN = P1^2;

void delay_ms(unsigned char t);
void init_1602(void);
void write_command(unsigned char cmd);
void write_data(unsigned char dat);
void dsp_onechar(unsigned char pos,unsigned char c);
void dsp_string(unsigned char line,unsigned char *p,unsigned char length);

void main(void)
{
	init_1602();
	delay_ms(15);
	
	dsp_string(0,text1,13);
	dsp_string(1,text2,12);
	
	while(1);	
}

void delay_ms(unsigned char t)
{
	unsigned char i;
	while(t--)
	{
		for(i=0;i<112;i++);	
	}
}

void write_command(unsigned char cmd)
{
	delay_ms(5); // check busy flag
 	EN = 0;
 	RS = 0;
 	RW = 0;
 	_nop_();
 	EN = 1;
 	P0 = cmd;
 	EN = 0;
}

void write_data(unsigned char dat)
{
 	delay_ms(5); //busy flag check 
	EN = 0;
 	RS = 1;
	RW = 0; 
	_nop_();
	EN = 1;
	P0 = dat;
	EN = 0;
	RS = 0;
}

void dsp_onechar(unsigned char pos,unsigned char c)
{
 	unsigned char p;
 	if (pos>=0x10)
    p=pos+0xb0; 
 	else 
    p=pos+0x80; 
 	write_command(p);
 	write_data(c); 
}

void dsp_string(unsigned char line,unsigned char *p,unsigned char length)
{
 	unsigned char l,i;
 	l=line<<4;
 	for (i=0;i<length;i++)
 	dsp_onechar(l++,*(p+i));
}

void init_1602(void)
{
 	delay_ms(15);
 	write_command(0x38); 
 	write_command(0x38); 
 	write_command(0x38); 
 	write_command(0x06); 
 	write_command(0x0c); 
 	write_command(0x01); 
}

