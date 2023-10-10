#include "iic.h"
#include "PCF8591.h"

#define PCF8591_ADDRESS 0x90
#define AD_AIN1 0x01		//����ͨ��
#define AD_AIN3 0X03		//��λ��ͨ��

unsigned char Data;

unsigned char AD_Read(bit n)
{
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS);
	I2CWaitAck();
	if(n==0){I2CSendByte(AD_AIN1);}
	else{I2CSendByte(AD_AIN3);}
	I2CWaitAck();
	I2CStop();
	
	I2CStart();
	I2CSendByte(PCF8591_ADDRESS|0X01);
	I2CWaitAck();
	Data=I2CReceiveByte();
	I2CSendAck(1);		//�������ͷ�Ӧ���ʾ����
	I2CStop();
	return Data;
}