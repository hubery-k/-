#ifndef _MAIN_H_
#define _MAIN_H_
#include "define.h"
#include "key.h"
#include "nixie.h"
#include "delay.h"
//ʱ�����
u8 hour,min,sec;
//��ʱ����ʱ����
u16 s1_count;
//led��־λ��ֻ������״̬��Ϊ���ٿռ�ռ�ã�����Ϊbit,0->off,1->on
bit led8_flag,led7_flag,led6_flag,led5_flag;
//
u8 led_count;
#endif