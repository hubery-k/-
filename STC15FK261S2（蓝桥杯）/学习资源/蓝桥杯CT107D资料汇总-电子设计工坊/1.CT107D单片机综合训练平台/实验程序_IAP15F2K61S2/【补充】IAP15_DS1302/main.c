/*
注意的问题:1、shi/16  shi%16
		   2、禁止写保护(0x8e,0)
		       开保护(0x8e,0x80)
		   3、(shi/16)*10+shi%10;
		   4、记得把读也写在while(1)里
			 
			 -电子设计工坊
			 -dianshe.taobao.com
*/
#include "ds1302.h"
uchar smg_du[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};
uchar smg_wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar shijian[10]={0,0,0,0,0,0,0,0,0,0};
void delay()
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 22;
	j = 128;
	do
	{
		while (--j);
	} while (--i);
}

void Smg(uchar weishu,uchar Num)
{
uchar i;
for(i=0;i<Num;i++)
{
P0=0;P2=0xc0;P2=0;
P0=shijian[i];P2=0xe0;P2=0;
P0=smg_wei[i+weishu];P2=0xc0;P2=0;
delay();
}
}

void main()
{
uchar shi,fen,miao;
P0=0xff;P2=0x80;P2=0;
P0=0;P2=0xc0;P2=0;
P0=0;P2=0xa0;P2=0;
set_sfm(13,59,50);

while(1)
{
	fen=Read_Ds1302(0x83); 
	shi=Read_Ds1302(0x85);
  miao=Read_Ds1302(0x81);

	shijian[0]=smg_du[shi/16];
	shijian[1]=smg_du[shi%16];
	shijian[2]=smg_du[10];
	shijian[3]=smg_du[fen/16];
	shijian[4]=smg_du[fen%16];
	shijian[5]=smg_du[10];
	shijian[6]=smg_du[miao/16];
	shijian[7]=smg_du[miao%16];
	Smg(0,8);
}
}