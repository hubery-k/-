#ifndef __PCF8591_H
#define __PCF8591_H
#include <STC15F2K60S2.H>
#include "Define.h"
#include "iic.h"

/*�Ӽ��豸��ַ+дָ��==0X90   �ӻ��豸��ַ+����ַ==0X91 */
#define PCF8591_ADDRESS 0X90
#define COMMAND_AIN1 0X01					//��������
#define COMMAND_AIN3 0X03					//�ɵ�����

uc PCF8591_ReadAD();
#endif