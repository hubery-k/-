#include "key.h"
u8 key_val=0;
bit key_flag=0;
u8 key_catch[4]={0xff,0xff,0xff,0xff,};
u8 key_state[4]={1,1,1,1};
u8 key_backup[4]={1,1,1,1};
u8 button[4]={7,6,5,4};
void KeyScan()
{
	static i;
	key_catch[0]=(key_catch[0]<<1)|key1;
	key_catch[1]=(key_catch[1]<<1)|key2;
	key_catch[2]=(key_catch[2]<<1)|key3;
	key_catch[3]=(key_catch[3]<<1)|key4;
	for(i=0;i<4;i++)
	{
		if((key_catch[i]&0xff)==0x00)
		{
			key_state[i]=0;
		}
		if((key_catch[i]&0xff)==0xff)
		{
			key_state[i]=1;
		}
	}
}
void Key_Judge()
{
	u8 i;
	for(i=0;i<4;i++)
	{
		if(key_state[i]!=key_backup[i])
		{
			if(key_backup[i]==0)
			{
				key_flag=1;
				key_val=button[i];
			}
			key_backup[i]=key_state[i];
		}
	}
}