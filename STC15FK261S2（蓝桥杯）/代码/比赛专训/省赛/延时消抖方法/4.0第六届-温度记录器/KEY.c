#include "KEY.h"
#include "Delay.h"

/*ʹ�ö�������*/
unsigned char KeyNum;
void KEY_Press(void)
{
	
	if(KEY7==0){Delay20ms();while(KEY7==0);Delay20ms();KeyNum=4;}
	if(KEY6==0){Delay20ms();while(KEY6==0);Delay20ms();KeyNum=3;}
	if(KEY5==0){Delay20ms();while(KEY5==0);Delay20ms();KeyNum=2;}
	if(KEY4==0){Delay20ms();while(KEY4==0);Delay20ms();KeyNum=1;}
}
/*��ס��Key�����в�������return�����ؼ�ֵ�����磬����һ�ΰ���KeyNum=1����ô�´ε��øú���ʱ��ʹû�а���������Ȼ�᷵�ؼ�ֵKeyNum=1
	������ʹ��ȫ�ֱ������ؼ�ֵ*/